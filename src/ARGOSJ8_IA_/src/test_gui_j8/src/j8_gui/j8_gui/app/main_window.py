import os
import re
import json
import threading
import mimetypes
import http.server
import socketserver

import requests

from ament_index_python.packages import get_package_share_directory

from PySide6.QtCore import Qt, QUrl
from PySide6.QtWidgets import (
    QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton,
    QLineEdit, QFormLayout, QTabWidget, QTableWidget, QTableWidgetItem,
    QSplitter, QComboBox, QFileDialog, QCheckBox
)
from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtWebEngineCore import (
    QWebEnginePage, QWebEngineSettings, QWebEngineProfile, QWebEngineUrlRequestInterceptor
)
from PySide6.QtWebChannel import QWebChannel
from PySide6.QtGui import QPainter, QColor, QPen, QFont, QTransform
from PySide6.QtCore import QSize


from PySide6.QtWidgets import QSizePolicy, QGridLayout
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Path

from j8_gui.app.map_bridge import MapBridge
from j8_gui.app.followzed_widget import FollowZEDWidget
from j8_gui.app.state_widget import MissionWidget
from j8_gui.app.control_widget import ControlWidget




# ========= entorno WebEngine =========
for k in ('http_proxy','https_proxy','HTTP_PROXY','HTTPS_PROXY'):
    os.environ.pop(k, None)
os.environ.setdefault('NO_PROXY', '127.0.0.1,localhost')
flags = os.environ.get('QTWEBENGINE_CHROMIUM_FLAGS', '')
add = ' --disable-gpu --disable-dev-shm-usage --no-sandbox'
if add not in flags:
    os.environ['QTWEBENGINE_CHROMIUM_FLAGS'] = flags + add


STATES = [
    ('disarmed', 'DISARMED'),
    ('ready', 'Ready'),
    ('standby', 'Standby'),
    ('guided', 'Guided'),
    ('auto', 'Auto'),
    ('follow_zed', 'FollowZED'),
    ('control_ai', 'Control IA'),
    
]


def _resolve_html_path():
    try:
        share = get_package_share_directory('j8_gui')
        p = os.path.join(share, 'resources', 'map.html')
        if os.path.exists(p):
            print(f"[GUI] map.html (install) -> {p}")
            return p
    except Exception:
        pass
    here = os.path.dirname(__file__)
    p2 = os.path.abspath(os.path.join(here, '..', 'resources', 'map.html'))
    if os.path.exists(p2):
        print(f"[GUI] map.html (src) -> {p2}")
        return p2
    raise FileNotFoundError("No encuentro map.html")


class ConsolePage(QWebEnginePage):
    def javaScriptConsoleMessage(self, level, message, lineNumber, sourceID):
        try:
            name = getattr(level, "name", None) or str(level)
        except Exception:
            name = str(level)
        print(f"[JS {name}] {sourceID}:{lineNumber} -> {message}")


class HeaderInjector(QWebEngineUrlRequestInterceptor):
    def interceptRequest(self, info):
        # No tocamos localhost; solo dominios públicos si se usaran directos
        url = info.requestUrl().toString()
        if ("tile.openstreetmap.org" in url or "arcgisonline.com" in url or "esri.com" in url):
            info.setHttpHeader(b"Referer", b"https://localhost/")
            info.setHttpHeader(
                b"User-Agent",
                b"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0 Safari/537.36"
            )

