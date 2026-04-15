from PySide6 import QtCore
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QLabel, QCheckBox, QPlainTextEdit
from PySide6.QtGui import QImage, QPixmap
import math
import numpy as np
import rclpy
from sensor_msgs.msg import Image, CompressedImage
from std_msgs.msg import Float32, Int32
from rclpy.qos import qos_profile_sensor_data, QoSProfile

try:
    from rosidl_runtime_py.utilities import get_message
except ImportError:
    get_message = None



class FollowZEDWidget(QWidget):
    image_signal = QtCore.Signal(QImage)
    leader_signal = QtCore.Signal(str, str)

    def __init__(self, node, topic_base='/follow_zed/image_for_gui'):
        super().__init__()
        self.node = node
        self.topic_raw = topic_base
        self.topic_comp = topic_base + '/compressed'


        lay = QVBoxLayout(self)
        hdr = QHBoxLayout()
        self.status = QLabel('FollowZED: esperando…')
        self.fit = QCheckBox('Ajustar a panel')
        self.fit.setChecked(True)
        hdr.addWidget(self.status)
        hdr.addStretch(1)
        hdr.addWidget(self.fit)
        lay.addLayout(hdr)


        self.view = QLabel(alignment=QtCore.Qt.AlignCenter)
        self.view.setMinimumSize(320, 240)
        lay.addWidget(self.view)

        telemetry = QHBoxLayout()
        self.person_id = QLabel('ID persona: --')
        self.person_distance = QLabel('Distancia: --')
        telemetry.addWidget(self.person_id)
        telemetry.addStretch(1)
        telemetry.addWidget(self.person_distance)
        lay.addLayout(telemetry)


        self.logs = QPlainTextEdit(); self.logs.setReadOnly(True); self.logs.setMaximumHeight(120)
        lay.addWidget(self.logs)


        self.image_signal.connect(self._on_qimage)
        self.leader_signal.connect(self._on_leader_snapshot)
        self._sub_raw = None
        self._sub_comp = None
        self._leader_position_sub = None
        self._leader_probe_timer = None
        self._leader_topic = '/follow_zed/leader_position_camera'
        self._leader_id = None
        self._leader_distance = math.nan
        self._leader_last_rendered = (None, None)
        self._subscriptions_started = False
        self.status.setText('FollowZED: suscripciones diferidas hasta abrir la pestaña')

    def log(self, s):
        try:
            self.logs.appendPlainText(s)
        except Exception:
            pass

    def ensure_started(self):
        if self._subscriptions_started:
            return
        self._subscriptions_started = True
        self._subscribe()

    def set_ros(self, node):
        self.node = node

    def _subscribe(self):
        try:
            self._sub_raw = self.node.create_subscription(Image, self.topic_raw, self._on_raw, qos_profile_sensor_data)
            self.status.setText(f'FollowZED: sub a {self.topic_raw}')
        except Exception:
            try:
                self._sub_comp = self.node.create_subscription(CompressedImage, self.topic_comp, self._on_comp, qos_profile_sensor_data)
                self.status.setText(f'FollowZED: sub a {self.topic_comp}')
            except Exception as e:
                self.status.setText(f'FollowZED: error sub ({e})')

        self._leader_id_sub = self.node.create_subscription(Int32, '/follow_zed/leader_id', self._on_leader_id_msg, 10)
        self._leader_distance_sub = self.node.create_subscription(Float32, '/follow_zed/leader_distance_state_m', self._on_leader_distance_msg, 10)
        self._leader_distance_fallback_sub = self.node.create_subscription(Float32, '/follow_zed/leader_distance_m', self._on_leader_distance_msg, 10)

        if get_message is not None:
            self._leader_probe_timer = self.node.create_timer(1.0, self._ensure_leader_subscription)

    def _on_raw(self, msg: Image):
        try:
            arr = np.frombuffer(msg.data, dtype=np.uint8).reshape(msg.height, msg.width, -1)
            enc = msg.encoding.lower()
            if enc == 'rgb8':
                # QImage espera RGB, perfecto
                qimg = QImage(arr.data, msg.width, msg.height, msg.width * arr.shape[2], QImage.Format_RGB888).copy()
            elif enc == 'bgr8':
                arr = arr[..., ::-1].copy() # BGR->RGB
                qimg = QImage(arr.data, msg.width, msg.height, msg.width * arr.shape[2], QImage.Format_RGB888)
            else:
                self.status.setText(f'FollowZED: encoding no soportado {msg.encoding}')
                return
            self.image_signal.emit(qimg)
            self.status.setText(f'FollowZED: frame {msg.width}x{msg.height}')
        except Exception as e:
            self.status.setText(f'FollowZED: error {e}')

    def _on_comp(self, msg: CompressedImage):
        try:
            import cv2
            np_arr = np.frombuffer(msg.data, np.uint8)
            cv_bgr = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
            if cv_bgr is None:
                return
            cv_rgb = cv_bgr[..., ::-1] # BGR->RGB
            h, w, ch = cv_rgb.shape
            qimg = QImage(cv_rgb.data, w, h, ch * w, QImage.Format_RGB888).copy()
            self.image_signal.emit(qimg)
        except Exception as e:
            self.status.setText(f'FollowZED: error {e}')

    def _on_qimage(self, qimg: QImage):
        if self.fit.isChecked():
            pix = QPixmap.fromImage(qimg).scaled(self.view.size(), QtCore.Qt.KeepAspectRatio, QtCore.Qt.SmoothTransformation)
        else:
            pix = QPixmap.fromImage(qimg)
        self.view.setPixmap(pix)

    def _on_leader_snapshot(self, person_id_text: str, distance_text: str):
        self.person_id.setText(f'ID persona: {person_id_text}')
        self.person_distance.setText(f'Distancia: {distance_text}')

        snapshot = (person_id_text, distance_text)
        if snapshot != self._leader_last_rendered and snapshot != ('--', '--'):
            self._leader_last_rendered = snapshot
            self.log(f'Leader -> ID: {person_id_text} | Distancia: {distance_text}')

    def _publish_leader_snapshot(self):
        person_id_text = '--' if self._leader_id is None else str(self._leader_id)
        distance_text = '--' if not math.isfinite(self._leader_distance) else f'{self._leader_distance:.2f} m'
        self.leader_signal.emit(person_id_text, distance_text)

    def _on_leader_id_msg(self, msg: Int32):
        if msg.data < 0:
            return
        self._leader_id = int(msg.data)
        self._publish_leader_snapshot()

    def _on_leader_distance_msg(self, msg: Float32):
        distance = float(msg.data)
        if not math.isfinite(distance) or distance < 0.0:
            return
        self._leader_distance = distance
        self._publish_leader_snapshot()

    def _ensure_leader_subscription(self):
        if self._leader_position_sub is not None or get_message is None:
            return

        try:
            topic_map = dict(self.node.get_topic_names_and_types())
        except Exception:
            return

        topic_types = topic_map.get(self._leader_topic)
        if not topic_types:
            return

        try:
            msg_type = get_message(topic_types[0])
            self._leader_position_sub = self.node.create_subscription(
                msg_type,
                self._leader_topic,
                self._on_leader_position_msg,
                QoSProfile(depth=10),
            )
            self.log(f'Leader topic -> {self._leader_topic} [{topic_types[0]}]')
            if self._leader_probe_timer is not None:
                self._leader_probe_timer.cancel()
        except Exception as e:
            self.log(f'Leader topic no disponible: {e}')

    def _message_fields(self, msg):
        if hasattr(msg, 'get_fields_and_field_types'):
            return list(msg.get_fields_and_field_types().keys())
        return []

    def _extract_numeric_field(self, obj, field_names, depth=0):
        if obj is None or depth > 3:
            return None

        for field_name in field_names:
            if hasattr(obj, field_name):
                value = getattr(obj, field_name)
                if isinstance(value, (int, float, np.integer, np.floating)):
                    value = float(value)
                    if math.isfinite(value):
                        return value

        for field_name in self._message_fields(obj):
            value = getattr(obj, field_name)
            if hasattr(value, 'get_fields_and_field_types'):
                result = self._extract_numeric_field(value, field_names, depth + 1)
                if result is not None:
                    return result

        return None

    def _extract_xyz(self, obj, depth=0):
        if obj is None or depth > 3:
            return None

        if all(hasattr(obj, axis) for axis in ('x', 'y', 'z')):
            try:
                x = float(getattr(obj, 'x'))
                y = float(getattr(obj, 'y'))
                z = float(getattr(obj, 'z'))
            except (TypeError, ValueError):
                x = y = z = math.nan
            if all(math.isfinite(value) for value in (x, y, z)):
                return (x, y, z)

        for field_name in self._message_fields(obj):
            value = getattr(obj, field_name)
            if hasattr(value, 'get_fields_and_field_types'):
                coords = self._extract_xyz(value, depth + 1)
                if coords is not None:
                    return coords

        return None

    def _on_leader_position_msg(self, msg):
        leader_id = self._extract_numeric_field(
            msg, ('person_id', 'leader_id', 'track_id', 'tracking_id', 'id', 'object_id'))
        if leader_id is not None:
            self._leader_id = int(leader_id)

        distance = self._extract_numeric_field(
            msg, ('distance_m', 'distance', 'dist', 'range', 'depth_m', 'depth'))
        if distance is None:
            coords = self._extract_xyz(msg)
            if coords is not None:
                distance = math.sqrt(sum(value * value for value in coords))

        if distance is not None and math.isfinite(distance) and distance >= 0.0:
            self._leader_distance = float(distance)

        self._publish_leader_snapshot()


    def resizeEvent(self, ev):
        pm = self.view.pixmap()
        if pm and self.fit.isChecked():
            self.view.setPixmap(pm.scaled(self.view.size(), QtCore.Qt.KeepAspectRatio, QtCore.Qt.SmoothTransformation))
        super().resizeEvent(ev)