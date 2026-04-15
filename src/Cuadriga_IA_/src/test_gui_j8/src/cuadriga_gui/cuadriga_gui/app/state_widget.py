# cuadriga_gui/app/state_widget.py
import math
import json
import os
import re
import time
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from typing import Optional, Dict, Callable, List, Tuple

from ament_index_python.packages import get_package_share_directory

from PySide6.QtCore import QObject, Signal, Qt, QPointF, QTimer
from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton,
    QLineEdit, QFormLayout, QTableWidget, QTableWidgetItem,
    QComboBox, QSplitter, QFileDialog, QSpinBox, QDoubleSpinBox
)
from PySide6.QtWidgets import QAbstractItemView

import rclpy
from rclpy.node import Node

# Mensajes ROS
from sensor_msgs.msg import Imu, NavSatFix
from nav_msgs.msg import Odometry, Path
from geometry_msgs.msg import PoseStamped, PoseArray
from geometry_msgs.msg import Vector3Stamped

from cuadriga_gui.app.geo_local import ll2xy


# ===================== Utils =====================
def quat_to_euler(qx, qy, qz, qw):
    """(roll, pitch, yaw) en radianes desde cuaternión (ZYX)."""
    sinr_cosp = 2.0 * (qw * qx + qy * qz)
    cosr_cosp = 1.0 - 2.0 * (qx * qx + qy * qy)
    roll = math.atan2(sinr_cosp, cosr_cosp)

    sinp = 2.0 * (qw * qy - qz * qx)
    pitch = math.copysign(math.pi / 2, sinp) if abs(sinp) >= 1 else math.asin(sinp)

    siny_cosp = 2.0 * (qw * qz + qx * qy)
    cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz)
    yaw = math.atan2(siny_cosp, cosy_cosp)
    return roll, pitch, yaw

def haversine_m(lat1, lon1, lat2, lon2):
    """Distancia en metros entre dos puntos WGS84."""
    R = 6371000.0
    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)
    a = (math.sin(dlat/2)**2
         + math.cos(math.radians(lat1)) * math.cos(math.radians(lat2)) * math.sin(dlon/2)**2)
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c


# ===================== HUD + Stats (Widgets internos) =====================
from PySide6.QtGui import QPainter, QColor, QPen, QFont
from PySide6.QtWidgets import QSizePolicy, QGridLayout

class StatTile(QWidget):
    def __init__(self, title: str, unit: str = "", color: QColor = QColor("#66E")):
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
        # Ocultar el campo de unidad si está vacío para no dejar una franja "muerta".
        if unit:
            box.addWidget(self.unit)
        else:
            self.unit.hide()

        self.setStyleSheet("background:#111; border-radius:10px;")

    def set_value(self, text: str):
        self.value.setText(text)

    def set_placeholder(self, text: str = "—"):
        self.value.setText(text)

class StatsPanel(QWidget):
    """Cuadrícula 3x2: Altitude, GroundSpeed, Dist to WP, Yaw, Vertical Speed, DistToMAV"""
    def __init__(self):
        super().__init__()
        self._grid = QGridLayout(self)
        self._grid.setContentsMargins(0, 0, 0, 0)
        self._grid.setSpacing(8)

        self._tiles: List[StatTile] = []
        self._key_by_tile: List[Optional[str]] = []
        self._last_values: Dict[str, float] = {}

        # Default group
        self.set_layout_group([
            ("Altitude (m)", "altitude", QColor("#E88")),
            ("Speed Odom (m/s)", "groundspeed", QColor("#6AE")),
            ("Speed GPS (m/s)", "gps_speed", QColor("#6AE")),
            ("Heading (deg)", "yaw", QColor("#6E6")),
            ("CTE (m)", "cte", QColor("#EE6")),
            ("LookAhead (m)", "lookahead", QColor("#EE6")),
        ])

        self.setStyleSheet("background:#0A0A0A; border:1px solid #222; border-radius:12px; padding:8px;")

    def set_layout_group(self, specs: List[Tuple[str, Optional[str], QColor]]):
        """Configura el panel 3x2 con un conjunto de métricas.

        specs: [(title, key, color)]
          - key: nombre de métrica que llega por update_stats(**kw)
        """
        # limpiar layout
        while self._grid.count():
            item = self._grid.takeAt(0)
            w = item.widget()
            if w is not None:
                w.setParent(None)

        self._tiles = []
        self._key_by_tile = []

        for i, (title, key, color) in enumerate(specs[:6]):
            tile = StatTile(title, "", color)
            self._tiles.append(tile)
            self._key_by_tile.append(key)
            r, c = divmod(i, 3)
            self._grid.addWidget(tile, r, c)

            if not key:
                tile.set_placeholder("—")

        # Rellenar valores antiguos si existen
        for tile, key in zip(self._tiles, self._key_by_tile):
            if key and key in self._last_values:
                tile.set_value(f"{self._last_values[key]:.2f}")

    def _set_value(self, key: str, value: float):
        self._last_values[key] = float(value)
        # actualizar tile visible si corresponde
        for tile, k in zip(self._tiles, self._key_by_tile):
            if k == key:
                tile.set_value(f"{float(value):.2f}")
                break

    def update_stats(self, **kw):
        for k, v in (kw or {}).items():
            try:
                if v is None:
                    continue
                self._set_value(str(k), float(v))
            except Exception:
                continue


class MetricSelector(QWidget):
    """Barra compacta para seleccionar qué métricas se quieren ver."""
    group_changed = Signal(str)

    def __init__(self, groups: List[Tuple[str, str]]):
        super().__init__()
        self.setStyleSheet(
            "background:#0A0A0A; border:1px solid #222; border-radius:12px; padding:6px;"
        )
        lay = QHBoxLayout(self)
        lay.setContentsMargins(8, 6, 8, 6)
        lay.setSpacing(8)

        lbl = QLabel("Métricas:")
        lbl.setStyleSheet("color:#DDD; font-weight:700;")
        lay.addWidget(lbl)
        self.cmb = QComboBox()
        # Estilo neutro (similar al combo de "Cambio de estado") + hover visible.
        self.cmb.setStyleSheet(
            "QComboBox {"
            "  color: #EEE;"
            "  background-color: #0F0F0F;"
            "  border: 1px solid #2A2A2A;"
            "  border-radius: 8px;"
            "  padding: 6px 10px;"
            "}"
            "QComboBox::drop-down { border: none; width: 26px; }"
            "QComboBox QAbstractItemView {"
            "  color: #EEE;"
            "  background-color: #0F0F0F;"
            "  border: 1px solid #2A2A2A;"
            "  outline: 0;"
            "}"
            "QComboBox QAbstractItemView::item { padding: 6px 10px; }"
            "QComboBox QAbstractItemView::item:selected { background: #2F2F2F; color: #FFF; }"
            "QComboBox QAbstractItemView::item:hover { background: #3A3A3A; color: #FFF; }"
        )
        for label, key in groups:
            self.cmb.addItem(label, userData=key)
        self.cmb.currentIndexChanged.connect(self._emit)
        lay.addWidget(self.cmb, 1)

    def _emit(self):
        key = self.cmb.currentData()
        if isinstance(key, str) and key:
            self.group_changed.emit(key)

