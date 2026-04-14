# j8_gui/app/control_widget.py
# -*- coding: utf-8 -*-
"""j8_gui/app/control_widget.py

Pestaña Control desacoplada.
- Muestra parámetros de control (vel/acc) y modo follow.
- Aplica la configuración vía RosSide.send_cfg(cfg_dict) (API real J8, aún por mapear a servicios).
- Puede usar el RosSide existente (set_ros) o crear su propio nodo (attach_ros) si se le pasa un topic.

Ejemplo (integrado con RosSide, recomendado):
    self.control_tab = ControlWidget(self._ros)
    self.tabs.addTab(self.control_tab, 'Control')
    ...
    # cuando arranque ROS:
    self.control_tab.set_ros(self._ros)

Ejemplo (modo autónomo, opcional):
    self.control_tab = ControlWidget()  # sin RosSide
    self.tabs.addTab(self.control_tab, 'Control')
    ...
    # cuando arranque ROS:
    # Para publicar a un topic debes pasar explícitamente uno (no se usan /gui/* por defecto)
    self.control_tab.attach_ros(self._exec, topic='/some/topic')

Comprobación (si usas modo autónomo y publicas a un topic):
    ros2 topic echo /some/topic
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Optional

from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QFormLayout, QLabel,
    QLineEdit, QComboBox, QPushButton, QHBoxLayout
)


@dataclass(frozen=True)
class ControlConfig:
        """UI -> ROS config payload.

        Notes:
            - `controller_type` is sent via `ctl_mission_interfaces/srv/ChangeController`.
            - The rest is sent via the corresponding `Config*` service.
            - `*_max`/`acc_*` are currently UI-only (we don't have a known J8 API for them yet).
        """

        controller_type: str
        v_forward: float
        l_ahead_dist: float
        k_error_lineal: float | None = None
        k_error_angular: float | None = None
        look_ahead_dis: float | None = None
        r_min: float | None = None
        # UI-only for now
        lin_max: float | None = None
        ang_max: float | None = None
        acc_lin: float | None = None
        acc_ang: float | None = None


class ControlWidget(QWidget):
    """
    UI:
      - Vel. lineal máx [m/s]
      - Vel. angular máx [rad/s]
      - Acel. lineal [m/s²]
      - Acel. angular [rad/s²]
      - Modo Follow (combo)
      - Botón 'Aplicar configuración'

        Publicación:
            - Si se llama set_ros(ros_side): usa ros_side.send_cfg(cfg_dict)
                (en nuestra app, RosSide traduce esto a servicios reales del J8)
            - El modo autónomo por topic (JSON) era prototipo y se mantiene solo como fallback
                para no romper usos existentes, pero no se usa por defecto.
    """

    def __init__(self, ros: Optional[object] = None, topic: str = ''):
        super().__init__()
        self._ros = ros                 # RosSide con .send_cfg(dict) o None
        self._topic = topic             # Topic para modo autónomo
        self._node = None               # Nodo propio (si attach_ros)
        self._pub = None                # Publisher propio (si attach_ros)

        # ===== UI =====
        root = QVBoxLayout(self)

        # --- Selección de controlador ---
        root.addWidget(QLabel('Controlador de seguimiento:'))
        self.cmb_controller = QComboBox()
        # nombres reales en CtrlNode::available_controller_types_
        self.cmb_controller.addItems([
            'pure_pursuit',
            'regulated_pure_pursuit',
            'dynamic_pure_pursuit',
            'dynamic_la_pure_pursuit',
            'stanley',
        ])
        self.cmb_controller.currentTextChanged.connect(self._update_controller_fields_visibility)
        root.addWidget(self.cmb_controller)

        frm = QFormLayout()
        # No ponemos QDoubleValidator para no pelear con locales (coma/punto), validamos al aplicar.
        self.ed_vlin = QLineEdit('1.0'); self.ed_vlin.setPlaceholderText('m/s')
        self.ed_vang = QLineEdit('0.6'); self.ed_vang.setPlaceholderText('rad/s')
        self.ed_alin = QLineEdit('0.5'); self.ed_alin.setPlaceholderText('m/s²')
        self.ed_aang = QLineEdit('0.8'); self.ed_aang.setPlaceholderText('rad/s²')

        frm.addRow('Vel. lineal máx [m/s]:', self.ed_vlin)
        frm.addRow('Vel. angular máx [rad/s]:', self.ed_vang)
        frm.addRow('Acel. lineal [m/s²]:', self.ed_alin)
        frm.addRow('Acel. angular [rad/s²]:', self.ed_aang)

        root.addLayout(frm)

        # --- Parámetros de controller (real) ---
        ctrl_frm = QFormLayout()

        self.ed_v_forward = QLineEdit('0.8'); self.ed_v_forward.setPlaceholderText('m/s')
        self.ed_l_ahead = QLineEdit('2.0'); self.ed_l_ahead.setPlaceholderText('m')
        ctrl_frm.addRow('v_forward [m/s]:', self.ed_v_forward)
        ctrl_frm.addRow('look_ahead_dist [m]:', self.ed_l_ahead)

        # Stanley-only
        self.ed_k_lineal = QLineEdit('0.2'); self.ed_k_lineal.setPlaceholderText('')
        self.ed_k_angular = QLineEdit('1.0'); self.ed_k_angular.setPlaceholderText('')
        ctrl_frm.addRow('Stanley k_error_lineal:', self.ed_k_lineal)
        ctrl_frm.addRow('Stanley k_error_angular:', self.ed_k_angular)

        # Regulated-only
        self.ed_rpp_r_min = QLineEdit('0.8'); self.ed_rpp_r_min.setPlaceholderText('m')
        self.ed_rpp_look_ahead = QLineEdit('2.0'); self.ed_rpp_look_ahead.setPlaceholderText('m')
        ctrl_frm.addRow('Regulated r_min [m]:', self.ed_rpp_r_min)
        ctrl_frm.addRow('Regulated look_ahead_dis [m]:', self.ed_rpp_look_ahead)

        root.addWidget(QLabel('Parámetros del controlador:'))
        root.addLayout(ctrl_frm)

        btn_row = QHBoxLayout()
        self.b_apply = QPushButton('Aplicar configuración')
        self.b_apply.clicked.connect(self._on_apply_cfg)

        self.b_reset = QPushButton('Valores por defecto')
        self.b_reset.clicked.connect(self._on_reset_defaults)

        btn_row.addWidget(self.b_apply)
        btn_row.addWidget(self.b_reset)
        root.addLayout(btn_row)

        root.addStretch(1)

        # Enter en cualquier campo = aplicar
        self.ed_vlin.returnPressed.connect(self.b_apply.click)
        self.ed_vang.returnPressed.connect(self.b_apply.click)
        self.ed_alin.returnPressed.connect(self.b_apply.click)
        self.ed_aang.returnPressed.connect(self.b_apply.click)

        self.ed_v_forward.returnPressed.connect(self.b_apply.click)
        self.ed_l_ahead.returnPressed.connect(self.b_apply.click)
        self.ed_k_lineal.returnPressed.connect(self.b_apply.click)
        self.ed_k_angular.returnPressed.connect(self.b_apply.click)
        self.ed_rpp_r_min.returnPressed.connect(self.b_apply.click)
        self.ed_rpp_look_ahead.returnPressed.connect(self.b_apply.click)

        self._update_controller_fields_visibility(self.cmb_controller.currentText())

    # ---------- API pública ----------
    def set_ros(self, ros_side: object):
        """
        Inyecta el RosSide existente.
        Debe exponer: send_cfg(self, cfg: dict)
        """
        self._ros = ros_side

    def attach_ros(self, executor, topic: Optional[str] = None):
        """
        Crea un nodo propio y un publisher a `topic` (por defecto self._topic).
        Lo añade al executor que ya está spinnando en MainWindow.
        """
        if topic:
            self._topic = topic

        if self._node is not None and self._pub is not None:
            # ya adjunto
            return

        # Mantener fallback del prototipo (publisher JSON) para no romper, pero no recomendado.
        # Import local para no forzar dependencias si no se usa modo autónomo
        from rclpy.node import Node
        from std_msgs.msg import String

        class _CtrlNode(Node):
            def __init__(self, t: str):
                super().__init__('control_widget_node')
                self.pub = self.create_publisher(String, t, 10)

        self._node = _CtrlNode(self._topic)
        self._pub = self._node.pub
        executor.add_node(self._node)
        print(f'[ControlWidget] [LEGACY] Nodo propio adjuntado, publicando JSON en {self._topic}')

    # ---------- Lógica ----------
    def _on_apply_cfg(self):
        cfg = self._read_cfg_from_fields()
        if cfg is None:
            print('[ControlWidget] Valores inválidos; no se envía configuración.')
            return

        # Prioridad: RosSide (servicios reales) > fallback publisher JSON
        if self._ros is not None and hasattr(self._ros, 'send_cfg'):
            # Convertimos a dict simple para RosSide
            self._ros.send_cfg(self._cfg_to_dict(cfg))
            print('[ControlWidget] CFG enviado (RosSide/services) ->', cfg)
            return

        # Fallback heredado (no recomendado)
        if self._pub is not None:
            import json
            from std_msgs.msg import String

            msg = String()
            msg.data = json.dumps(self._cfg_to_dict(cfg))
            self._pub.publish(msg)
            print('[ControlWidget] [LEGACY] CFG enviado (publisher JSON) ->', cfg)
        else:
            print('[ControlWidget] No hay ROS inicializado (ni RosSide ni attach_ros).')

    def _on_reset_defaults(self):
        self.ed_vlin.setText('1.0')
        self.ed_vang.setText('0.6')
        self.ed_alin.setText('0.5')
        self.ed_aang.setText('0.8')
    # No cambiamos el controller
        print('[ControlWidget] Restablecidos valores por defecto.')

    # ---------- Utilidades ----------
    def _read_cfg_from_fields(self) -> Optional[ControlConfig]:
        """Lee UI y devuelve `ControlConfig` o None si hay error."""
        try:
            lin_max = self._to_float(self.ed_vlin.text())
            ang_max = self._to_float(self.ed_vang.text())
            acc_lin = self._to_float(self.ed_alin.text())
            acc_ang = self._to_float(self.ed_aang.text())

            v_forward = self._to_float(self.ed_v_forward.text())
            l_ahead_dist = self._to_float(self.ed_l_ahead.text())
        except Exception:
            return None

        ctrl_type = self.cmb_controller.currentText().strip()

        # Controller-specific
        k_lineal = k_angular = None
        r_min = look_ahead_dis = None

        if ctrl_type == 'stanley':
            try:
                k_lineal = self._to_float(self.ed_k_lineal.text())
                k_angular = self._to_float(self.ed_k_angular.text())
            except Exception:
                return None

        if ctrl_type == 'regulated_pure_pursuit':
            try:
                r_min = self._to_float(self.ed_rpp_r_min.text())
                look_ahead_dis = self._to_float(self.ed_rpp_look_ahead.text())
            except Exception:
                return None

        return ControlConfig(
            controller_type=ctrl_type,
            v_forward=v_forward,
            l_ahead_dist=l_ahead_dist,
            k_error_lineal=k_lineal,
            k_error_angular=k_angular,
            look_ahead_dis=look_ahead_dis,
            r_min=r_min,
            lin_max=lin_max,
            ang_max=ang_max,
            acc_lin=acc_lin,
            acc_ang=acc_ang,
        )

    @staticmethod
    def _cfg_to_dict(cfg: ControlConfig) -> dict:
        d = {
            'controller_type': cfg.controller_type,
            'v_forward': cfg.v_forward,
            'l_ahead_dist': cfg.l_ahead_dist,
            # UI-only still included in case we map later
            'lin_max': cfg.lin_max,
            'ang_max': cfg.ang_max,
            'acc_lin': cfg.acc_lin,
            'acc_ang': cfg.acc_ang,
        }

        if cfg.k_error_lineal is not None:
            d['k_error_lineal'] = cfg.k_error_lineal
        if cfg.k_error_angular is not None:
            d['k_error_angular'] = cfg.k_error_angular
        if cfg.look_ahead_dis is not None:
            d['look_ahead_dis'] = cfg.look_ahead_dis
        if cfg.r_min is not None:
            d['r_min'] = cfg.r_min
        return d

    def _update_controller_fields_visibility(self, controller_type: str):
        controller_type = (controller_type or '').strip()

        is_stanley = controller_type == 'stanley'
        is_regulated = controller_type == 'regulated_pure_pursuit'

        self.ed_k_lineal.setEnabled(is_stanley)
        self.ed_k_angular.setEnabled(is_stanley)

        self.ed_rpp_r_min.setEnabled(is_regulated)
        self.ed_rpp_look_ahead.setEnabled(is_regulated)

    @staticmethod
    def _to_float(s: str) -> float:
        """
        Convierte string a float aceptando ',' o '.' como separador decimal.
        Lanza excepción si no es convertible.
        """
        if s is None:
            raise ValueError('empty')
        s = s.strip().replace(',', '.')
        return float(s)
