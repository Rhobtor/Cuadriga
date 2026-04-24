import os
import threading
import time

import requests
from PySide6 import QtCore
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QLabel, QCheckBox, QPlainTextEdit, QComboBox
from PySide6.QtGui import QImage, QPixmap
import math
import numpy as np
import rclpy
from sensor_msgs.msg import Image, CompressedImage
from std_msgs.msg import Float32, Int32, Float32MultiArray
from geometry_msgs.msg import PoseArray
from rclpy.qos import qos_profile_sensor_data, QoSProfile, ReliabilityPolicy, HistoryPolicy

try:
    from rosidl_runtime_py.utilities import get_message
except ImportError:
    get_message = None



class FollowZEDWidget(QWidget):
    leader_signal = QtCore.Signal(str, str)
    detections_signal = QtCore.Signal(object)

    def __init__(self, node, topic_base='/follow_zed/image_for_gui'):
        super().__init__()
        self.node = node
        self.topic_raw = topic_base
        self.topic_comp = topic_base + '/compressed'
        self._relay_base_url = os.environ.get('IMAGE_RELAY_BASE_URL', 'http://127.0.0.1:8080').rstrip('/')
        self._image_topics = {
            'zed_rgb_original': self._relay_base_url + '/snapshot/rgb_original',
            'fanet_rgb_annotated': self._relay_base_url + '/snapshot/rgb_overlay',
            'fanet_thermal_annotated': self._relay_base_url + '/snapshot/thermal_overlay',
            'fanet_thermal_original': self._relay_base_url + '/snapshot/thermal',
        }
        self._image_labels = {
            'zed_rgb_original': 'ZED RGB original',
            'fanet_rgb_annotated': 'RGB anotada',
            'fanet_thermal_annotated': 'Térmica anotada',
            'fanet_thermal_original': 'Térmica original',
        }
        self._selected_source = 'zed_rgb_original'


        lay = QVBoxLayout(self)
        hdr = QHBoxLayout()
        self.status = QLabel('Video: esperando…')
        self.fit = QCheckBox('Ajustar a panel')
        self.fit.setChecked(True)
        self.cmb_source = QComboBox()
        self.cmb_source.addItem('ZED RGB original', userData='zed_rgb_original')
        self.cmb_source.addItem('RGB anotada', userData='fanet_rgb_annotated')
        self.cmb_source.addItem('Térmica anotada', userData='fanet_thermal_annotated')
        self.cmb_source.addItem('Térmica original', userData='fanet_thermal_original')
        self.cmb_source.currentIndexChanged.connect(self._on_source_changed)
        hdr.addWidget(self.status)
        hdr.addStretch(1)
        hdr.addWidget(QLabel('Fuente:'))
        hdr.addWidget(self.cmb_source)
        hdr.addWidget(self.fit)
        lay.addLayout(hdr)


        self.view = QLabel(alignment=QtCore.Qt.AlignCenter)
        self.view.setMinimumSize(320, 240)
        lay.addWidget(self.view)

        telemetry = QHBoxLayout()
        self.person_id = QLabel('Personas: 0')
        self.person_distance = QLabel('Última distancia: --')
        telemetry.addWidget(self.person_id)
        telemetry.addStretch(1)
        telemetry.addWidget(self.person_distance)
        lay.addLayout(telemetry)


        self.logs = QPlainTextEdit(); self.logs.setReadOnly(True); self.logs.setMaximumHeight(120)
        lay.addWidget(self.logs)

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
        self._detection_subscriptions_started = False
        self._sub_fanet_rgb = None
        self._sub_fanet_thermal = None
        self._sub_zed_rgb = None
        self._sub_thermal_raw = None
        self._sub_centroids = None
        self._sub_positions = None
        self._sub_distances = None
        self._fanet_centroids = []
        self._fanet_positions = []
        self._fanet_distances = []
        self._fanet_frame_count = 0
        self._last_detection_signature = None
        self._latest_qimage = None
        self._latest_status_text = None
        self._http_session = requests.Session()
        self._http_thread = None
        self._http_stop_event = threading.Event()
        self._frame_timer = QtCore.QTimer(self)
        self._frame_timer.setInterval(50)
        self._frame_timer.timeout.connect(self._flush_latest_image)
        self._frame_timer.start()
        self.status.setText('Video: suscripciones diferidas hasta abrir la pestaña')

    def log(self, s):
        try:
            self.logs.appendPlainText(s)
        except Exception:
            pass

    def ensure_started(self):
        if self._subscriptions_started:
            return
        self._subscriptions_started = True
        self.ensure_detection_started()
        self._start_http_polling()
        self._leader_id_sub = self.node.create_subscription(Int32, '/follow_zed/leader_id', self._on_leader_id_msg, 10)
        self._leader_distance_sub = self.node.create_subscription(Float32, '/follow_zed/leader_distance_state_m', self._on_leader_distance_msg, 10)
        self._leader_distance_fallback_sub = self.node.create_subscription(Float32, '/follow_zed/leader_distance_m', self._on_leader_distance_msg, 10)

        if get_message is not None:
            self._leader_probe_timer = self.node.create_timer(1.0, self._ensure_leader_subscription)

    def ensure_detection_started(self):
        if self._detection_subscriptions_started:
            return
        self._detection_subscriptions_started = True
        self._subscribe_detections()

    def set_ros(self, node):
        self.node = node

    def _start_http_polling(self):
        if self._http_thread is not None and self._http_thread.is_alive():
            return
        self._http_stop_event.clear()
        self._http_thread = threading.Thread(target=self._http_poll_loop, daemon=True)
        self._http_thread.start()
        self.log(f'Video -> relay HTTP {self._relay_base_url}')

    def _http_poll_loop(self):
        while not self._http_stop_event.is_set():
            source_key = self._selected_source
            url = self._image_topics.get(source_key)
            if not url:
                time.sleep(0.2)
                continue
            try:
                response = self._http_session.get(url, timeout=(0.5, 1.0))
                if response.status_code == 200:
                    qimg = QImage.fromData(response.content)
                    if not qimg.isNull():
                        self._emit_image_if_selected(
                            source_key,
                            qimg,
                            self._build_relay_status_text(source_key, response.headers),
                        )
                elif response.status_code != 503:
                    self._latest_status_text = f'Video: relay HTTP {response.status_code}'
            except Exception:
                self._latest_status_text = 'Video: esperando relay HTTP'
            time.sleep(0.08)

    def _build_relay_status_text(self, source_key: str, headers) -> str:
        label = self._image_labels.get(source_key, source_key)
        age_ms_raw = headers.get('X-Relay-Age-Ms')
        sequence = headers.get('X-Relay-Sequence')

        suffix = 'relay'
        if age_ms_raw is not None:
            try:
                age_ms = int(age_ms_raw)
                suffix = f'relay {age_ms} ms'
            except ValueError:
                suffix = 'relay'

        if sequence:
            suffix = f'{suffix} seq {sequence}'

        return f'{label}: {suffix}'

    def _subscribe_detections(self):
        try:
            self._sub_centroids = self.node.create_subscription(
                PoseArray, '/fanet/person_centroids', self._on_person_centroids, QoSProfile(depth=10))
            self._sub_positions = self.node.create_subscription(
                PoseArray, '/fanet/person_positions_robot', self._on_person_positions, QoSProfile(depth=10))
            self._sub_distances = self.node.create_subscription(
                Float32MultiArray, '/fanet/person_distances', self._on_person_distances, QoSProfile(depth=10))
            self.log('FANET -> sub a detecciones')
        except Exception as e:
            self.log(f'FANET -> error sub detecciones: {e}')

    def _on_source_changed(self, _index: int):
        selected = self.cmb_source.currentData()
        if selected:
            self._selected_source = selected
            self.status.setText(f'Video: fuente {self._image_labels.get(selected, selected)}')

    def _emit_image_if_selected(self, source_key: str, qimg: QImage, status_text: str):
        if self._selected_source != source_key:
            return
        self._latest_qimage = qimg
        self._latest_status_text = status_text

    def _flush_latest_image(self):
        qimg = self._latest_qimage
        if not isinstance(qimg, QImage):
            return
        self._latest_qimage = None
        self._on_qimage(qimg)
        if self._latest_status_text:
            self.status.setText(self._latest_status_text)

    def _on_qimage(self, qimg):
        if not isinstance(qimg, QImage):
            return
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

    def _on_person_centroids(self, msg: PoseArray):
        self._fanet_centroids = list(msg.poses)
        self._set_fanet_frame_count(len(self._fanet_centroids))
        self._publish_fanet_snapshot()

    def _on_person_positions(self, msg: PoseArray):
        self._fanet_positions = list(msg.poses)
        self._set_fanet_frame_count(len(self._fanet_positions))
        self._publish_fanet_snapshot()

    def _on_person_distances(self, msg: Float32MultiArray):
        self._fanet_distances = [float(value) for value in msg.data]
        self._set_fanet_frame_count(len(self._fanet_distances))
        self._publish_fanet_snapshot()

    def _set_fanet_frame_count(self, count: int):
        self._fanet_frame_count = max(0, int(count))
        if len(self._fanet_centroids) > self._fanet_frame_count:
            self._fanet_centroids = self._fanet_centroids[:self._fanet_frame_count]
        if len(self._fanet_positions) > self._fanet_frame_count:
            self._fanet_positions = self._fanet_positions[:self._fanet_frame_count]
        if len(self._fanet_distances) > self._fanet_frame_count:
            self._fanet_distances = self._fanet_distances[:self._fanet_frame_count]

    def _build_fanet_detections(self):
        detections = []
        count = self._fanet_frame_count
        for index in range(count):
            centroid = self._fanet_centroids[index] if index < len(self._fanet_centroids) else None
            position = self._fanet_positions[index] if index < len(self._fanet_positions) else None
            distance = self._fanet_distances[index] if index < len(self._fanet_distances) else math.nan

            detection = {
                'id': index + 1,
                'pixel_x': None,
                'pixel_y': None,
                'area_px': None,
                'robot_x': None,
                'robot_y': None,
                'robot_z': None,
                'distance_m': None,
            }

            if centroid is not None:
                detection['pixel_x'] = float(centroid.position.x)
                detection['pixel_y'] = float(centroid.position.y)
                detection['area_px'] = float(centroid.position.z)

            if position is not None:
                detection['robot_x'] = float(position.position.x)
                detection['robot_y'] = float(position.position.y)
                detection['robot_z'] = float(position.position.z)

            if math.isfinite(distance):
                detection['distance_m'] = float(distance)

            detections.append(detection)

        return detections

    def _publish_fanet_snapshot(self):
        detections = self._build_fanet_detections()
        self.person_id.setText(f'Personas: {len(detections)}')

        last_distance = '--'
        if detections:
            for detection in detections:
                distance = detection.get('distance_m', None)
                if distance is not None and math.isfinite(distance):
                    last_distance = f'{distance:.2f} m'
                    break
        self.person_distance.setText(f'Última distancia: {last_distance}')

        signature = tuple(
            (
                detection['id'],
                None if detection['pixel_x'] is None else round(float(detection['pixel_x']), 1),
                None if detection['pixel_y'] is None else round(float(detection['pixel_y']), 1),
                None if detection['robot_x'] is None else round(float(detection['robot_x']), 2),
                None if detection['robot_y'] is None else round(float(detection['robot_y']), 2),
                None if detection['distance_m'] is None else round(float(detection['distance_m']), 2),
            )
            for detection in detections
        )

        if signature != self._last_detection_signature:
            self._last_detection_signature = signature
            if not detections:
                self.log('FANET -> sin personas detectadas')
            else:
                summary = []
                for detection in detections:
                    px = '--' if detection['pixel_x'] is None or detection['pixel_y'] is None else f"px=({detection['pixel_x']:.0f},{detection['pixel_y']:.0f})"
                    robot = '--' if detection['robot_x'] is None or detection['robot_y'] is None else f"robot=({detection['robot_x']:.2f},{detection['robot_y']:.2f})"
                    dist = '--' if detection['distance_m'] is None else f"dist={detection['distance_m']:.2f}m"
                    summary.append(f"P{detection['id']}: {px} {robot} {dist}")
                self.log('FANET -> ' + ' | '.join(summary))

        self.detections_signal.emit(detections)

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

    def closeEvent(self, ev):
        self._http_stop_event.set()
        try:
            self._http_session.close()
        except Exception:
            pass
        super().closeEvent(ev)


    def resizeEvent(self, ev):
        pm = self.view.pixmap()
        if pm and self.fit.isChecked():
            self.view.setPixmap(pm.scaled(self.view.size(), QtCore.Qt.KeepAspectRatio, QtCore.Qt.SmoothTransformation))
        super().resizeEvent(ev)