class HUDWidget(QWidget):
    """
    HUD sencillo: cielo/tierra, horizonte con pitch/roll, mira central, heading y cartela GPS/estado.
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMinimumHeight(200)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)

        # Estado
        self.pitch_deg = 0.0
        self.roll_deg  = 0.0
        self.yaw_deg   = 0.0

        # Estado (texto y color)
        self.state_text = "DISARMED"
        self.state_color_override = QColor("#ff4444")
        self.state_colors = {
            "DISARMED": QColor(255, 80, 80),
            "READY": QColor(80, 200, 120),
            "STANDBY": QColor(255, 190, 60),
            "GUIDED": QColor(120, 190, 255),
            "FOLLOWPATH": QColor(120, 220, 255),
            "FOLLOWZED": QColor(140, 220, 180),
            "CONTROLIA": QColor(180, 160, 255),
        }
        # GPS
        self.gps_sats = None
        self.gps_hdop = None
        self.gps_label = "GPS: —"
        self.gps_color = QColor(255, 80, 80)

        # Apariencia
        self._sky    = QColor(110, 170, 255)
        self._ground = QColor(170, 140, 100)
        self._lines  = QColor(255, 255, 255)

        # Para robot de tierra: por defecto no mostramos un horizonte "avión".
        # Mantenemos la estética pero reducimos el efecto cielo/tierra.
        self.show_horizon = False

    def set_state(self, text: str):
        t = (text or "").strip()
        self.state_text = t if t else "DISARMED"
        self.update()

    def set_state_color(self, color):
        if color is None:
            self.state_color_override = None
        else:
            self.state_color_override = QColor(color) if not isinstance(color, QColor) else color
        self.update()

    def set_gps_status(self, fix=None, sats=None, hdop=None, label: str=None):
        if sats is not None: self.gps_sats = int(sats)
        if hdop is not None: self.gps_hdop = float(hdop)

        text, color = None, QColor(200, 200, 200)
        if isinstance(fix, int):
            # ROS NavSatStatus.status uses:
            #   -1 NO_FIX, 0 FIX, 1 SBAS, 2 GBAS
            # Some autopilot/MAVLink stacks use a different "fix_type" (0..6).
            # NOTE: Both scales overlap at 0, but mean different things:
            #   ROS 0 => FIX (good)
            #   APM 0 => No GPS
            # In this GUI, `fix` comes from sensor_msgs/NavSatFix.status.status,
            # so we must prefer the ROS interpretation.
            ros_map = {
                -1: ("No GPS", QColor(255, 80, 80)),
                0: ("3D Fix", QColor( 80, 220, 120)),
                1: ("SBAS",   QColor( 80, 200, 240)),
                2: ("GBAS",   QColor( 80, 200, 240)),
            }
            apm_map = {
                0: ("No GPS",     QColor(255, 80, 80)),
                1: ("No Fix",     QColor(255, 80, 80)),
                2: ("2D Fix",     QColor(255, 190, 60)),
                3: ("3D Fix",     QColor( 80, 220, 120)),
                4: ("DGPS",       QColor( 80, 200, 240)),
                5: ("RTK Float",  QColor(160, 120, 255)),
                6: ("RTK Fixed",  QColor(140,  90, 255)),
            }
            if fix in ros_map:
                text, color = ros_map[fix]
            elif fix in apm_map:
                text, color = apm_map[fix]
            else:
                text, color = (f"Fix {fix}", QColor(200, 200, 200))
        elif isinstance(fix, str):
            text = fix
            color = QColor(255, 80, 80) if "NO" in fix.upper() else QColor(80, 220, 120)

        if label is not None:
            text = label

        parts = [f"GPS: {text or '—'}"]
        if self.gps_sats is not None: parts.append(f"Sats: {self.gps_sats}")
        if self.gps_hdop is not None: parts.append(f"HDOP: {self.gps_hdop:.1f}")
        self.gps_label = "  ".join(parts)
        self.gps_color = color
        self.update()

    def set_attitude(self, pitch: float=None, roll: float=None, yaw: float=None):
        if pitch is not None: self.pitch_deg = float(pitch)
        if roll  is not None: self.roll_deg  = float(roll)
        if yaw   is not None: self.yaw_deg   = float(yaw) % 360.0
        self.update()

    def paintEvent(self, ev):
        w = self.width()
        h = self.height()
        cx, cy = w / 2.0, h / 2.0

        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing, True)

        # Fondo + horizonte (opcional). Para el cuadriga (tierra) lo desactivamos por defecto.
        p.setPen(Qt.NoPen)
        p.setBrush(QColor(10, 10, 10))
        p.drawRect(0, 0, w, h)

        if self.show_horizon:
            p.save()
            p.translate(cx, cy)
            p.rotate(-self.roll_deg)

            px_per_deg = h / 120.0
            dy = self.pitch_deg * px_per_deg

            p.setBrush(self._sky)
            p.drawRect(-w, -h - dy, 2 * w, h + dy)
            p.setBrush(self._ground)
            p.drawRect(-w, -dy, 2 * w, h + dy)

            p.setPen(QPen(self._lines, 2))
            p.drawLine(-w, -dy, w, -dy)

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

        # Anillo de heading (útil para robot de tierra)
        p.save()
        radius = min(w, h) * 0.33
        p.setPen(QPen(QColor(255, 255, 255, 90), 2))
        p.setBrush(Qt.NoBrush)
        p.drawEllipse(int(cx - radius), int(cy - radius), int(2 * radius), int(2 * radius))

        # Aguja hacia el heading actual
        p.translate(cx, cy)
        p.rotate(self.yaw_deg)
        p.setPen(QPen(QColor(120, 190, 255), 3))
        p.drawLine(0, 0, 0, int(-radius))
        # Punta de flecha (triángulo) para indicar sentido
        p.setBrush(QColor(120, 190, 255))
        tip_y = int(-radius)
        arrow = [
            QPointF(0, tip_y),
            QPointF(-8, tip_y + 14),
            QPointF(8, tip_y + 14),
        ]
        p.drawPolygon(arrow)
        p.restore()

        # Mira central
        p.setPen(QPen(self._lines, 2))
        p.drawLine(cx - 18, cy, cx + 18, cy)
        p.drawLine(cx, cy - 12, cx, cy + 12)

        # Heading
        p.setPen(QPen(self._lines, 1))
        f = QFont(self.font())
        f.setPointSizeF(f.pointSizeF() * 0.9)
        p.setFont(f)
        p.drawText(8, 18, f"HDG {self.yaw_deg:06.2f}°")

        # FSM / estado (arriba izquierda, cerca de HDG)
        if getattr(self, "state_text", ""):
            txt = str(self.state_text)
            key = txt.upper().replace("_", "").replace(" ", "")
            col = self.state_color_override or self.state_colors.get(key, QColor(255, 255, 255))

            f2 = QFont(self.font()); f2.setBold(True)
            f2.setPointSizeF(f2.pointSizeF() * 1.1)
            p.setFont(f2)
            p.setPen(QPen(col))
            p.drawText(8, 38, txt)

        # Cartela GPS
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

            rx, ry = 12, h - rect_h - 10

            p.setPen(Qt.NoPen)
            p.setBrush(QColor(0, 0, 0, 140))
            p.drawRoundedRect(rx, ry, rect_w, rect_h, 8, 8)

            p.setPen(QPen(self.gps_color))
            p.drawText(rx + pad_x, ry + pad_y + metrics.ascent(), txt)


# ===================== Señales y Nodo ROS =====================
class StateSignals(QObject):
    attitude = Signal(float, float, float)   # pitch_deg, roll_deg, yaw_deg
    gps      = Signal(object)                # dict {'fix':int|str|None,'sats':int|None,'hdop':float|None}
    stats    = Signal(object)                # dict para StatsPanel.update_stats(**d)
    state_text = Signal(str)
    # GPS pose for other widgets (e.g. the embedded Leaflet map in main_window).
    # Emits: (lat_deg: float, lon_deg: float, heading_deg: float|None)
    gps_pose = Signal(float, float, object)
    detected_persons = Signal(object)
    # FSM feedback (legacy GUI_pkg contract)
    fsm_mode = Signal(int)
    possible_transitions = Signal(object)  # List[int]

@dataclass
class TopicMap:
    """Mapa de topics para telemetría.

    Defaults alineados con `GUI_pkg` para ser reemplazo directo.
    """

    # GUI_pkg defaults
    imu: str = '/imu_head/data'
    odom: str = '/fixposition/odometry_enu'
    fix: str = '/fixposition/navsatfix'
    gps_vel: Optional[str] = '/fixposition/navsatfix/vel'

    # Topic interno del prototipo (no existía en GUI_pkg). Se mantiene para
    # compatibilidad entre widgets.
    # NOTE: legacy GUI_pkg had no guided-target topic; we keep this empty to avoid /gui/* defaults.
    guided: str = ''
    gps_hdop: Optional[str] = None
    gps_sats: Optional[str] = None

    # --- FSM topics (legacy GUI_pkg) ---
    # Defaults are namespaced as in the legacy GUI: /cuadriga/<name>
    fsm_mode: str = '/cuadriga/fsm_mode'
    possible_transitions: str = '/cuadriga/possible_transitions'

    # --- Path-following metrics (cuadriga) ---
    cte: Optional[str] = '/cuadriga/cte'
    look_ahead_distance: Optional[str] = '/cuadriga/look_ahead_distance'
    min_distance_to_path: Optional[str] = '/cuadriga/min_distance_to_path'
    detected_persons_latlon: Optional[str] = '/cuadriga/detected_persons_latlon'
    detected_persons_global_latlon: Optional[str] = '/detected_persons_latlon_global'

class TelemetryNode(Node):
    def __init__(self, signals: StateSignals, topics: TopicMap):
        super().__init__('cuadriga_state_node')
        self._sig = signals
        self._topics = topics

        self._have_fix = False
        self._lat = self._lon = self._alt = None
        self._guided_lat = self._guided_lon = None

        self._cached_hdop = None
        self._cached_sats = None
        self._cached_gps_speed = None

        self.create_subscription(Imu, topics.imu, self._on_imu, 10)
        self.create_subscription(Odometry, topics.odom, self._on_odom, 10)
        self.create_subscription(NavSatFix, topics.fix, self._on_fix, 10)

        if topics.gps_vel:
            self.create_subscription(Vector3Stamped, topics.gps_vel, self._on_gps_vel, 10)
        # This topic is optional. Legacy GUI_pkg had no guided-target topic and
        # our default is '' (disabled) to avoid /gui/*.
        if topics.guided:
            self.create_subscription(PoseStamped, topics.guided, self._on_guided, 10)

        # FSM feedback (legacy GUI_pkg)
        from std_msgs.msg import Int32 as MsgInt32
        from std_msgs.msg import Int32MultiArray
        if topics.fsm_mode:
            self.create_subscription(MsgInt32, topics.fsm_mode, self._on_fsm_mode, 10)
        if topics.possible_transitions:
            self.create_subscription(Int32MultiArray, topics.possible_transitions, self._on_possible_transitions, 10)

        if topics.gps_hdop:
            from std_msgs.msg import Float32
            self.create_subscription(Float32, topics.gps_hdop, self._on_hdop, 10)
        if topics.gps_sats:
            from std_msgs.msg import Int32
            self.create_subscription(Int32, topics.gps_sats, self._on_sats, 10)

        # Metrics for ground navigation
        from std_msgs.msg import Float32
        if topics.cte:
            self.create_subscription(Float32, topics.cte, self._on_cte, 10)
        if topics.look_ahead_distance:
            self.create_subscription(Float32, topics.look_ahead_distance, self._on_lookahead, 10)
        if topics.min_distance_to_path:
            self.create_subscription(Float32, topics.min_distance_to_path, self._on_min_dist_path, 10)
        if topics.detected_persons_latlon:
            self.create_subscription(PoseArray, topics.detected_persons_latlon, self._on_detected_persons_latlon, 10)
        if topics.detected_persons_global_latlon:
            self.create_subscription(PoseArray, topics.detected_persons_global_latlon, self._on_detected_persons_latlon, 10)

    def _on_imu(self, msg: Imu):
        q = msg.orientation
        roll, pitch, yaw = quat_to_euler(q.x, q.y, q.z, q.w)
        yaw_deg_enu = (math.degrees(yaw) % 360.0)
        # Compass-like bearing expected by the GUI/map: 0° = North, clockwise positive.
        # ROS yaw in ENU: 0° = East, CCW positive.
        bearing_deg = (90.0 - yaw_deg_enu) % 360.0
        self._sig.attitude.emit(math.degrees(pitch), math.degrees(roll), bearing_deg)

        # If we already have a valid GPS fix, also publish the pose for the map.
        # Heading comes from IMU in degrees [0,360).
        if self._have_fix and self._lat is not None and self._lon is not None:
            self._sig.gps_pose.emit(self._lat, self._lon, bearing_deg)

    def _on_odom(self, msg: Odometry):
        vx = msg.twist.twist.linear.x
        vy = msg.twist.twist.linear.y
        vz = msg.twist.twist.linear.z
        yaw_rate = float(msg.twist.twist.angular.z)
        gspeed = math.hypot(vx, vy)
        vspeed = float(vz)

        # Heading desde odometría (útil en tierra y en simulación)
        try:
            q = msg.pose.pose.orientation
            roll, pitch, yaw = quat_to_euler(q.x, q.y, q.z, q.w)
            yaw_deg_enu = (math.degrees(yaw) % 360.0)
            bearing_deg = (90.0 - yaw_deg_enu) % 360.0
            self._sig.attitude.emit(math.degrees(pitch), math.degrees(roll), bearing_deg)

            # Also feed the GPS pose signal if we already have lat/lon.
            if self._have_fix and self._lat is not None and self._lon is not None:
                self._sig.gps_pose.emit(self._lat, self._lon, bearing_deg)
        except Exception:
            pass

        alt = None
        try:
            alt = float(msg.pose.pose.position.z)
        except Exception:
            pass

        dist_wp = None
        if self._have_fix and self._guided_lat is not None and self._guided_lon is not None:
            dist_wp = haversine_m(self._lat, self._lon, self._guided_lat, self._guided_lon)

        payload = {'groundspeed': gspeed, 'vspeed': vspeed, 'yaw_rate': yaw_rate}
        if alt is not None and self._alt is None:
            payload['altitude'] = alt
        if dist_wp is not None:
            payload['dist_wp'] = dist_wp
            payload['dist_mav'] = dist_wp

        self._sig.stats.emit(payload)

    def _on_fix(self, msg: NavSatFix):
        self._have_fix = (msg.status.status is not None) and (msg.status.status >= 0)
        self._lat = float(msg.latitude)
        self._lon = float(msg.longitude)
        self._alt = float(msg.altitude)

        self._sig.stats.emit({'altitude': self._alt})
        self._sig.gps.emit({'fix': int(msg.status.status), 'sats': self._cached_sats, 'hdop': self._cached_hdop})

        # Notify current GPS pose (heading may have arrived via IMU/Odom already).
        # We do not force a heading here to avoid lying; consumers may choose a default.
        self._sig.gps_pose.emit(self._lat, self._lon, None)

        if self._guided_lat is not None and self._guided_lon is not None:
            dist_wp = haversine_m(self._lat, self._lon, self._guided_lat, self._guided_lon)
            self._sig.stats.emit({'dist_wp': dist_wp, 'dist_mav': dist_wp})

    def _on_guided(self, msg: PoseStamped):
        self._guided_lon = float(msg.pose.position.x)
        self._guided_lat = float(msg.pose.position.y)
        if self._have_fix and self._lat is not None and self._lon is not None:
            dist_wp = haversine_m(self._lat, self._lon, self._guided_lat, self._guided_lon)
            self._sig.stats.emit({'dist_wp': dist_wp, 'dist_mav': dist_wp})

    def _on_hdop(self, msg):
        try:
            self._cached_hdop = float(msg.data)
            self._sig.gps.emit({'fix': None, 'sats': self._cached_sats, 'hdop': self._cached_hdop})
        except Exception:
            pass

    def _on_sats(self, msg):
        try:
            self._cached_sats = int(msg.data)
            self._sig.gps.emit({'fix': None, 'sats': self._cached_sats, 'hdop': self._cached_hdop})
        except Exception:
            pass

    def _on_gps_vel(self, msg: Vector3Stamped):
        try:
            vx = float(msg.vector.x)
            vy = float(msg.vector.y)
            vz = float(msg.vector.z)
            self._cached_gps_speed = math.sqrt(vx * vx + vy * vy + vz * vz)
            self._sig.stats.emit({'gps_speed': self._cached_gps_speed})
        except Exception:
            pass

    def _on_cte(self, msg):
        try:
            self._sig.stats.emit({'cte': float(msg.data)})
        except Exception:
            pass

    def _on_lookahead(self, msg):
        try:
            self._sig.stats.emit({'lookahead': float(msg.data)})
        except Exception:
            pass

    def _on_min_dist_path(self, msg):
        try:
            # Reaprovechamos el HUD/tiles existentes; si luego queremos mostrarlo,
            # añadimos un tile dedicado.
            self._sig.stats.emit({'min_dist_path': float(msg.data)})
        except Exception:
            pass

    def _on_fsm_mode(self, msg):
        try:
            self._sig.fsm_mode.emit(int(msg.data))
        except Exception:
            pass

    def _on_possible_transitions(self, msg):
        try:
            arr = msg.data
            try:
                # sometimes it's array('i'), sometimes list
                lst = list(arr)
            except Exception:
                lst = []
            self._sig.possible_transitions.emit(lst)
        except Exception:
            pass

    def _on_detected_persons_latlon(self, msg: PoseArray):
        points = []
        try:
            for pose in msg.poses:
                person_id = 0
                try:
                    person_id = int(round(float(pose.orientation.w)))
                except Exception:
                    person_id = 0

                points.append({
                    'id': person_id,
                    'lat': float(pose.position.x),
                    'lon': float(pose.position.y),
                })
        except Exception:
            return
        self._sig.detected_persons.emit(points)


# ===================== StateWidget (HUD + Stats) =====================
class StateWidget(QWidget):
    """
    Componente con:
      - HUDWidget (actitud, heading, estado, GPS)
      - StatsPanel (altitud, velocidad, distancias)
    Exposición:
      - set_state(text)
      - attach_ros(executor, topics=None)
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.hud = HUDWidget()
        self.metric_selector = MetricSelector([
            ("Navegación", "nav"),
            ("Seguimiento de Path", "path"),
            ("GPS", "gps"),
            ("Debug", "debug"),
        ])
        self.stats = StatsPanel()

        lay = QVBoxLayout(self)
        lay.setContentsMargins(0,0,0,0); lay.setSpacing(8)
        lay.addWidget(self.hud)
        lay.addWidget(self.metric_selector)
        lay.addWidget(self.stats)

        self._signals = StateSignals()
        self._signals.attitude.connect(self._on_attitude)
        self._signals.gps.connect(self._on_gps)
        self._signals.stats.connect(self._on_stats)
        self._signals.state_text.connect(self.hud.set_state)

        # Cambiar qué métricas se ven (sin parar actualizaciones)
        self.metric_selector.group_changed.connect(self._on_metric_group)

        self._node = None  # se crea en attach_ros

        # Default
        self._on_metric_group("nav")

    def _on_metric_group(self, key: str):
        """Cambia dinámicamente el set de tiles visibles."""
        key = (key or "").strip().lower()
        if key == "gps":
            specs = [
                ("GPS Speed (m/s)", "gps_speed", QColor("#6AE")),
                ("Altitude (m)", "altitude", QColor("#E88")),
                ("Heading (deg)", "yaw", QColor("#6E6")),
                ("Yaw Rate (rad/s)", "yaw_rate", QColor("#6AE")),
                ("CTE (m)", "cte", QColor("#EE6")),
                ("MinDistPath (m)", "min_dist_path", QColor("#EE6")),
            ]
        elif key == "path":
            specs = [
                ("CTE (m)", "cte", QColor("#EE6")),
                ("LookAhead (m)", "lookahead", QColor("#EE6")),
                ("MinDistPath (m)", "min_dist_path", QColor("#EE6")),
                ("Speed Odom (m/s)", "groundspeed", QColor("#6AE")),
                ("Yaw Rate (rad/s)", "yaw_rate", QColor("#6AE")),
                ("GPS Speed (m/s)", "gps_speed", QColor("#6AE")),
            ]
        elif key == "debug":
            specs = [
                ("Speed Odom (m/s)", "groundspeed", QColor("#6AE")),
                ("Yaw Rate (rad/s)", "yaw_rate", QColor("#6AE")),
                ("Altitude (m)", "altitude", QColor("#E88")),
                ("GPS Speed (m/s)", "gps_speed", QColor("#6AE")),
                ("Fix Status", None, QColor("#999")),
                ("Reserved", None, QColor("#999")),
            ]
        else:
            # nav
            specs = [
                ("Altitude (m)", "altitude", QColor("#E88")),
                ("Speed Odom (m/s)", "groundspeed", QColor("#6AE")),
                ("Speed GPS (m/s)", "gps_speed", QColor("#6AE")),
                ("Heading (deg)", "yaw", QColor("#6E6")),
                ("Yaw Rate (rad/s)", "yaw_rate", QColor("#6AE")),
                ("MinDistPath (m)", "min_dist_path", QColor("#EE6")),
            ]

        self.stats.set_layout_group(specs)

    def set_state(self, text: str):
        self.hud.set_state(text)

    def attach_ros(self, executor: rclpy.executors.Executor, topics: Dict[str, str] = None):
        self.detach_ros(executor)
        tmap = TopicMap()
        if topics:
            for k, v in topics.items():
                if hasattr(tmap, k) and isinstance(v, str):
                    setattr(tmap, k, v)
        self._node = TelemetryNode(self._signals, tmap)
        executor.add_node(self._node)

    def detach_ros(self, executor: rclpy.executors.Executor | None = None):
        if self._node is None:
            return
        if executor is not None:
            try:
                executor.remove_node(self._node)
            except Exception:
                pass
        try:
            self._node.destroy_node()
        except Exception:
            pass
        self._node = None

    # --- Slots internos ---
    def _on_attitude(self, pitch_deg: float, roll_deg: float, yaw_deg: float):
        self.hud.set_attitude(pitch=pitch_deg, roll=roll_deg, yaw=yaw_deg)
        self.stats.update_stats(yaw=yaw_deg)

    def _on_gps(self, d: dict):
        self.hud.set_gps_status(fix=d.get('fix', None), sats=d.get('sats', None), hdop=d.get('hdop', None))

    def _on_stats(self, d: dict):
        self.stats.update_stats(**d)