class HUDWidget(QWidget):
    """
    HUD sencillo: cielo/tierra, línea de horizonte con pitch/roll, mira central y aviso DISARMED.
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMinimumHeight(200)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)

        # Estado
        self.pitch_deg = 0.0   # + arriba, - abajo
        self.roll_deg  = 0.0   # + derecha
        self.yaw_deg   = 0.0   # 0..360
        
        # Estado (texto y color)
        self.state_text = "DISARMED"
        self.state_color_override = QColor("#ff4444")  # <- fuerza ROJO para cualquier estado; pon None si quieres mapeo por estado
        self.state_colors = {
            "DISARMED": QColor(255, 80, 80),
            "READY": QColor(80, 200, 120),
            "STANDBY": QColor(255, 190, 60),
            "GUIDED": QColor(120, 190, 255),
            "FOLLOWPATH": QColor(120, 220, 255),
            "FOLLOWZED": QColor(140, 220, 180),
            "CONTROLIA": QColor(180, 160, 255),
        }
        # GPS status
        self.gps_fix = None    # puede ser int (ROS NavSatStatus o APM fix_type) o str
        self.gps_sats = None
        self.gps_hdop = None
        self.gps_label = "GPS: —"
        self.gps_color = QColor(255, 80, 80)  # por defecto rojo
        # Apariencia
        self._sky    = QColor(110, 170, 255)
        self._ground = QColor(170, 140, 100)
        self._lines  = QColor(255, 255, 255)

    def sizeHint(self) -> QSize:
        return QSize(600, 260)

    # --- NUEVO: setter del estado ---
    def set_state(self, text: str):
        t = (text or "").strip()
        self.state_text = t if t else "DISARMED"
        self.update()
    def set_state_color(self, color):  # acepta QColor o '#rrggbb' o None
        if color is None:
            self.state_color_override = None
        else:
            self.state_color_override = QColor(color) if not isinstance(color, QColor) else color
        self.update()

    def set_gps_status(self, fix=None, sats=None, hdop=None, label: str=None):
        """
        fix: puede ser
            - ROS sensor_msgs/NavSatStatus.status:  -1 NO_FIX, 0 FIX, 1 SBAS, 2 GBAS
            - APM/MAVLink fix_type: 0/1 no fix, 2 2D, 3 3D, 4 DGPS, 5 RTK Float, 6 RTK Fixed
            - o None/str
        """
        if sats is not None: self.gps_sats = int(sats)
        if hdop is not None: self.gps_hdop = float(hdop)

        text, color = None, QColor(200, 200, 200)
        if isinstance(fix, int):
            # Map ROS
            ros_map = {
                -1: ("No GPS", QColor(255, 80, 80)),
                0: ("3D Fix", QColor( 80, 220, 120)),
                1: ("SBAS",   QColor( 80, 200, 240)),
                2: ("GBAS",   QColor( 80, 200, 240)),
            }
            # Map APM (si los valores son de 0..6)
            apm_map = {
                0: ("No GPS",     QColor(255, 80, 80)),
                1: ("No Fix",     QColor(255, 80, 80)),
                2: ("2D Fix",     QColor(255, 190, 60)),
                3: ("3D Fix",     QColor( 80, 220, 120)),
                4: ("DGPS",       QColor( 80, 200, 240)),
                5: ("RTK Float",  QColor(160, 120, 255)),
                6: ("RTK Fixed",  QColor(140,  90, 255)),
            }
            if fix in apm_map:
                text, color = apm_map[fix]
            elif fix in ros_map:
                text, color = ros_map[fix]
            else:
                text, color = (f"Fix {fix}", QColor(200, 200, 200))
        elif isinstance(fix, str):
            text = fix
            color = QColor(255, 80, 80) if "NO" in fix.upper() else QColor(80, 220, 120)

        if label is not None:
            text = label

        # Confecciona el texto final con sats/hdop si hay
        parts = [f"GPS: {text or '—'}"]
        if self.gps_sats is not None: parts.append(f"Sats: {self.gps_sats}")
        if self.gps_hdop is not None: parts.append(f"HDOP: {self.gps_hdop:.1f}")
        self.gps_label = "  ".join(parts)
        self.gps_color = color
        self.update()
    # --- API pública ---
    def set_attitude(self, pitch: float=None, roll: float=None, yaw: float=None):
        if pitch is not None: self.pitch_deg = float(pitch)
        if roll  is not None: self.roll_deg  = float(roll)
        if yaw   is not None: self.yaw_deg   = float(yaw) % 360.0
        self.update()

    def set_armed(self, armed: bool):
        self.armed = bool(armed)
        self.update()

    # --- Pintado ---
    def paintEvent(self, ev):
        w = self.width()
        h = self.height()
        cx, cy = w / 2.0, h / 2.0

        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing, True)

        # ----- Cielo/Tierra + horizonte -----
        p.save()
        p.translate(cx, cy)
        p.rotate(-self.roll_deg)

        px_per_deg = h / 120.0
        dy = self.pitch_deg * px_per_deg

        p.setPen(Qt.NoPen)
        p.setBrush(self._sky)
        p.drawRect(-w, -h - dy, 2 * w, h + dy)   # cielo

        p.setBrush(self._ground)
        p.drawRect(-w, -dy, 2 * w, h + dy)       # tierra

        p.setPen(QPen(self._lines, 2))
        p.drawLine(-w, -dy, w, -dy)              # línea horizonte

        # Marcas de pitch
        p.setPen(QPen(self._lines, 1))
        for ang in range(-40, 50, 10):
            y = -(ang * px_per_deg) - dy
            if -h / 2 - 40 <= y <= h / 2 + 40:
                length = 50 if ang % 20 == 0 else 30
                p.drawLine(-length, y, length, y)
                if ang != 0:
                    p.drawText(length + 6, y + 4, f"{ang}")
                    p.drawText(-length - 26, y + 4, f"{ang}")
        p.restore()

        # ----- Mira central -----
        p.setPen(QPen(self._lines, 2))
        p.drawLine(cx - 18, cy, cx + 18, cy)
        p.drawLine(cx, cy - 12, cx, cy + 12)

        # ----- Rumbo (heading) -----
        p.setPen(QPen(self._lines, 1))
        f = QFont(self.font())
        f.setPointSizeF(f.pointSizeF() * 0.9)
        p.setFont(f)
        p.drawText(8, 18, f"HDG {self.yaw_deg:06.2f}°")

        # ----- Estado centrado (color forzado si quieres siempre rojo) -----
        if getattr(self, "state_text", ""):
            txt = str(self.state_text)
            key = txt.upper().replace("_", "").replace(" ", "")
            col = self.state_color_override or self.state_colors.get(key, QColor(255, 255, 255))

            f2 = QFont(self.font()); f2.setBold(True)
            f2.setPointSizeF(f2.pointSizeF() * 1.6)
            p.setFont(f2)
            p.setPen(QPen(col))
            p.drawText(0, 0, w, h, Qt.AlignCenter, txt)

        # ----- Cartela de GPS sobre la "tierra" (abajo-izquierda) -----
        if getattr(self, "gps_label", None):
            txt = self.gps_label
            f3 = QFont(self.font())
            f3.setPointSizeF(f3.pointSizeF() * 0.95)
            p.setFont(f3)

            metrics = p.fontMetrics()
            pad_x, pad_y = 10, 6
            tw = metrics.horizontalAdvance(txt)
            th = metrics.height()
            rect_w = tw + pad_x * 2
            rect_h = th + pad_y * 2

            # Colocar 12px desde el borde inferior-izquierdo
            rx, ry = 12, h - rect_h - 10

            # Fondo semitransparente
            p.setPen(Qt.NoPen)
            p.setBrush(QColor(0, 0, 0, 140))
            p.drawRoundedRect(rx, ry, rect_w, rect_h, 8, 8)

            # Texto
            p.setPen(QPen(self.gps_color))
            p.drawText(rx + pad_x, ry + pad_y + metrics.ascent(), txt)





class StatTile(QWidget):
    def __init__(self, title: str, unit: str="", color: QColor=QColor("#66E")):
        super().__init__()
        self.title = QLabel(title)
        self.title.setStyleSheet("color:#BBB; font-size:12px;")
        self.value = QLabel("0.00")
        self.value.setStyleSheet(f"color:{color.name()}; font-weight:800; font-size:34px;")
        self.unit  = QLabel(unit)
        self.unit.setStyleSheet("color:#999; font-size:12px;")

        box = QVBoxLayout(self); box.setContentsMargins(10,8,10,8); box.setSpacing(2)
        box.addWidget(self.title)
        box.addWidget(self.value)
        box.addWidget(self.unit)

        self.setStyleSheet("background:#111; border-radius:10px;")
    
    def set_value(self, text: str):
        self.value.setText(text)

class StatsPanel(QWidget):
    """Cuadrícula 3x2 con tiles: Altitude, GroundSpeed, Dist to WP, Yaw, Vertical Speed, DistToMAV"""
    def __init__(self):
        super().__init__()
        grid = QGridLayout(self); grid.setContentsMargins(0,0,0,0); grid.setSpacing(8)

        self.t_alt   = StatTile("Altitude (m)",        "", QColor("#E88"))
        self.t_gspd  = StatTile("GroundSpeed (m/s)",   "", QColor("#6AE"))
        self.t_dwp   = StatTile("Dist to WP (m)",      "", QColor("#6AE"))
        self.t_yaw   = StatTile("Yaw (deg)",           "", QColor("#6E6"))
        self.t_vspd  = StatTile("Vertical Speed (m/s)", "", QColor("#6AE"))
        self.t_dmav  = StatTile("DistToMAV (m)",       "", QColor("#EE6"))

        tiles = [self.t_alt, self.t_gspd, self.t_dwp, self.t_yaw, self.t_vspd, self.t_dmav]
        for i, tile in enumerate(tiles):
            r, c = divmod(i, 3)
            grid.addWidget(tile, r, c)

        self.setStyleSheet("background:#0A0A0A; border:1px solid #222; border-radius:12px; padding:8px;")

    def update_stats(self, **kw):
        if "altitude" in kw:     self.t_alt.set_value(f"{kw['altitude']:.2f}")
        if "groundspeed" in kw:  self.t_gspd.set_value(f"{kw['groundspeed']:.2f}")
        if "dist_wp" in kw:      self.t_dwp.set_value(f"{kw['dist_wp']:.2f}")
        if "yaw" in kw:          self.t_yaw.set_value(f"{kw['yaw']:.2f}")
        if "vspeed" in kw:       self.t_vspd.set_value(f"{kw['vspeed']:.2f}")
        if "dist_mav" in kw:     self.t_dmav.set_value(f"{kw['dist_mav']:.2f}")



# ====== Proxy de tiles + servidor estático /ui/* ======
class _TileProxyHandler(http.server.BaseHTTPRequestHandler):
    ROOT_DIR = None  # se setea desde TileProxy.start(root_dir)

    UPSTREAMS = {
        'osm':  'https://tile.openstreetmap.org/{z}/{x}/{y}.png',
        'esri': 'https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',
    }
    session = requests.Session()
    timeout = 15
    headers = {
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 '
                      '(KHTML, like Gecko) Chrome/122.0 Safari/537.36',
        'Referer': 'https://localhost/'
    }
    _re_tiles = re.compile(r'^/(osm|esri)/(\d+)/(\d+)/(\d+)\.(png|jpg)$')

    def do_GET(self):
        path = self.path.split('?', 1)[0]

        # 1) /ui/* -> servir ficheros estáticos (map.html, vendor/leaflet/*, etc.)
        if path.startswith('/ui/'):
            if not self.ROOT_DIR:
                self.send_error(500, "ROOT_DIR no configurado"); return
            rel = path[len('/ui/'):]  # lo que venga detrás
            fs_path = os.path.normpath(os.path.join(self.ROOT_DIR, rel))
            # seguridad: que no escape del directorio raíz
            if not fs_path.startswith(os.path.abspath(self.ROOT_DIR)):
                self.send_error(403); return
            if not os.path.exists(fs_path):
                self.send_error(404); return
            if os.path.isdir(fs_path):
                self.send_error(403); return
            ctype, _ = mimetypes.guess_type(fs_path)
            if not ctype: ctype = 'application/octet-stream'
            try:
                with open(fs_path, 'rb') as f:
                    data = f.read()
                self.send_response(200)
                self.send_header('Content-Type', ctype)
                self.send_header('Cache-Control', 'no-cache')
                self.end_headers()
                self.wfile.write(data)
            except Exception as e:
                self.send_error(500, str(e))
            return

        # 2) /osm/... o /esri/... -> proxy a upstream
        m = self._re_tiles.match(path)
        if m:
            kind, z, x, y, ext = m.groups()
            url = self.UPSTREAMS[kind].format(z=z, x=x, y=y)
            try:
                r = self.session.get(url, headers=self.headers, timeout=self.timeout, stream=True)
                ct = r.headers.get('Content-Type', 'image/png')
                if r.status_code != 200:
                    print(f"[tiles] {url} -> HTTP {r.status_code}")
                    self.send_response(502)
                    self.send_header('Content-Type', 'text/plain; charset=utf-8')
                    self.end_headers()
                    self.wfile.write(f'Upstream {kind} {r.status_code}\n'.encode())
                    return
                self.send_response(200)
                self.send_header('Content-Type', ct)
                self.send_header('Cache-Control', 'public, max-age=86400')
                self.end_headers()
                for chunk in r.iter_content(8192):
                    if chunk:
                        self.wfile.write(chunk)
            except Exception as e:
                print(f"[tiles] ERROR {url}: {e}")
                self.send_response(502)
                self.send_header('Content-Type', 'text/plain; charset=utf-8')
                self.end_headers()
                self.wfile.write(f'Error fetching {url}: {e}\n'.encode())
            return

        # 3) cualquier otra ruta
        self.send_error(404)

    def log_message(self, *args, **kwargs):
        pass


class TileProxy:
    """ http://127.0.0.1:<port> sirve:
        - /ui/<archivos>  (map.html, vendor/leaflet/*)
        - /osm/{z}/{x}/{y}.png
        - /esri/{z}/{y}/{x}
    """
    def __init__(self, host='127.0.0.1', port=0, root_dir=None):
        self._host = host
        self._port = port
        self._server = None
        self.port = None
        self.thread = None
        self.root_dir = root_dir

    def start(self, root_dir=None):
        if root_dir:
            self.root_dir = root_dir
        if not self.root_dir:
            raise ValueError("TileProxy.start necesita root_dir para /ui/*")
        _TileProxyHandler.ROOT_DIR = os.path.abspath(self.root_dir)
        self._server = socketserver.ThreadingTCPServer((self._host, self._port), _TileProxyHandler)
        self._server.daemon_threads = True
        self.port = self._server.server_address[1]
        self.thread = threading.Thread(target=self._server.serve_forever, daemon=True)
        self.thread.start()
        print(f"[tiles] proxy+static en http://127.0.0.1:{self.port}  (root: {self.root_dir})")
        return self.port

    def stop(self):
        if self._server:
            self._server.shutdown()
            self._server.server_close()
            self._server = None


# ====== ROS ======
class RosSide(Node):
    def __init__(self):
        super().__init__('j8_gui_node')

        # --- Compatibilidad con GUI_pkg (parámetros y defaults) ---
        from j8_gui.ros_api import J8GuiDefaults, J8GuiParams, namespaced

        p = J8GuiParams()
        d = J8GuiDefaults()

        # Nota: GUI_pkg fija el namespace internamente a 'ARGJ801'.
        # Aquí lo dejamos como parámetro, pero el default debe ser ARGJ801.
        self.declare_parameter('namespace', p.namespace)
        namespace = self.get_parameter('namespace').get_parameter_value().string_value or p.namespace

        # services (mismos parámetros que GUI_pkg)
        self.declare_parameter(p.fsm_change_fsm_mode_srv_name, d.fsm_change_fsm_mode_srv_name)
        self.declare_parameter(p.fsm_get_fsm_srv_name, d.fsm_get_fsm_srv_name)
        self.declare_parameter(p.fsm_get_possible_transition_srv_name, d.fsm_get_possible_transition_srv_name)
        self.declare_parameter(p.path_planner_srv_name, d.path_planner_srv_name)
        self.declare_parameter(p.receive_ll_path_srv_name, d.receive_ll_path_srv_name)
        self.declare_parameter(p.read_path_service, d.read_path_service)
        self.declare_parameter(p.write_path_service, d.write_path_service)
        self.declare_parameter(p.return_path_service, d.return_path_service)
        self.declare_parameter(p.config_controller_srv_name, d.config_controller_srv_name)
        self.declare_parameter(p.change_controller_srv_name, d.change_controller_srv_name)
        self.declare_parameter(p.config_pure_pursuit_srv_name, d.config_pure_pursuit_srv_name)
        self.declare_parameter(p.config_stanley_srv_name, d.config_stanley_srv_name)
        self.declare_parameter(p.config_dynamic_pure_srv_name, d.config_dynamic_pure_srv_name)
        self.declare_parameter(p.config_dynamic_la_pure_srv_name, d.config_dynamic_la_pure_srv_name)
        self.declare_parameter(p.config_regulated_pure_srv_name, d.config_regulated_pure_srv_name)
        self.declare_parameter(p.enable_security_check_srv_name, d.enable_security_check_srv_name)
        self.declare_parameter(p.get_security_check_srv_name, d.get_security_check_srv_name)

        srv_change_fsm = namespaced(namespace, self.get_parameter(p.fsm_change_fsm_mode_srv_name).value)
        srv_get_state = namespaced(namespace, self.get_parameter(p.fsm_get_fsm_srv_name).value)
        srv_receive_ll_path = namespaced(namespace, self.get_parameter(p.receive_ll_path_srv_name).value)

        # clients (equivalentes a GUI_pkg/ros_classes.py)
        from ctl_mission_interfaces.srv import ChangeMode, GetMode, GetPossibleTransitions
        from path_manager_interfaces.srv import RobotPath

        self._srv_ChangeMode = ChangeMode
        self._srv_GetMode = GetMode
        self._srv_GetPossibleTransitions = GetPossibleTransitions
        self._srv_RobotPath = RobotPath
        self._fsm_mode_emit = None
        self._possible_transitions_emit = None

        self.cli_change_fsm = self.create_client(ChangeMode, srv_change_fsm)
        self.cli_get_state = self.create_client(GetMode, srv_get_state)
        self.cli_get_possible_transitions = self.create_client(
            GetPossibleTransitions,
            namespaced(namespace, self.get_parameter(p.fsm_get_possible_transition_srv_name).value),
        )
        self.cli_send_draw_path = self.create_client(RobotPath, srv_receive_ll_path)

        # Controller config services (ctl_mission CtrlNode)
        from ctl_mission_interfaces.srv import (
            ChangeController,
            ConfigPurePursuitCtrl,
            ConfigRegulatedPureCtrl,
            ConfigStanleyCtrl,
        )

        # Names are parameters in CtrlNode, but the default launch normally sets them.
        # We assume the common service names under namespace.
        self._srv_ChangeController = ChangeController
        self._srv_ConfigPurePursuitCtrl = ConfigPurePursuitCtrl
        self._srv_ConfigRegulatedPureCtrl = ConfigRegulatedPureCtrl
        self._srv_ConfigStanleyCtrl = ConfigStanleyCtrl

        srv_change_ctrl = namespaced(namespace, self.get_parameter(p.change_controller_srv_name).value)
        srv_config_pp = namespaced(namespace, self.get_parameter(p.config_pure_pursuit_srv_name).value)
        srv_config_regulated = namespaced(namespace, self.get_parameter(p.config_regulated_pure_srv_name).value)
        srv_config_stanley = namespaced(namespace, self.get_parameter(p.config_stanley_srv_name).value)

        self.cli_change_controller = self.create_client(ChangeController, srv_change_ctrl)
        self.cli_config_pp = self.create_client(ConfigPurePursuitCtrl, srv_config_pp)
        self.cli_config_regulated = self.create_client(ConfigRegulatedPureCtrl, srv_config_regulated)
        self.cli_config_stanley = self.create_client(ConfigStanleyCtrl, srv_config_stanley)

    # NOTE: no /gui/* publishers. We talk to J8 via services (legacy GUI_pkg contract).

    def set_fsm_feedback_callbacks(self, fsm_mode_emit=None, possible_transitions_emit=None):
        self._fsm_mode_emit = fsm_mode_emit
        self._possible_transitions_emit = possible_transitions_emit

    def send_state(self, key: str):
        # Intentar traducir a transición FSM real (GUI_pkg usa servicio ChangeMode)
        # Aquí asumimos que key llega como string numérica o 'int-like'.
        try:
            transition = int(key)
        except Exception:
            transition = None

        if transition is not None and self.cli_change_fsm.service_is_ready():
            req = self._srv_ChangeMode.Request()
            req.transition = transition
            future = self.cli_change_fsm.call_async(req)
            future.add_done_callback(
                lambda fut, transition_id=transition: self._on_change_fsm_done(fut, transition_id)
            )
            self.get_logger().info(f"FSM transition -> {transition}")
        else:
            self.get_logger().warn(f"FSM ChangeMode not ready/invalid key, ignored: {key}")

    def _on_change_fsm_done(self, future, transition_id: int):
        try:
            response = future.result()
        except Exception as exc:
            self.get_logger().warn(f"FSM transition {transition_id} failed: {exc}")
            return

        if getattr(response, 'success', False):
            self.get_logger().info(f"FSM transition {transition_id} accepted")
            self._refresh_fsm_feedback()
        else:
            self.get_logger().warn(f"FSM transition {transition_id} rejected by ctl_mission")

    def _refresh_fsm_feedback(self):
        if self._fsm_mode_emit is not None and self.cli_get_state.service_is_ready():
            try:
                future = self.cli_get_state.call_async(self._srv_GetMode.Request())
                future.add_done_callback(self._on_get_mode_done)
            except Exception as exc:
                self.get_logger().warn(f"GetMode refresh failed: {exc}")

        if self._possible_transitions_emit is not None and self.cli_get_possible_transitions.service_is_ready():
            try:
                future = self.cli_get_possible_transitions.call_async(self._srv_GetPossibleTransitions.Request())
                future.add_done_callback(self._on_get_possible_transitions_done)
            except Exception as exc:
                self.get_logger().warn(f"GetPossibleTransitions refresh failed: {exc}")

    def _on_get_mode_done(self, future):
        try:
            response = future.result()
            if self._fsm_mode_emit is not None:
                self._fsm_mode_emit(int(response.mode))
        except Exception as exc:
            self.get_logger().warn(f"GetMode result failed: {exc}")

    def _on_get_possible_transitions_done(self, future):
        try:
            response = future.result()
            if self._possible_transitions_emit is not None:
                self._possible_transitions_emit([int(x) for x in response.possible_transitions])
        except Exception as exc:
            self.get_logger().warn(f"GetPossibleTransitions result failed: {exc}")

    def send_guided(self, lat: float, lon: float):
        # Legacy GUI_pkg doesn't have a guided-target API.
        self.get_logger().warn(
            "Guided target requested but J8 has no known guided-target API in the legacy contract"
        )

    def send_path(self, pts):
        path = Path()
        path.header.frame_id = 'wgs84'
        path.header.stamp = self.get_clock().now().to_msg()
        for (lat, lon) in pts:
            ps = PoseStamped()
            ps.header.frame_id = 'wgs84'
            ps.pose.position.x = float(lon)
            ps.pose.position.y = float(lat)
            path.poses.append(ps)

        # Envío al J8 via servicio (RobotPath) soportado por GUI_pkg
        if self.cli_send_draw_path.service_is_ready():
            try:
                req = self._srv_RobotPath.Request()
                req.path = path
                self.cli_send_draw_path.call_async(req)
                self.get_logger().info(f"Path -> {len(pts)} waypoints (service)")
                return
            except Exception:
                pass

        self.get_logger().warn(f"RobotPath service not ready, ignored {len(pts)} waypoints")

    def send_cfg(self, cfg: dict):
        """Apply controller selection + parameters.

        Expected dict keys (from ControlWidget):
          - controller_type: str
          - v_forward: float
          - l_ahead_dist: float
          - (optional) k_error_lineal, k_error_angular
          - (optional) look_ahead_dis, r_min
        """

        if not isinstance(cfg, dict):
            self.get_logger().warn(f"send_cfg ignored (not a dict): {type(cfg)}")
            return

        controller_type = str(cfg.get('controller_type', '')).strip()
        if not controller_type:
            self.get_logger().warn('send_cfg ignored: missing controller_type')
            return

        # 1) Change controller type (if service available)
        if self.cli_change_controller.service_is_ready():
            try:
                req = self._srv_ChangeController.Request()
                req.controller_type = controller_type
                self.cli_change_controller.call_async(req)
            except Exception as e:
                self.get_logger().warn(f"ChangeController call failed: {e}")
        else:
            self.get_logger().warn('ChangeController service not ready')

        # 2) Configure parameters
        try:
            v_forward = float(cfg.get('v_forward'))
            l_ahead_dist = float(cfg.get('l_ahead_dist'))
        except Exception:
            self.get_logger().warn('send_cfg ignored: invalid v_forward/l_ahead_dist')
            return

        # Pure pursuit controllers (including dynamic variants) share the PP config surface in this codebase
        if controller_type in {
            'pure_pursuit',
            'dynamic_pure_pursuit',
            'dynamic_la_pure_pursuit',
        }:
            if self.cli_config_pp.service_is_ready():
                try:
                    req = self._srv_ConfigPurePursuitCtrl.Request()
                    req.v_forward = v_forward
                    req.l_ahead_dist = l_ahead_dist
                    self.cli_config_pp.call_async(req)
                    self.get_logger().info(f"ConfigPurePursuitCtrl -> v={v_forward} la={l_ahead_dist}")
                    return
                except Exception as e:
                    self.get_logger().warn(f"ConfigPurePursuitCtrl call failed: {e}")
            else:
                self.get_logger().warn('ConfigPurePursuitCtrl service not ready')
            return

        if controller_type == 'regulated_pure_pursuit':
            try:
                look_ahead_dis = float(cfg.get('look_ahead_dis', l_ahead_dist))
                r_min = float(cfg.get('r_min'))
            except Exception:
                self.get_logger().warn('send_cfg ignored: invalid regulated params (r_min/look_ahead_dis)')
                return

            if self.cli_config_regulated.service_is_ready():
                try:
                    req = self._srv_ConfigRegulatedPureCtrl.Request()
                    req.look_ahead_dis = look_ahead_dis
                    req.v_forward = v_forward
                    req.r_min = r_min
                    self.cli_config_regulated.call_async(req)
                    self.get_logger().info(
                        f"ConfigRegulatedPureCtrl -> la={look_ahead_dis} v={v_forward} r_min={r_min}"
                    )
                except Exception as e:
                    self.get_logger().warn(f"ConfigRegulatedPureCtrl call failed: {e}")
            else:
                self.get_logger().warn('ConfigRegulatedPureCtrl service not ready')
            return

        if controller_type == 'stanley':
            try:
                k_lin = float(cfg.get('k_error_lineal'))
                k_ang = float(cfg.get('k_error_angular'))
            except Exception:
                self.get_logger().warn('send_cfg ignored: invalid stanley params (k_error_lineal/k_error_angular)')
                return

            if self.cli_config_stanley.service_is_ready():
                try:
                    req = self._srv_ConfigStanleyCtrl.Request()
                    req.v_forward = v_forward
                    req.l_ahead_dist = l_ahead_dist
                    req.k_error_lineal = k_lin
                    req.k_error_angular = k_ang
                    self.cli_config_stanley.call_async(req)
                    self.get_logger().info(
                        f"ConfigStanleyCtrl -> v={v_forward} la={l_ahead_dist} k_lin={k_lin} k_ang={k_ang}"
                    )
                except Exception as e:
                    self.get_logger().warn(f"ConfigStanleyCtrl call failed: {e}")
            else:
                self.get_logger().warn('ConfigStanleyCtrl service not ready')
            return

        self.get_logger().warn(
            f"Control config for controller_type='{controller_type}' not mapped yet"
        )


# ====== MainWindow ======
class MainWindow(QMainWindow):
    def __init__(self, defer_ros_start=True):
        super().__init__()
        self.setWindowTitle('J8 GUI — Map + Control + FollowZED')
        self._planned = []
        self._ros = None
        self._defer_ros = defer_ros_start
        self._tile_proxy = None

        self.tabs = QTabWidget()
        self._build_tab_mission()
        self._build_tab_control()

        right = QWidget()
        rv = QVBoxLayout(right); rv.setContentsMargins(0,0,0,0)

        # Map controls
        self.chk_follow_robot = QCheckBox('Seguir robot')
        self.chk_follow_robot.setChecked(True)
        rv.addWidget(self.chk_follow_robot)

        self.web = QWebEngineView()
        rv.addWidget(self.web)

        sp = QSplitter(Qt.Horizontal)
        sp.addWidget(self.tabs)
        sp.addWidget(right)
        sp.setSizes([420, 1000])
        sp.setStretchFactor(0, 0)
        sp.setStretchFactor(1, 1)
        self.setCentralWidget(sp)
        self.resize(1400, 900)

        self._init_map()

    def _build_tab_mission(self):
        # Tab principal (estado + misión) vive en state_widget.py
        self.mission_tab = MissionWidget()
        self.tabs.addTab(self.mission_tab, 'Misión')

    # def _build_tab_control(self):
    #     w = QWidget(); lay = QVBoxLayout(w)
    #     frm = QFormLayout()
    #     self.ed_vlin = QLineEdit('1.0')
    #     self.ed_vang = QLineEdit('0.6')
    #     self.ed_alin = QLineEdit('0.5')
    #     self.ed_aang = QLineEdit('0.8')
    #     frm.addRow('Vel. lineal máx [m/s]:', self.ed_vlin)
    #     frm.addRow('Vel. angular máx [rad/s]:', self.ed_vang)
    #     frm.addRow('Acel. lineal [m/s²]:', self.ed_alin)
    #     frm.addRow('Acel. angular [rad/s²]:', self.ed_aang)
    #     lay.addLayout(frm)

    #     self.cmb_follow = QComboBox(); self.cmb_follow.addItems(['follow1', 'follow2'])
    #     lay.addWidget(QLabel('Modo Follow:')); lay.addWidget(self.cmb_follow)

    #     b_apply = QPushButton('Aplicar configuración'); b_apply.clicked.connect(self._on_apply_cfg)
    #     lay.addWidget(b_apply)
    #     lay.addStretch(1)
    #     self.tabs.addTab(w, 'Control')
    def _build_tab_control(self):
        self.control_tab = ControlWidget(self._ros)
        self.tabs.addTab(self.control_tab, 'Control')


    def _build_tab_follow(self):
        self.follow_tab = FollowZEDWidget(self._ros)
        self.tabs.addTab(self.follow_tab, 'FollowZED')

    def _init_map(self):
        # settings
        self.web.settings().setAttribute(QWebEngineSettings.LocalContentCanAccessFileUrls, True)
        self.web.settings().setAttribute(QWebEngineSettings.LocalContentCanAccessRemoteUrls, True)

        # load logs
        self.web.loadStarted.connect(lambda: print("[WEB] loadStarted"))
        self.web.loadProgress.connect(lambda p: print(f"[WEB] loadProgress {p}%"))
        self.web.loadFinished.connect(lambda ok: print(f"[WEB] loadFinished ok={ok}"))

        # profile
        profile: QWebEngineProfile = self.web.page().profile()
        profile.setHttpUserAgent(
            "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
            "(KHTML, like Gecko) Chrome/122.0 Safari/537.36"
        )
        profile.setHttpAcceptLanguage("es-ES,es;q=0.9,en;q=0.8")
        self._interceptor = HeaderInjector()
        profile.setUrlRequestInterceptor(self._interceptor)

        # WebChannel
        self.bridge = MapBridge(self)
        self.bridge.removeWaypointIndex.connect(self.mission_tab.on_remove_index_from_map)
        # self.bridge.guidedTarget.connect(self._guided_from_map)
        # self.bridge.plannedWaypoint.connect(self._planned_from_map)
        self.bridge.guidedTarget.connect(self.mission_tab.on_guided_from_map)
        self.bridge.plannedWaypoint.connect(self.mission_tab.on_planned_from_map)
        channel = QWebChannel(self.web.page())
        channel.registerObject("bridge", self.bridge)
        self.web.page().setWebChannel(channel)
        self.mission_tab.set_js_call(self._js_call)

        # proxy + estático
        html_path = _resolve_html_path()
        static_root = os.path.dirname(html_path)
        self._tile_proxy = TileProxy(root_dir=static_root)
        port = self._tile_proxy.start()  # sirve /ui/* y /osm /esri
        print(f"[GUI] estático desde: {static_root}")

        # cargar por HTTP, no file://
        url = QUrl(f"http://127.0.0.1:{port}/ui/map.html")
        print(f"[GUI] cargando: {url.toString()}")
        self.web.setUrl(url)

        # Vehicle marker bootstrap: show something immediately so we can tell the JS bridge works
        # even when GPS hasn't arrived yet.
        self._last_vehicle = (0.0, 0.0, None)
        self._have_vehicle_gps = False
        self.web.loadFinished.connect(self._on_map_load_finished)

        # Feed Fixposition pose (already consumed by the State tab) into the web map.
        # We hook the signal here because main_window owns the web view and JS calls.
        try:
            self.mission_tab.state._signals.gps_pose.connect(self._on_gps_pose)
        except Exception:
            # If the widget structure changes, we fail gracefully (map just won't show vehicle).
            pass

    def _on_map_load_finished(self, ok: bool):
        if not ok:
            return
        # 0,0 fallback marker (Greenwich + Equator) to confirm code path is alive.
        lat, lon, heading = self._last_vehicle
        if not self._have_vehicle_gps:
            self._js_update_vehicle(lat, lon, heading, pan=False, no_gps=True)
        else:
            self._js_update_vehicle(lat, lon, heading, pan=False)

    def _on_gps_pose(self, lat: float, lon: float, heading_deg):
        """Update the vehicle marker on the embedded Leaflet map.

        Expects `map.html` to expose `window.updateVehicle(lat, lon[, headingDeg])`.
        """

        try:
            lat_f = float(lat)
            lon_f = float(lon)
        except Exception:
            return

        # Record last known pose.
        self._have_vehicle_gps = True
        self._last_vehicle = (lat_f, lon_f, heading_deg)

        pan = bool(self.chk_follow_robot.isChecked())
        self._js_update_vehicle(lat_f, lon_f, heading_deg, pan=pan, no_gps=False)

    def _js_update_vehicle(self, lat: float, lon: float, heading_deg, pan: bool = False, no_gps: bool = False):
        """Send vehicle update to map.

        - If heading is None: calls updateVehicle(lat, lon)
        - Else: updateVehicle(lat, lon, headingDeg)
        - If pan=True: also panTo(lat, lon)
        """

        try:
            lat_f = float(lat)
            lon_f = float(lon)
        except Exception:
            return

        # Optional style (e.g. different marker when GPS isn't available yet)
        opts_js = ''
        if no_gps:
            # grey marker + badge
            opts_js = ", {color: '#9ca3af', badge: 'NO GPS'}"

        if heading_deg is None:
            self._js_call(f'window.updateVehicle({lat_f}, {lon_f}, undefined{opts_js});')
        else:
            try:
                hdg_f = float(heading_deg) % 360.0
                self._js_call(f'window.updateVehicle({lat_f}, {lon_f}, {hdg_f}{opts_js});')
            except Exception:
                self._js_call(f'window.updateVehicle({lat_f}, {lon_f}, undefined{opts_js});')

        if pan:
            self._js_call(f'window.panTo({lat_f}, {lon_f});')

    # ===== ROS =====
    def start_ros(self):
        if self._ros:
            return
        if not rclpy.ok():
            rclpy.init(args=None)
        self._ros = RosSide()
        self._exec = rclpy.executors.SingleThreadedExecutor()
        self._exec.add_node(self._ros)
        self._th = threading.Thread(target=self._exec.spin, daemon=True)
        self._th.start()
        self._build_tab_follow()
        self.mission_tab.set_ros(self._ros)
        self.mission_tab.attach_ros(self._exec)  # puedes pasar topics={'imu':'/mavros/imu/data', ...}
        self._ros.set_fsm_feedback_callbacks(
            self.mission_tab.state._signals.fsm_mode.emit,
            self.mission_tab.state._signals.possible_transitions.emit,
        )
        self.control_tab.set_ros(self._ros)

    # ===== GUI slots =====
    def _on_change_state(self):
        if not self._ros:
            # Aunque no haya ROS, actualiza el HUD para ver el estado en pantalla
            label = self.cmb_state.currentText()
            self.hud.set_state(label)
            return

        key = self.cmb_state.currentData()
        label = self.cmb_state.currentText()
        self._ros.send_state(key)    # publicas a /gui/state_cmd
        self.hud.set_state(label)    # lo ves en el HUD


    def _on_add_wp(self):
        try:
            lat = float(self.ed_lat.text()); lon = float(self.ed_lon.text())
        except Exception:
            return
        self._planned.append((lat, lon))
        self._refresh_table()
        self._js_set_mission()

    def _on_pan(self):
        try:
            lat = float(self.ed_lat.text()); lon = float(self.ed_lon.text())
        except Exception:
            return
        self._js_call(f'window.panTo({lat}, {lon});')

    def _on_send_path(self):
        if not self._ros: return
        self._ros.send_path(self._planned)

    def _on_clear(self):
        self._planned.clear()
        self._refresh_table()
        self._js_call('window.clearMission();')

    def _on_load_csv(self):
        fn, _ = QFileDialog.getOpenFileName(self, 'CSV de waypoints', filter='CSV (*.csv)')
        if not fn: return
        pts = []
        try:
            with open(fn, 'r') as f:
                for line in f:
                    parts = [p.strip() for p in line.split(',')]
                    if len(parts) < 2: continue
                    lat = float(parts[0]); lon = float(parts[1])
                    pts.append((lat, lon))
        except Exception:
            return
        self._planned = pts
        self._refresh_table()
        self._js_set_mission()

    def _on_apply_cfg(self):
        if not self._ros: return
        try:
            cfg = dict(
                lin_max=float(self.ed_vlin.text()),
                ang_max=float(self.ed_vang.text()),
                acc_lin=float(self.ed_alin.text()),
                acc_ang=float(self.ed_aang.text()),
                follow_mode=self.cmb_follow.currentText(),
            )
        except Exception:
            return
        self._ros.send_cfg(cfg)

    # ===== Bridge callbacks =====
    def _guided_from_map(self, lat, lon):
        if self._ros:
            self._ros.send_guided(lat, lon)
        self._js_call(f'window.panTo({lat}, {lon});')

    def _planned_from_map(self, lat, lon):
        self._planned.append((lat, lon))
        self._refresh_table()
        self._js_set_mission()

    # ===== util =====
    def _refresh_table(self):
        self.tbl.setRowCount(len(self._planned))
        for i, (lat, lon) in enumerate(self._planned):
            self.tbl.setItem(i, 0, QTableWidgetItem(str(i)))
            self.tbl.setItem(i, 1, QTableWidgetItem(f"{lat:.7f}"))
            self.tbl.setItem(i, 2, QTableWidgetItem(f"{lon:.7f}"))

    def _js_call(self, js: str):
        self.web.page().runJavaScript(js)

    def _js_set_mission(self):
        js_array = json.dumps(self._planned)
        self._js_call(f'window.setMission({js_array});')
