import os
import re
import json
import math
import threading
import time
import mimetypes
import http.server
import socketserver

import requests

from ament_index_python.packages import get_package_share_directory

from PySide6.QtCore import Qt, QUrl, QTimer
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

from cuadriga_gui.app.map_bridge import MapBridge
from cuadriga_gui.app.followzed_widget import FollowZEDWidget
from cuadriga_gui.app.state_widget import MissionWidget
from cuadriga_gui.app.control_widget import ControlWidget
from cuadriga_gui.app.person_manager_widget import PersonsWidget




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

ROBOT_ROLE_LABELS = [
    'explorador',
    'cargador',
    'rescatador',
    'mixto',
    'personalizado',
]

ROBOT_DISCOVERY_TOPIC_SUFFIXES = (
    '/fsm_mode',
    '/possible_transitions',
)

ROBOT_DISCOVERY_SERVICE_SUFFIXES = (
    '/change_fsm',
    '/get_fsm_mode',
    '/get_possible_transitions',
    '/receive_ll_path',
)


def _haversine_m(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
    radius_m = 6371000.0
    dlat = math.radians(float(lat2) - float(lat1))
    dlon = math.radians(float(lon2) - float(lon1))
    a = (
        math.sin(dlat / 2.0) ** 2
        + math.cos(math.radians(float(lat1)))
        * math.cos(math.radians(float(lat2)))
        * math.sin(dlon / 2.0) ** 2
    )
    return 2.0 * radius_m * math.atan2(math.sqrt(a), math.sqrt(1.0 - a))


def _resolve_html_path():
    try:
        share = get_package_share_directory('cuadriga_gui')
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
    def __init__(self, namespace: str = 'cuadriga'):
        super().__init__('cuadriga_gui_node')

        # --- Compatibilidad con GUI_pkg (parámetros y defaults) ---
        from cuadriga_gui.ros_api import cuadrigaGuiDefaults, cuadrigaGuiParams, namespaced

        p = cuadrigaGuiParams()
        d = cuadrigaGuiDefaults()

        # Nota: GUI_pkg fija el namespace internamente a 'cuadriga'.
        # Aquí lo dejamos como parámetro, pero el default debe ser cuadriga.
        self.declare_parameter('namespace', namespace or p.namespace)
        namespace = self.get_parameter('namespace').get_parameter_value().string_value or p.namespace
        self.namespace = str(namespace).strip().strip('/') or p.namespace

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

        srv_change_fsm = namespaced(self.namespace, self.get_parameter(p.fsm_change_fsm_mode_srv_name).value)
        srv_get_state = namespaced(self.namespace, self.get_parameter(p.fsm_get_fsm_srv_name).value)
        srv_receive_ll_path = namespaced(self.namespace, self.get_parameter(p.receive_ll_path_srv_name).value)

        # clients (equivalentes a GUI_pkg/ros_classes.py)
        from ctl_mission_interfaces.srv import ChangeMode
        from lifecycle_msgs.srv import GetState
        from path_manager_interfaces.srv import RobotPath, ReadPathFromFile, WritePathToFile, ReturnRobotPath

        self._srv_ChangeMode = ChangeMode
        self._srv_RobotPath = RobotPath
        self._srv_ReadPathFromFile = ReadPathFromFile
        self._srv_WritePathToFile = WritePathToFile
        self._srv_ReturnRobotPath = ReturnRobotPath

        self.cli_change_fsm = self.create_client(ChangeMode, srv_change_fsm)
        self.cli_get_state = self.create_client(GetState, srv_get_state)
        self.cli_send_draw_path = self.create_client(RobotPath, srv_receive_ll_path)

        srv_read_path = namespaced(self.namespace, self.get_parameter(p.read_path_service).value)
        srv_write_path = namespaced(self.namespace, self.get_parameter(p.write_path_service).value)
        srv_return_path = namespaced(self.namespace, self.get_parameter(p.return_path_service).value)
        self.cli_read_path = self.create_client(ReadPathFromFile, srv_read_path)
        self.cli_write_path = self.create_client(WritePathToFile, srv_write_path)
        self.cli_return_path = self.create_client(ReturnRobotPath, srv_return_path)

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

        srv_change_ctrl = namespaced(self.namespace, self.get_parameter(p.change_controller_srv_name).value)
        srv_config_pp = namespaced(self.namespace, self.get_parameter(p.config_pure_pursuit_srv_name).value)
        srv_config_regulated = namespaced(self.namespace, self.get_parameter(p.config_regulated_pure_srv_name).value)
        srv_config_stanley = namespaced(self.namespace, self.get_parameter(p.config_stanley_srv_name).value)

        self.cli_change_controller = self.create_client(ChangeController, srv_change_ctrl)
        self.cli_config_pp = self.create_client(ConfigPurePursuitCtrl, srv_config_pp)
        self.cli_config_regulated = self.create_client(ConfigRegulatedPureCtrl, srv_config_regulated)
        self.cli_config_stanley = self.create_client(ConfigStanleyCtrl, srv_config_stanley)

    # NOTE: no /gui/* publishers. We talk to cuadriga via services (legacy GUI_pkg contract).

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
            self.cli_change_fsm.call_async(req)
            self.get_logger().info(f"FSM transition -> {transition}")
        else:
            self.get_logger().warn(f"FSM ChangeMode not ready/invalid key, ignored: {key}")

    def send_guided(self, lat: float, lon: float):
        # Legacy GUI_pkg doesn't have a guided-target API.
        self.get_logger().warn(
            "Guided target requested but cuadriga has no known guided-target API in the legacy contract"
        )

    def send_path(self, pts):
        path = Path()
        path.header.frame_id = 'll'
        path.header.stamp = self.get_clock().now().to_msg()
        for (lat, lon) in (pts or []):
            ps = PoseStamped()
            ps.header.frame_id = 'll'
            ps.pose.position.x = float(lat)
            ps.pose.position.y = float(lon)
            ps.pose.orientation.w = 1.0
            path.poses.append(ps)

        # Envío al cuadriga via servicio (RobotPath) soportado por GUI_pkg
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

    def save_current_path(self, filename: str):
        name = str(filename or '').strip()
        if not name or not self.cli_write_path.service_is_ready():
            self.get_logger().warn(f"WritePathToFile service not ready/invalid filename: {name}")
            return

        req = self._srv_WritePathToFile.Request()
        req.filename = name
        self.cli_write_path.call_async(req)
        self.get_logger().info(f"Save path -> {name}")

    def load_saved_path(self, filename: str):
        name = str(filename or '').strip()
        if not name or not self.cli_read_path.service_is_ready():
            self.get_logger().warn(f"ReadPathFromFile service not ready/invalid filename: {name}")
            return

        req = self._srv_ReadPathFromFile.Request()
        req.filename = name
        self.cli_read_path.call_async(req)
        self.get_logger().info(f"Load path -> {name}")

    def start_path_following(self):
        self.send_state(0)

    def stop_path_following(self):
        self.send_state(1)

    def clear_active_path(self):
        self.send_path([])

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
        self._planned = []
        self._person_records = []
        self._next_person_id = 1
        self._person_match_threshold_m = 2.0
        self._person_pending_distance_m = 20.0
        self._person_rescued_distance_m = 5.0
        self._auto_person_tracking_enabled = True
        self._ros = None
        self._defer_ros = defer_ros_start
        self._tile_proxy = None
        self._exec = None
        self._th = None
        self._current_namespace = 'cuadriga'
        self._known_robot_namespaces = [self._current_namespace]
        self._robot_roles = {self._current_namespace: 'explorador'}
        self._pending_js_by_key = {}
        self._pending_js_generic = []
        self._js_flush_timer = QTimer(self)
        self._js_flush_timer.setSingleShot(True)
        self._js_flush_timer.timeout.connect(self._flush_js_queue)
        self._namespace_refresh_timer = QTimer(self)
        self._namespace_refresh_timer.setInterval(1500)
        self._namespace_refresh_timer.timeout.connect(self._refresh_robot_namespace_choices)
        self._persons_refresh_timer = QTimer(self)
        self._persons_refresh_timer.setSingleShot(True)
        self._persons_refresh_timer.timeout.connect(self._apply_refresh_person_records_view)

        self.tabs = QTabWidget()
        self._build_tab_mission()
        self._build_tab_control()
        self._build_tab_persons()
        self.tabs.currentChanged.connect(self._on_tab_changed)

        right = QWidget()
        rv = QVBoxLayout(right); rv.setContentsMargins(0,0,0,0)

        robot_row = QHBoxLayout()
        self.cmb_robot = QComboBox()
        self.cmb_robot.setEditable(True)
        self._set_robot_combo_items(self._known_robot_namespaces)
        self.cmb_role = QComboBox()
        self.cmb_role.setEditable(True)
        self.cmb_role.addItems(ROBOT_ROLE_LABELS)
        self.cmb_role.setCurrentText(self._robot_roles[self._current_namespace])
        self.cmb_robot.setCurrentText(self._current_namespace)
        self.btn_apply_robot = QPushButton('Aplicar robot')
        self.btn_apply_robot.clicked.connect(self._on_apply_robot_namespace)
        robot_row.addWidget(QLabel('Robot:'))
        robot_row.addWidget(self.cmb_robot, 1)
        robot_row.addWidget(QLabel('Rol:'))
        robot_row.addWidget(self.cmb_role, 1)
        robot_row.addWidget(self.btn_apply_robot)
        rv.addLayout(robot_row)

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
        self._update_window_title()

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

    def _build_tab_persons(self):
        self.persons_tab = PersonsWidget()
        self.persons_tab.statusChangeRequested.connect(self._on_person_status_change)
        self.persons_tab.assignmentRequested.connect(self._on_person_assignment_change)
        self.persons_tab.autoTrackingChanged.connect(self._on_auto_person_tracking_changed)
        self.persons_tab.thresholdsChanged.connect(self._on_person_thresholds_changed)
        self.persons_tab.set_current_robot(self._current_namespace)
        self.persons_tab.set_current_role(self._robot_roles.get(self._current_namespace, 'explorador'))
        self.persons_tab.set_auto_tracking(self._auto_person_tracking_enabled)
        self.persons_tab.set_thresholds(self._person_pending_distance_m, self._person_rescued_distance_m)
        self.tabs.addTab(self.persons_tab, 'Personas')


    def _build_tab_follow(self):
        self.follow_tab = FollowZEDWidget(self._ros)
        self.tabs.addTab(self.follow_tab, 'FollowZED')

    def _refresh_follow_tab(self):
        if not hasattr(self, 'follow_tab'):
            self._build_tab_follow()
            return

        index = self.tabs.indexOf(self.follow_tab)
        if index < 0:
            self.follow_tab = FollowZEDWidget(self._ros)
            self.tabs.addTab(self.follow_tab, 'FollowZED')
            return

        was_current = self.tabs.currentIndex() == index
        old_follow = self.follow_tab
        self.follow_tab = FollowZEDWidget(self._ros)
        self.tabs.removeTab(index)
        self.tabs.insertTab(index, self.follow_tab, 'FollowZED')
        if was_current:
            self.tabs.setCurrentIndex(index)
        old_follow.deleteLater()

    def _normalized_namespace(self, namespace: str) -> str:
        value = str(namespace or '').strip().strip('/')
        return value or 'cuadriga'

    def _normalize_detected_namespace(self, namespace: str):
        value = str(namespace or '').strip().strip('/')
        return value or None

    def _extract_namespace_from_graph_name(self, graph_name: str, suffix: str):
        name = str(graph_name or '').strip()
        if not name.startswith('/') or not name.endswith(suffix):
            return None

        namespace = name[:-len(suffix)]
        return self._normalize_detected_namespace(namespace)

    def _discover_active_robot_namespaces(self):
        if self._ros is None:
            return []

        namespaces = set()

        try:
            topic_names = self._ros.get_topic_names_and_types()
        except Exception:
            topic_names = []

        for topic_name, _topic_types in topic_names:
            for suffix in ROBOT_DISCOVERY_TOPIC_SUFFIXES:
                namespace = self._extract_namespace_from_graph_name(topic_name, suffix)
                if namespace:
                    namespaces.add(namespace)

        try:
            service_names = self._ros.get_service_names_and_types()
        except Exception:
            service_names = []

        for service_name, _service_types in service_names:
            for suffix in ROBOT_DISCOVERY_SERVICE_SUFFIXES:
                namespace = self._extract_namespace_from_graph_name(service_name, suffix)
                if namespace:
                    namespaces.add(namespace)

        return sorted(namespaces)

    def _set_robot_combo_items(self, namespaces, preserve_text: str = None):
        items = []
        seen = set()
        for namespace in namespaces:
            ns = self._normalize_detected_namespace(namespace)
            if not ns or ns in seen:
                continue
            seen.add(ns)
            items.append(ns)

        current_text = preserve_text
        if current_text is None and hasattr(self, 'cmb_robot'):
            current_text = self.cmb_robot.currentText()
        current_text = self._normalize_detected_namespace(current_text) or self._current_namespace

        if current_text not in seen:
            items.insert(0, current_text)
            seen.add(current_text)

        self._known_robot_namespaces = items

        if not hasattr(self, 'cmb_robot'):
            return

        existing_items = [self.cmb_robot.itemText(i) for i in range(self.cmb_robot.count())]
        if existing_items == items and self.cmb_robot.currentText() == current_text:
            return

        self.cmb_robot.blockSignals(True)
        self.cmb_robot.clear()
        self.cmb_robot.addItems(items)
        self.cmb_robot.setCurrentText(current_text)
        self.cmb_robot.blockSignals(False)

    def _remember_robot_namespace(self, namespace: str):
        ns = self._normalized_namespace(namespace)
        merged = list(self._known_robot_namespaces)
        if ns not in merged:
            merged.append(ns)
        self._set_robot_combo_items(merged, preserve_text=ns)
        return ns

    def _refresh_robot_namespace_choices(self):
        detected = self._discover_active_robot_namespaces()
        preferred = [self._current_namespace]
        if hasattr(self, 'cmb_robot'):
            preferred.append(self.cmb_robot.currentText())

        merged = []
        for namespace in preferred + detected:
            ns = self._normalize_detected_namespace(namespace)
            if ns and ns not in merged:
                merged.append(ns)

        self._set_robot_combo_items(merged)

    def _update_window_title(self):
        role = self._robot_roles.get(self._current_namespace, 'sin rol')
        self.setWindowTitle(f'cuadriga GUI — {self._current_namespace} [{role}] — Map + Control + FollowZED')

    def _build_mission_topics(self, namespace: str):
        from cuadriga_gui.ros_api import namespaced

        ns = self._normalized_namespace(namespace)
        return {
            'fsm_mode': namespaced(ns, 'fsm_mode'),
            'possible_transitions': namespaced(ns, 'possible_transitions'),
            'cte': namespaced(ns, 'cte'),
            'look_ahead_distance': namespaced(ns, 'look_ahead_distance'),
            'min_distance_to_path': namespaced(ns, 'min_distance_to_path'),
            'detected_persons_latlon': namespaced(ns, 'detected_persons_latlon'),
            'detected_persons_global_latlon': '/detected_persons_latlon_global',
        }

    def _connect_robot_namespace(self, namespace: str):
        ns = self._remember_robot_namespace(namespace)
        old_ros = self._ros

        self._ros = RosSide(namespace=ns)
        self._exec.add_node(self._ros)

        self._current_namespace = ns
        self._person_records = []
        self._next_person_id = 1
        self.cmb_robot.setCurrentText(ns)
        self.cmb_role.setCurrentText(self._robot_roles.get(ns, 'explorador'))
        self._update_window_title()

        self.mission_tab.set_ros(self._ros)
        self.mission_tab.attach_ros(self._exec, topics=self._build_mission_topics(ns))
        self.control_tab.set_ros(self._ros)
        self.persons_tab.set_current_robot(ns)
        self.persons_tab.set_current_role(self._robot_roles.get(ns, 'explorador'))
        self.persons_tab.set_auto_tracking(self._auto_person_tracking_enabled)
        self.persons_tab.set_thresholds(self._person_pending_distance_m, self._person_rescued_distance_m)
        self.persons_tab.set_records([])
        self._js_set_detected_persons([])

        if hasattr(self, 'follow_tab'):
            self._refresh_follow_tab()

        if old_ros is not None:
            try:
                self._exec.remove_node(old_ros)
            except Exception:
                pass
            try:
                old_ros.destroy_node()
            except Exception:
                pass

        self._refresh_robot_namespace_choices()

    def _on_apply_robot_namespace(self):
        namespace = self._remember_robot_namespace(self.cmb_robot.currentText())
        role = self.cmb_role.currentText().strip() or 'explorador'
        self._robot_roles[namespace] = role
        if namespace == self._current_namespace and self._ros is not None:
            self.persons_tab.set_current_role(role)
            self._update_window_title()
            return
        if self._exec is None:
            self._current_namespace = namespace
            self.persons_tab.set_current_robot(namespace)
            self.persons_tab.set_current_role(role)
            self._update_window_title()
            return
        try:
            self._connect_robot_namespace(namespace)
        except Exception as e:
            print(f"[GUI] no se pudo conectar al namespace {namespace}: {e}")

    def _on_tab_changed(self, index: int):
        if not hasattr(self, 'follow_tab'):
            return
        if self.tabs.widget(index) is self.follow_tab:
            try:
                self.follow_tab.ensure_started()
            except Exception as e:
                print(f"[GUI] FollowZED init failed: {e}")

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
            self.mission_tab.state._signals.detected_persons.connect(self._on_detected_persons)
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
        self._auto_update_person_statuses(lat_f, lon_f)

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

    def _on_detected_persons(self, points):
        if not isinstance(points, list):
            return
        self._merge_detected_persons(points)
        self._refresh_person_records_view()

    def _merge_detected_persons(self, points):
        unmatched_records = {
            record['id']
            for record in self._person_records
            if record.get('status') != 'rescued'
        }

        for point in points:
            person_id = 0
            if isinstance(point, dict):
                try:
                    lat = float(point['lat'])
                    lon = float(point['lon'])
                    person_id = int(point.get('id', 0) or 0)
                except Exception:
                    continue
            else:
                try:
                    lat = float(point[0])
                    lon = float(point[1])
                except Exception:
                    continue

            if person_id > 0:
                existing = next((record for record in self._person_records if int(record['id']) == person_id), None)
                if existing is not None:
                    existing['lat'] = lat
                    existing['lon'] = lon
                    existing.setdefault('assigned_robot', '')
                    existing.setdefault('status_source', 'auto')
                    continue

            best_record = None
            best_distance = None
            for record in self._person_records:
                if record.get('status') == 'rescued':
                    continue
                if record['id'] not in unmatched_records:
                    continue
                distance = _haversine_m(lat, lon, record['lat'], record['lon'])
                if distance > self._person_match_threshold_m:
                    continue
                if best_distance is None or distance < best_distance:
                    best_distance = distance
                    best_record = record

            if best_record is not None:
                best_record['lat'] = lat
                best_record['lon'] = lon
                best_record.setdefault('assigned_robot', '')
                best_record.setdefault('status_source', 'auto')
                unmatched_records.discard(best_record['id'])
                continue

            self._person_records.append({
                'id': person_id if person_id > 0 else self._next_person_id,
                'lat': lat,
                'lon': lon,
                'status': 'detected',
                'status_source': 'auto',
                'assigned_robot': '',
                'distance_m': None,
            })
            if person_id > 0:
                self._next_person_id = max(self._next_person_id, person_id + 1)
            else:
                self._next_person_id += 1

        self._person_records.sort(key=lambda record: int(record['id']))

    def _refresh_person_records_view(self):
        if not self._persons_refresh_timer.isActive():
            self._persons_refresh_timer.start(120)

    def _apply_refresh_person_records_view(self):
        self.persons_tab.set_records(self._person_records)
        points = []
        for record in self._person_records:
            if record.get('status') == 'rescued':
                continue
            points.append({
                'id': int(record['id']),
                'lat': float(record['lat']),
                'lon': float(record['lon']),
                'status': str(record.get('status', 'detected')),
                'assigned_robot': str(record.get('assigned_robot', '') or ''),
                'distance_m': record.get('distance_m', None),
            })
        self._js_set_detected_persons(points)

    def _auto_update_person_statuses(self, robot_lat: float, robot_lon: float):
        changed = False
        for record in self._person_records:
            status = str(record.get('status', 'detected'))
            try:
                distance = _haversine_m(robot_lat, robot_lon, record['lat'], record['lon'])
            except Exception:
                continue

            previous_distance = record.get('distance_m', None)
            record['distance_m'] = float(distance)
            if previous_distance is None or abs(float(previous_distance) - float(distance)) >= 1.0:
                changed = True

            if status == 'rescued':
                continue
            if not self._auto_person_tracking_enabled:
                continue

            if distance <= self._person_rescued_distance_m:
                if status != 'rescued':
                    record['status'] = 'rescued'
                    record['status_source'] = 'auto'
                    if not record.get('assigned_robot'):
                        record['assigned_robot'] = self._current_namespace
                    changed = True
            elif distance <= self._person_pending_distance_m and str(record.get('status_source', 'auto')) != 'manual':
                if status == 'detected':
                    record['status'] = 'pending'
                    record['status_source'] = 'auto'
                    if not record.get('assigned_robot'):
                        record['assigned_robot'] = self._current_namespace
                    changed = True

        if changed:
            self._refresh_person_records_view()

    def _on_person_status_change(self, person_id: int, status: str):
        status_value = str(status or '').strip().lower()
        if status_value not in {'detected', 'pending', 'rescued'}:
            return
        for record in self._person_records:
            if int(record['id']) == int(person_id):
                record['status'] = status_value
                record['status_source'] = 'manual'
                if status_value in {'pending', 'rescued'} and not record.get('assigned_robot'):
                    record['assigned_robot'] = self._current_namespace
                break
        self._refresh_person_records_view()

    def _on_person_assignment_change(self, person_id: int, robot_name: str):
        robot_value = str(robot_name or '').strip()
        for record in self._person_records:
            if int(record['id']) == int(person_id):
                record['assigned_robot'] = robot_value
                break
        self._refresh_person_records_view()

    def _on_auto_person_tracking_changed(self, enabled: bool):
        self._auto_person_tracking_enabled = bool(enabled)
        lat, lon, _ = self._last_vehicle
        if self._have_vehicle_gps:
            self._auto_update_person_statuses(lat, lon)
        self._refresh_person_records_view()

    def _on_person_thresholds_changed(self, pending_m: float, rescued_m: float):
        self._person_pending_distance_m = max(float(pending_m), 1.0)
        self._person_rescued_distance_m = max(min(float(rescued_m), self._person_pending_distance_m), 1.0)
        self.persons_tab.set_thresholds(self._person_pending_distance_m, self._person_rescued_distance_m)
        lat, lon, _ = self._last_vehicle
        if self._have_vehicle_gps:
            self._auto_update_person_statuses(lat, lon)
        self._refresh_person_records_view()

    def _js_set_detected_persons(self, points):
        safe_points = []
        for point in points:
            if isinstance(point, dict):
                try:
                    safe_points.append({
                        'id': int(point.get('id', 0)),
                        'lat': float(point['lat']),
                        'lon': float(point['lon']),
                        'status': str(point.get('status', 'detected')),
                        'assigned_robot': str(point.get('assigned_robot', '') or ''),
                        'distance_m': point.get('distance_m', None),
                    })
                except Exception:
                    continue
                continue

            try:
                lat = float(point[0])
                lon = float(point[1])
            except Exception:
                continue
            safe_points.append({'id': 0, 'lat': lat, 'lon': lon, 'status': 'detected'})
        self._js_call(f'window.setDetectedPersons({json.dumps(safe_points)});')

    @staticmethod
    def _classify_js_call(js: str):
        if js.startswith('window.setMission('):
            return 'mission'
        if js.startswith('window.setDetectedPersons('):
            return 'persons'
        if js.startswith('window.updateVehicle('):
            return 'vehicle'
        if js.startswith('window.panTo('):
            return 'pan'
        return None

    def _flush_js_queue(self):
        page = self.web.page() if hasattr(self, 'web') else None
        if page is None:
            self._pending_js_by_key.clear()
            self._pending_js_generic.clear()
            return

        ordered_keys = ['mission', 'persons', 'vehicle', 'pan']
        for key in ordered_keys:
            js = self._pending_js_by_key.pop(key, None)
            if js:
                page.runJavaScript(js)

        generic_calls = self._pending_js_generic[:8]
        self._pending_js_generic.clear()
        for js in generic_calls:
            page.runJavaScript(js)

    # ===== ROS =====
    def start_ros(self):
        try:
            if not rclpy.ok():
                rclpy.init(args=None)
            if self._exec is None:
                self._exec = rclpy.executors.SingleThreadedExecutor()
                self._th = threading.Thread(target=self._spin_executor, daemon=True)
                self._th.start()
            self._connect_robot_namespace(self._current_namespace)
            if not self._namespace_refresh_timer.isActive():
                self._namespace_refresh_timer.start()
            if not hasattr(self, 'follow_tab'):
                self._build_tab_follow()
        except Exception as e:
            print(f"[GUI] no se pudo iniciar ROS: {e}")

    def _spin_executor(self):
        while rclpy.ok():
            try:
                self._exec.spin_once(timeout_sec=0.25)
            except RuntimeError as e:
                print(f"[GUI] executor runtime error: {e}")
                time.sleep(0.1)
            except Exception as e:
                print(f"[GUI] executor error: {e}")
                time.sleep(0.25)

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
        page = self.web.page() if hasattr(self, 'web') else None
        if page is None:
            return

        key = self._classify_js_call(js)
        if key is None:
            self._pending_js_generic.append(js)
            if len(self._pending_js_generic) > 8:
                self._pending_js_generic = self._pending_js_generic[-8:]
        else:
            self._pending_js_by_key[key] = js

        if not self._js_flush_timer.isActive():
            self._js_flush_timer.start(33)

    def _js_set_mission(self):
        js_array = json.dumps(self._planned)
        self._js_call(f'window.setMission({js_array});')