# ===================== MissionWidget (pestaña completa) =====================
# El GUI legacy trabaja con IDs de transición (ints). En modo dinámico,
# poblamos el combo desde /cuadriga/possible_transitions.

class MissionWidget(QWidget):
    """
    Pestaña Misión lista para usar:
      - Arriba: StateWidget (HUD + Stats) conectado a ROS con attach_ros(...)
      - Abajo: planificador (estado, tabla WPs, CSV, enviar path)
    Desde main_window:
      - set_ros(ros_side) para publicaciones
      - attach_ros(executor, topics=None) para suscripciones
      - set_js_call(callable_js) para pan/setMission en el mapa
      - on_guided_from_map / on_planned_from_map para MapBridge
    """
    def __init__(self, ros=None, js_call: Optional[Callable[[str], None]] = None):
        super().__init__()
        self._ros = ros
        self._js_call = js_call
        self._planned: List[Tuple[float,float]] = []

        # Latest robot GPS fix (origin for local waypoint coordinates)
        self._robot_lat: Optional[float] = None
        self._robot_lon: Optional[float] = None
        # Latest robot heading (compass bearing degrees: 0=N, clockwise)
        self._robot_heading_deg: Optional[float] = None

        # FSM feedback cache
        self._fsm_mode = None
        self._possible_transitions = []
        # Cache for UI stability: avoid rebuilding combo if nothing changed
        self._allowed_transition_ids: List[int] = []
        self._action_timestamps: Dict[str, float] = {}
        self._table_refresh_pending = False
        self._mission_sync_pending = False

        self._table_refresh_timer = QTimer(self)
        self._table_refresh_timer.setSingleShot(True)
        self._table_refresh_timer.timeout.connect(self._apply_table_refresh)

        self._mission_sync_timer = QTimer(self)
        self._mission_sync_timer.setSingleShot(True)
        self._mission_sync_timer.timeout.connect(self._apply_js_set_mission)

        # Arriba
        self.state = StateWidget()
        # Conectar señales FSM (si el nodo ROS está adjunto)
        self.state._signals.fsm_mode.connect(self._on_fsm_mode)
        self.state._signals.possible_transitions.connect(self._on_possible_transitions)
        # Cache robot GPS pose to use as origin for local conversion.
        self.state._signals.gps_pose.connect(self._on_robot_gps_pose)

        # Abajo (planificador)
        plan = QWidget(); lay = QVBoxLayout(plan)

        row = QHBoxLayout()
        self.cmb_state = QComboBox()

        # Inicialmente vacío: se rellenará al llegar possible_transitions.
        # Mientras tanto, dejamos un placeholder.
        self.cmb_state.addItem('Esperando FSM…', userData=None)
        self.btn_change_state = QPushButton('Cambiar estado')
        self.btn_change_state.clicked.connect(self._on_change_state)
        self.btn_change_state.setEnabled(False)
        row.addWidget(QLabel('Estado:')); row.addWidget(self.cmb_state); row.addWidget(self.btn_change_state)
        lay.addLayout(row)

        self.tbl = QTableWidget(0, 5)
        # Robot-frame local coordinates (base_link): X forward, Y left
        self.tbl.setHorizontalHeaderLabels(['#', 'Lat', 'Lon', 'X_fwd (m)', 'Y_left (m)'])
        self.tbl.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.tbl.setSelectionMode(QAbstractItemView.ExtendedSelection)  # multi-borrado
        self.tbl.cellDoubleClicked.connect(self._on_row_double_clicked) # opcional: doble click borra

        lay.addWidget(self.tbl)

        f = QFormLayout()
        self.ed_lat = QLineEdit(); self.ed_lon = QLineEdit()
        f.addRow('Lat:', self.ed_lat); f.addRow('Lon:', self.ed_lon)
        lay.addLayout(f)

        saved_row = QHBoxLayout()
        self.ed_saved_path_name = QLineEdit()
        self.ed_saved_path_name.setPlaceholderText('nombre_path')
        self.cmb_saved_paths = QComboBox()
        self.btn_refresh_paths = QPushButton('Refrescar')
        self.btn_refresh_paths.clicked.connect(self._refresh_saved_paths_combo)
        saved_row.addWidget(QLabel('Path guardado:'))
        saved_row.addWidget(self.ed_saved_path_name, 1)
        saved_row.addWidget(self.cmb_saved_paths, 1)
        saved_row.addWidget(self.btn_refresh_paths)
        lay.addLayout(saved_row)

        densify_row = QHBoxLayout()
        self.spn_intermediate_points = QSpinBox()
        self.spn_intermediate_points.setRange(1, 100)
        self.spn_intermediate_points.setValue(1)
        self.spn_intermediate_points.setToolTip('Numero de puntos a insertar entre cada pareja de waypoints')
        self.btn_insert_selected = QPushButton('Insertar entre seleccionados')
        self.btn_insert_selected.clicked.connect(self._on_insert_selected_intermediate_points)
        self.btn_insert_all = QPushButton('Densificar todo')
        self.btn_insert_all.clicked.connect(self._on_insert_all_intermediate_points)
        self.spn_spacing_m = QDoubleSpinBox()
        self.spn_spacing_m.setRange(0.5, 1000.0)
        self.spn_spacing_m.setDecimals(1)
        self.spn_spacing_m.setSingleStep(0.5)
        self.spn_spacing_m.setValue(5.0)
        self.spn_spacing_m.setToolTip('Separacion maxima objetivo entre waypoints')
        self.btn_densify_spacing = QPushButton('Densificar por distancia')
        self.btn_densify_spacing.clicked.connect(self._on_densify_by_spacing)
        densify_row.addWidget(QLabel('Pts intermedios:'))
        densify_row.addWidget(self.spn_intermediate_points)
        densify_row.addWidget(self.btn_insert_selected)
        densify_row.addWidget(self.btn_insert_all)
        densify_row.addWidget(QLabel('Espaciado max (m):'))
        densify_row.addWidget(self.spn_spacing_m)
        densify_row.addWidget(self.btn_densify_spacing)
        lay.addLayout(densify_row)

        exec_row = QHBoxLayout()
        self.btn_start_path = QPushButton('Ejecutar Path')
        self.btn_start_path.clicked.connect(self._on_start_path_following)
        self.btn_stop_path = QPushButton('Parar Path')
        self.btn_stop_path.clicked.connect(self._on_stop_path_following)
        self.btn_cancel_path = QPushButton('Cancelar Path')
        self.btn_cancel_path.clicked.connect(self._on_cancel_path_execution)
        exec_row.addWidget(self.btn_start_path)
        exec_row.addWidget(self.btn_stop_path)
        exec_row.addWidget(self.btn_cancel_path)
        lay.addLayout(exec_row)

        r2 = QHBoxLayout()
        b_add = QPushButton('Añadir WP');  b_add.clicked.connect(self._on_add_wp)
        b_pan = QPushButton('Centrar en mapa'); b_pan.clicked.connect(self._on_pan)
        b_send = QPushButton('Enviar Path'); b_send.clicked.connect(self._on_send_path)
        b_save = QPushButton('Guardar Path'); b_save.clicked.connect(self._on_save_path)
        b_clear= QPushButton('Limpiar');   b_clear.clicked.connect(self._on_clear)
        b_load = QPushButton('Cargar Path'); b_load.clicked.connect(self._on_load_saved_path)
        b_del = QPushButton('Borrar seleccionado(s)')
        for b in (b_add, b_pan, b_send, b_save, b_clear, b_load): r2.addWidget(b)
        b_del.clicked.connect(self._on_delete_selected)
        r2.addWidget(b_del)
        lay.addLayout(r2)

        lay.addWidget(QLabel('Consejo: Click simple = Guided. Shift+Click = añadir WP.'))

        spv = QSplitter(Qt.Vertical)
        spv.addWidget(self.state)
        spv.addWidget(plan)
        spv.setStretchFactor(0, 1)
        spv.setStretchFactor(1, 1)

        root = QVBoxLayout(self); root.setContentsMargins(8,8,8,8)
        root.addWidget(spv)

        # Estado inicial: sin transiciones aún
        self._apply_possible_transitions_to_combo()
        self._refresh_saved_paths_combo()

    # ---- API simple para main_window ----
    def set_ros(self, ros_side):
        self._ros = ros_side

    def attach_ros(self, executor, topics: dict = None):
        self.state.attach_ros(executor, topics)

    def detach_ros(self, executor=None):
        self.state.detach_ros(executor)

    def set_js_call(self, js_callable: Callable[[str], None]):
        self._js_call = js_callable

    # ---- Conexión con MapBridge ----
    def on_guided_from_map(self, lat: float, lon: float):
        if self._ros:
            self._ros.send_guided(lat, lon)

    @staticmethod
    def _fsm_transition_label(transition_id: int) -> str:
        """Devuelve un label humano para IDs de transición del FSM.

        Fuente: `src/ctl_mission/ctl_mission/include/ctl_mission/cuadriga_FSM.h`.

        Nota: `possible_transitions` publica IDs de *Transition*, no de *Mode*.
        """
        # Transition enum in cuadriga_FSM.h
        transition_labels = {
            0: 'Ready → PathFollowing',
            1: 'PathFollowing → Ready',
            2: 'Ready → Teleoperation',
            3: 'Teleoperation → Ready',
            4: 'Ready → GoingHome',
            5: 'GoingHome → Ready',
            6: 'Ready → RecordPath',
            7: 'RecordPath → Ready',
            8: 'E-Stop → Ready',
            9: 'All → E-Stop',
            10: 'Ready → FollowZED',
            11: 'FollowZED → Ready',
            12: 'Ready → External cmd_vel',
            13: 'External cmd_vel → Ready',
        }
        return transition_labels.get(int(transition_id), '')

    def on_planned_from_map(self, lat: float, lon: float):
        self._planned.append((lat, lon))
        self._refresh_table()
        self._js_set_mission()

    def _on_robot_gps_pose(self, lat: float, lon: float, heading_deg):
        # Called frequently (Fix/IMU/Odom). Keep it cheap.
        try:
            self._robot_lat = float(lat)
            self._robot_lon = float(lon)
        except Exception:
            return

        # heading_deg may be None (e.g., right after first GPS fix)
        if heading_deg is not None:
            try:
                self._robot_heading_deg = float(heading_deg) % 360.0
            except Exception:
                pass

        if self._planned:
            self._refresh_table()

    # ---- Handlers GUI ----
    def _on_change_state(self):
        label = self.cmb_state.currentText()
        if not self._ros:
            self.state.set_state(label)
            return
        key = self.cmb_state.currentData()
        # Nunca enviar transiciones inválidas (-1/None)
        if isinstance(key, int) and key >= 0:
            self._ros.send_state(key)
            self.state.set_state(label)
        else:
            # Nada válido seleccionado todavía
            return

    # ---- FSM feedback ----
    def _fsm_mode_label(self, mode: int) -> str:
        """Devuelve un nombre humano para el modo actual del FSM.

        Fuente: `src/ctl_mission/ctl_mission/include/ctl_mission/cuadriga_FSM.h`.

        Nota: estos IDs son parte del contrato ROS (se publican en `/fsm_mode`).
        """
        mode_labels = {
            0: 'Ready',
            1: 'PathFollowing',
            2: 'Teleoperation',
            3: 'GoingHome',
            4: 'EmergencyStop',
            5: 'RecordPath',
            6: 'FollowZED',
            7: 'External cmd_vel',
        }
        return mode_labels.get(int(mode), f"FSM {int(mode)}")

    def _on_fsm_mode(self, mode: int):
        self._fsm_mode = int(mode)
        # Mostrar el modo en el HUD con etiqueta humana.
        self.state.set_state(self._fsm_mode_label(self._fsm_mode))

    def _on_possible_transitions(self, transitions):
        # En ctl_mission, get_possible_transitions() devuelve un array de tamaño NumberOfModes
        # con -1 en no-permitidos y con el ID de Transition en el índice del modo objetivo.
        try:
            raw = [int(x) for x in (transitions or [])]
        except Exception:
            raw = []

        # Nos quedamos sólo con transiciones válidas (>=0)
        self._possible_transitions = [x for x in raw if isinstance(x, int) and x >= 0]
        self._apply_possible_transitions_to_combo()

    def _apply_possible_transitions_to_combo(self):
        """Puebla el combo de forma dinámica a partir de possible_transitions (IDs)."""
        allowed = sorted(set(int(x) for x in (self._possible_transitions or []) if isinstance(x, int) and x >= 0))

        # No tocar el combo si la lista no ha cambiado: evita resetear selección/foco.
        if allowed == self._allowed_transition_ids:
            if not allowed:
                self.btn_change_state.setEnabled(False)
            return

        # Preservar selección del usuario si sigue siendo válida
        prev_data = self.cmb_state.currentData()
        prev_ok = isinstance(prev_data, int) and prev_data >= 0

        was_popup = self.cmb_state.view().isVisible()

        self.cmb_state.blockSignals(True)
        self.cmb_state.clear()

        if not allowed:
            self.cmb_state.addItem('Sin transiciones (esperando FSM…) ', userData=None)
            self.btn_change_state.setEnabled(False)
        else:
            for tid in allowed:
                label = self._fsm_transition_label(int(tid))
                if label:
                    self.cmb_state.addItem(f'Transition {tid} ({label})', userData=int(tid))
                else:
                    self.cmb_state.addItem(f'Transition {tid}', userData=int(tid))

            # Restaurar selección si aplica; si no, dejar la primera.
            if prev_ok:
                idx = self.cmb_state.findData(int(prev_data))
                if idx >= 0 and not was_popup:
                    self.cmb_state.setCurrentIndex(idx)

            self.btn_change_state.setEnabled(True)

        self._allowed_transition_ids = allowed
        self.cmb_state.blockSignals(False)

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
        self._pan(lat, lon)

    def _on_send_path(self):
        if not self._allow_action('send_path', 0.35):
            return
        if not self._ros: return
        # Export also to the car_python circuit config (robot-frame local coords)
        try:
            self._export_goal_points_to_circuit_yaml(self._planned)
        except Exception as e:
            print(f"[cuadriga_gui] WARN: could not export circuit_1.yaml: {e}")

        # usa el helper del RosSide que ya tienes:
        self._ros.send_path(self._planned)

    @staticmethod
    def _get_saved_paths_dir() -> Optional[str]:
        try:
            share = get_package_share_directory('path_manager')
        except Exception:
            return None
        path_dir = os.path.join(share, 'paths')
        return path_dir if os.path.isdir(path_dir) else None

    def _refresh_saved_paths_combo(self):
        current = self.cmb_saved_paths.currentText().strip()
        self.cmb_saved_paths.blockSignals(True)
        self.cmb_saved_paths.clear()
        path_dir = self._get_saved_paths_dir()
        if path_dir:
            names = []
            for entry in sorted(os.listdir(path_dir)):
                if entry.lower().endswith('.gpx'):
                    names.append(os.path.splitext(entry)[0])
            self.cmb_saved_paths.addItems(names)
            if current:
                idx = self.cmb_saved_paths.findText(current)
                if idx >= 0:
                    self.cmb_saved_paths.setCurrentIndex(idx)
        self.cmb_saved_paths.blockSignals(False)

    def _parse_saved_path_file(self, path_name: str) -> List[Tuple[float, float]]:
        path_dir = self._get_saved_paths_dir()
        if not path_dir:
            return []
        file_path = os.path.join(path_dir, f'{path_name}.gpx')
        if not os.path.exists(file_path):
            return []

        try:
            root = ET.parse(file_path).getroot()
        except Exception:
            return []

        points: List[Tuple[float, float]] = []
        for elem in root.iter():
            tag = elem.tag.split('}')[-1]
            if tag != 'trkpt':
                continue
            try:
                lat = float(elem.attrib['lat'])
                lon = float(elem.attrib['lon'])
            except Exception:
                continue
            points.append((lat, lon))
        return points

    def _on_save_path(self):
        if not self._allow_action('save_path', 0.5):
            return
        name = self.ed_saved_path_name.text().strip() or self.cmb_saved_paths.currentText().strip()
        if not name or not self._planned or not self._ros:
            return

        self._ros.send_path(self._planned)
        self._ros.save_current_path(name)
        self._refresh_saved_paths_combo()
        idx = self.cmb_saved_paths.findText(name)
        if idx >= 0:
            self.cmb_saved_paths.setCurrentIndex(idx)

    def _on_load_saved_path(self):
        if not self._allow_action('load_path', 0.5):
            return
        name = self.cmb_saved_paths.currentText().strip() or self.ed_saved_path_name.text().strip()
        if not name:
            return

        points = self._parse_saved_path_file(name)
        if points:
            self._planned = points
            self._refresh_table()
            self._js_set_mission()

        if self._ros:
            self._ros.load_saved_path(name)

    @staticmethod
    def _find_circuit_yaml_path() -> Optional[str]:
        """Locate the workspace root and circuit_1.yaml without hardcoding an absolute path."""
        here = os.path.abspath(os.path.dirname(__file__))
        for _ in range(12):
            candidate = os.path.join(here, 'src', 'car_python', 'config', 'circuit_1.yaml')
            if os.path.exists(candidate):
                return candidate
            parent = os.path.dirname(here)
            if parent == here:
                break
            here = parent
        return None

    def _export_goal_points_to_circuit_yaml(self, planned_latlon: List[Tuple[float, float]]):
        if not planned_latlon:
            return

        if (
            self._robot_lat is None
            or self._robot_lon is None
            or self._robot_heading_deg is None
        ):
            raise RuntimeError('Missing robot origin/heading (need GPS + orientation)')

        # Precompute yaw (ENU) from compass bearing
        yaw_enu_deg = (90.0 - float(self._robot_heading_deg)) % 360.0
        yaw = math.radians(yaw_enu_deg)
        cy = math.cos(yaw)
        sy = math.sin(yaw)

        goal_pts: List[Tuple[float, float, float]] = []
        for lat, lon in planned_latlon:
            # ENU vector robot -> waypoint
            x_e_m, y_n_m = ll2xy(lat, lon, self._robot_lat, self._robot_lon)
            # Rotate ENU -> base_link (X forward, Y left)
            x_fwd_m = (cy * x_e_m) + (sy * y_n_m)
            y_left_m = (-sy * x_e_m) + (cy * y_n_m)
            goal_pts.append((x_fwd_m, y_left_m, 0.0))

        yaml_path = self._find_circuit_yaml_path()
        if not yaml_path:
            raise FileNotFoundError('Could not find src/car_python/config/circuit_1.yaml')

        self._write_goal_points_in_yaml(yaml_path, goal_pts)
        print(f"[cuadriga_gui] Wrote {len(goal_pts)} goal_points to {yaml_path}")

    @staticmethod
    def _write_goal_points_in_yaml(yaml_path: str, goal_pts: List[Tuple[float, float, float]]):
        with open(yaml_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Match the whole goal_points bracket list, preserving indentation and trailing comment.
        # Example:
        #   goal_points: [x,y,0.0,
        #                ...]  # comment
        pattern = re.compile(
            r'(^[\t ]*goal_points:[\t ]*\[)(.*?)(\][^\n]*\n)',
            re.MULTILINE | re.DOTALL,
        )
        m = pattern.search(content)
        if not m:
            raise RuntimeError('goal_points list not found in YAML')

        prefix = m.group(1)
        suffix = m.group(3)

        indent = re.match(r'^[\t ]*', prefix).group(0)
        cont_indent = indent + (' ' * 14)  # aligns with existing formatting

        # Preserve any trailing comment after the closing bracket on the same line.
        # suffix looks like: "]  # comment\n" or "]\n"
        comment = ''
        cm = re.match(r'\]([^\n]*)\n', suffix)
        if cm and cm.group(1):
            comment = cm.group(1)

        # Build multiline list body (without the opening '[' and closing ']')
        lines: List[str] = []
        for i, (x, y, z) in enumerate(goal_pts):
            if i == 0:
                lines.append(f"{x:.3f},{y:.3f},{z:.1f},")
            elif i == len(goal_pts) - 1:
                lines.append(f"\n{cont_indent}{x:.3f},{y:.3f},{z:.1f}")
            else:
                lines.append(f"\n{cont_indent}{x:.3f},{y:.3f},{z:.1f},")

        new_body = ''.join(lines)
        new_suffix = f"]{comment}\n"
        new_content = prefix + new_body + new_suffix

        updated = content[:m.start()] + new_content + content[m.end():]

        with open(yaml_path, 'w', encoding='utf-8') as f:
            f.write(updated)

    def _on_clear(self):
        self._planned.clear()
        self._refresh_table()
        self._js_call_safe('window.clearMission();')

    def _selected_waypoint_rows(self) -> List[int]:
        return sorted({index.row() for index in self.tbl.selectedIndexes()})

    @staticmethod
    def _interpolate_waypoints(
        start: Tuple[float, float],
        end: Tuple[float, float],
        count: int,
    ) -> List[Tuple[float, float]]:
        if count <= 0:
            return []
        lat1, lon1 = start
        lat2, lon2 = end
        points: List[Tuple[float, float]] = []
        for step in range(1, count + 1):
            ratio = step / float(count + 1)
            points.append((
                lat1 + ((lat2 - lat1) * ratio),
                lon1 + ((lon2 - lon1) * ratio),
            ))
        return points

    def _insert_intermediate_points(self, segment_start_indexes: List[int], count: int):
        if count <= 0 or len(self._planned) < 2 or not segment_start_indexes:
            return

        valid_indexes = {
            index for index in segment_start_indexes
            if 0 <= index < (len(self._planned) - 1)
        }
        if not valid_indexes:
            return

        densified: List[Tuple[float, float]] = []
        for index, point in enumerate(self._planned):
            densified.append(point)
            if index in valid_indexes:
                densified.extend(self._interpolate_waypoints(
                    self._planned[index],
                    self._planned[index + 1],
                    count,
                ))

        self._planned = densified
        self._refresh_table()
        self._js_set_mission()

    def _densify_by_spacing(self, max_spacing_m: float):
        if len(self._planned) < 2 or max_spacing_m <= 0.0:
            return

        densified: List[Tuple[float, float]] = []
        for index in range(len(self._planned) - 1):
            start = self._planned[index]
            end = self._planned[index + 1]
            densified.append(start)

            distance_m = haversine_m(start[0], start[1], end[0], end[1])
            extra_points = max(0, int(math.ceil(distance_m / max_spacing_m)) - 1)
            if extra_points > 0:
                densified.extend(self._interpolate_waypoints(start, end, extra_points))

        densified.append(self._planned[-1])
        self._planned = densified
        self._refresh_table()
        self._js_set_mission()

    def _on_insert_selected_intermediate_points(self):
        if not self._allow_action('insert_selected_points', 0.2):
            return
        rows = self._selected_waypoint_rows()
        if len(rows) < 2:
            return

        count = int(self.spn_intermediate_points.value())
        segment_start_indexes = [
            rows[i] for i in range(len(rows) - 1)
            if rows[i + 1] == rows[i] + 1
        ]
        self._insert_intermediate_points(segment_start_indexes, count)

    def _on_insert_all_intermediate_points(self):
        if not self._allow_action('insert_all_points', 0.2):
            return
        if len(self._planned) < 2:
            return
        count = int(self.spn_intermediate_points.value())
        self._insert_intermediate_points(list(range(len(self._planned) - 1)), count)

    def _on_densify_by_spacing(self):
        if not self._allow_action('densify_spacing', 0.2):
            return
        self._densify_by_spacing(float(self.spn_spacing_m.value()))

    def _on_start_path_following(self):
        if not self._allow_action('start_path_following', 0.6):
            return
        if not self._ros or not self._planned:
            return

        self._on_send_path()
        self._ros.start_path_following()

    def _on_stop_path_following(self):
        if not self._allow_action('stop_path_following', 0.4):
            return
        if not self._ros:
            return
        self._ros.stop_path_following()

    def _on_cancel_path_execution(self):
        if not self._allow_action('cancel_path_execution', 0.4):
            return
        if self._ros:
            self._ros.stop_path_following()
            self._ros.clear_active_path()
        self._on_clear()

    def _on_delete_selected(self):
        if not self._allow_action('delete_selected', 0.15):
            return
        rows = sorted({idx.row() for idx in self.tbl.selectedIndexes()}, reverse=True)
        if not rows:
            return
        for r in rows:
            if 0 <= r < len(self._planned):
                self._planned.pop(r)
        self._refresh_table()
        self._js_set_mission()  # <-- actualiza el mapa

    def _on_row_double_clicked(self, row, col):
        if 0 <= row < len(self._planned):
            self._planned.pop(row)
            self._refresh_table()
            self._js_set_mission()

    def on_remove_index_from_map(self, idx: int):
        try:
            i = int(idx)
        except Exception:
            return
        if 0 <= i < len(self._planned):
            self._planned.pop(i)
            self._refresh_table()
            self._js_set_mission()  # <- reenvía toda la lista al mapa


    def _on_load_csv(self):
        if not self._allow_action('load_csv', 0.5):
            return
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

    # ---- Utils ----
    def _allow_action(self, key: str, cooldown_s: float) -> bool:
        now = time.monotonic()
        last = self._action_timestamps.get(key, 0.0)
        if now - last < cooldown_s:
            return False
        self._action_timestamps[key] = now
        return True

    def _refresh_table(self):
        self._table_refresh_pending = True
        if not self._table_refresh_timer.isActive():
            self._table_refresh_timer.start(0)

    def _apply_table_refresh(self):
        if not self._table_refresh_pending:
            return
        self._table_refresh_pending = False

        heading_ready = (
            self._robot_lat is not None
            and self._robot_lon is not None
            and self._robot_heading_deg is not None
        )
        if heading_ready:
            yaw_enu_deg = (90.0 - self._robot_heading_deg) % 360.0
            yaw = math.radians(yaw_enu_deg)
            cos_yaw = math.cos(yaw)
            sin_yaw = math.sin(yaw)
        else:
            cos_yaw = 0.0
            sin_yaw = 0.0

        self.tbl.setUpdatesEnabled(False)
        self.tbl.blockSignals(True)
        self.tbl.setRowCount(len(self._planned))
        try:
            for i, (lat, lon) in enumerate(self._planned):
                self.tbl.setItem(i, 0, QTableWidgetItem(str(i)))
                self.tbl.setItem(i, 1, QTableWidgetItem(f"{lat:.7f}"))
                self.tbl.setItem(i, 2, QTableWidgetItem(f"{lon:.7f}"))

                if heading_ready:
                    try:
                        x_e_m, y_n_m = ll2xy(lat, lon, self._robot_lat, self._robot_lon)
                        x_fwd_m = (cos_yaw * x_e_m) + (sin_yaw * y_n_m)
                        y_left_m = (-sin_yaw * x_e_m) + (cos_yaw * y_n_m)
                        self.tbl.setItem(i, 3, QTableWidgetItem(f"{x_fwd_m:.2f}"))
                        self.tbl.setItem(i, 4, QTableWidgetItem(f"{y_left_m:.2f}"))
                    except Exception:
                        self.tbl.setItem(i, 3, QTableWidgetItem(''))
                        self.tbl.setItem(i, 4, QTableWidgetItem(''))
                else:
                    self.tbl.setItem(i, 3, QTableWidgetItem(''))
                    self.tbl.setItem(i, 4, QTableWidgetItem(''))
        finally:
            self.tbl.blockSignals(False)
            self.tbl.setUpdatesEnabled(True)

    def _js_set_mission(self):
        self._mission_sync_pending = True
        if not self._mission_sync_timer.isActive():
            self._mission_sync_timer.start(40)

    def _apply_js_set_mission(self):
        if not self._mission_sync_pending:
            return
        self._mission_sync_pending = False
        js_array = json.dumps(self._planned)
        self._js_call_safe(f'window.setMission({js_array});')

    def _pan(self, lat, lon):
        self._js_call_safe(f'window.panTo({lat}, {lon});')

    def _js_call_safe(self, js: str):
        if callable(self._js_call):
            self._js_call(js)
