from __future__ import annotations

from typing import Iterable

from PySide6.QtCore import Qt, Signal
from PySide6.QtGui import QColor
from PySide6.QtWidgets import (
    QCheckBox,
    QComboBox,
    QDoubleSpinBox,
    QHBoxLayout,
    QLabel,
    QPushButton,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)


STATUS_LABELS = {
    'detected': 'Detectada',
    'pending': 'Pendiente',
    'rescued': 'Rescatada',
}

STATUS_COLORS = {
    'detected': QColor('#dc2626'),
    'pending': QColor('#facc15'),
    'rescued': QColor('#22c55e'),
}


class PersonsWidget(QWidget):
    statusChangeRequested = Signal(int, str)
    assignmentRequested = Signal(int, str)
    autoTrackingChanged = Signal(bool)
    thresholdsChanged = Signal(float, float)

    def __init__(self, parent=None):
        super().__init__(parent)
        self._records = []
        self._current_robot = 'cuadriga'
        self._current_role = 'explorador'

        root = QVBoxLayout(self)

        controls = QHBoxLayout()
        controls.addWidget(QLabel('Filtrar:'))
        self.cmb_filter = QComboBox()
        self.cmb_filter.addItem('Todas', userData='all')
        self.cmb_filter.addItem('Detectadas', userData='detected')
        self.cmb_filter.addItem('Pendientes', userData='pending')
        self.cmb_filter.addItem('Rescatadas', userData='rescued')
        self.cmb_filter.currentIndexChanged.connect(self._refresh_table)
        controls.addWidget(self.cmb_filter)

        self.lbl_summary = QLabel('0 detectadas | 0 pendientes | 0 rescatadas')
        controls.addWidget(self.lbl_summary, 1)
        root.addLayout(controls)

        auto_row = QHBoxLayout()
        self.chk_auto_tracking = QCheckBox('Seguimiento automatico')
        self.chk_auto_tracking.setChecked(True)
        self.chk_auto_tracking.toggled.connect(self.autoTrackingChanged.emit)
        auto_row.addWidget(self.chk_auto_tracking)

        auto_row.addWidget(QLabel('Pendiente <= m:'))
        self.spn_pending_m = QDoubleSpinBox()
        self.spn_pending_m.setRange(1.0, 500.0)
        self.spn_pending_m.setDecimals(1)
        self.spn_pending_m.setSingleStep(1.0)
        self.spn_pending_m.setValue(20.0)
        auto_row.addWidget(self.spn_pending_m)

        auto_row.addWidget(QLabel('Rescatada <= m:'))
        self.spn_rescued_m = QDoubleSpinBox()
        self.spn_rescued_m.setRange(1.0, 100.0)
        self.spn_rescued_m.setDecimals(1)
        self.spn_rescued_m.setSingleStep(0.5)
        self.spn_rescued_m.setValue(5.0)
        auto_row.addWidget(self.spn_rescued_m)

        self.btn_apply_thresholds = QPushButton('Aplicar umbrales')
        self.btn_apply_thresholds.clicked.connect(self._emit_thresholds_change)
        auto_row.addWidget(self.btn_apply_thresholds)
        root.addLayout(auto_row)

        assign_row = QHBoxLayout()
        self.lbl_current_robot = QLabel('Robot actual: cuadriga')
        assign_row.addWidget(self.lbl_current_robot)
        self.lbl_current_role = QLabel('Rol: explorador')
        assign_row.addWidget(self.lbl_current_role)
        assign_row.addStretch(1)
        root.addLayout(assign_row)

        self.tbl = QTableWidget(0, 7)
        self.tbl.setHorizontalHeaderLabels(['ID', 'Estado', 'Origen', 'Robot', 'Dist (m)', 'Lat', 'Lon'])
        self.tbl.setSelectionBehavior(QTableWidget.SelectRows)
        self.tbl.setSelectionMode(QTableWidget.SingleSelection)
        self.tbl.setEditTriggers(QTableWidget.NoEditTriggers)
        root.addWidget(self.tbl)

        actions = QHBoxLayout()
        self.btn_mark_detected = QPushButton('Marcar detectada')
        self.btn_mark_pending = QPushButton('Marcar pendiente')
        self.btn_mark_rescued = QPushButton('Marcar rescatada')
        self.btn_mark_detected.clicked.connect(lambda: self._emit_status_change('detected'))
        self.btn_mark_pending.clicked.connect(lambda: self._emit_status_change('pending'))
        self.btn_mark_rescued.clicked.connect(lambda: self._emit_status_change('rescued'))
        actions.addWidget(self.btn_mark_detected)
        actions.addWidget(self.btn_mark_pending)
        actions.addWidget(self.btn_mark_rescued)
        self.btn_assign_current_robot = QPushButton('Asignar robot actual')
        self.btn_clear_assignment = QPushButton('Liberar asignacion')
        self.btn_assign_current_robot.clicked.connect(self._emit_assignment_current_robot)
        self.btn_clear_assignment.clicked.connect(lambda: self._emit_assignment(''))
        actions.addWidget(self.btn_assign_current_robot)
        actions.addWidget(self.btn_clear_assignment)
        root.addLayout(actions)

    def set_records(self, records: Iterable[dict]):
        self._records = [dict(record) for record in records]
        self._refresh_table()

    def _selected_person_id(self):
        row = self.tbl.currentRow()
        if row < 0:
            return None
        item = self.tbl.item(row, 0)
        if item is None:
            return None
        try:
            return int(item.text())
        except Exception:
            return None

    def _emit_status_change(self, status: str):
        person_id = self._selected_person_id()
        if person_id is None:
            return
        self.statusChangeRequested.emit(person_id, status)

    def _emit_assignment_current_robot(self):
        self._emit_assignment(self._current_robot)

    def _emit_assignment(self, robot_name: str):
        person_id = self._selected_person_id()
        if person_id is None:
            return
        self.assignmentRequested.emit(person_id, str(robot_name or '').strip())

    def _emit_thresholds_change(self):
        pending_m = float(self.spn_pending_m.value())
        rescued_m = float(self.spn_rescued_m.value())
        self.thresholdsChanged.emit(pending_m, rescued_m)

    def set_current_robot(self, robot_name: str):
        self._current_robot = str(robot_name or '').strip() or 'cuadriga'
        self.lbl_current_robot.setText(f'Robot actual: {self._current_robot}')

    def set_current_role(self, role_name: str):
        self._current_role = str(role_name or '').strip() or 'explorador'
        self.lbl_current_role.setText(f'Rol: {self._current_role}')

    def set_auto_tracking(self, enabled: bool):
        self.chk_auto_tracking.blockSignals(True)
        self.chk_auto_tracking.setChecked(bool(enabled))
        self.chk_auto_tracking.blockSignals(False)

    def set_thresholds(self, pending_m: float, rescued_m: float):
        self.spn_pending_m.blockSignals(True)
        self.spn_rescued_m.blockSignals(True)
        self.spn_pending_m.setValue(float(pending_m))
        self.spn_rescued_m.setValue(float(rescued_m))
        self.spn_pending_m.blockSignals(False)
        self.spn_rescued_m.blockSignals(False)

    def _refresh_table(self):
        filter_value = self.cmb_filter.currentData()
        rows = []
        summary = {'detected': 0, 'pending': 0, 'rescued': 0}

        for record in self._records:
            status = str(record.get('status', 'detected'))
            if status in summary:
                summary[status] += 1
            if filter_value not in (None, 'all') and status != filter_value:
                continue
            rows.append(record)

        self.lbl_summary.setText(
            f"{summary['detected']} detectadas | {summary['pending']} pendientes | {summary['rescued']} rescatadas"
        )

        self.tbl.setRowCount(len(rows))
        for row_index, record in enumerate(rows):
            id_item = QTableWidgetItem(str(int(record.get('id', 0))))
            status = str(record.get('status', 'detected'))
            status_item = QTableWidgetItem(STATUS_LABELS.get(status, status))
            source_item = QTableWidgetItem(str(record.get('status_source', 'auto')).capitalize())
            assigned_robot_item = QTableWidgetItem(str(record.get('assigned_robot', '') or '-'))
            distance_m = record.get('distance_m', None)
            if distance_m is None:
                distance_item = QTableWidgetItem('-')
            else:
                distance_item = QTableWidgetItem(f"{float(distance_m):.1f}")
            color = STATUS_COLORS.get(status)
            if color is not None:
                status_item.setForeground(color)
            lat_item = QTableWidgetItem(f"{float(record.get('lat', 0.0)):.7f}")
            lon_item = QTableWidgetItem(f"{float(record.get('lon', 0.0)):.7f}")

            id_item.setTextAlignment(Qt.AlignCenter)
            status_item.setTextAlignment(Qt.AlignCenter)
            source_item.setTextAlignment(Qt.AlignCenter)
            assigned_robot_item.setTextAlignment(Qt.AlignCenter)
            distance_item.setTextAlignment(Qt.AlignCenter)
            lat_item.setTextAlignment(Qt.AlignCenter)
            lon_item.setTextAlignment(Qt.AlignCenter)

            self.tbl.setItem(row_index, 0, id_item)
            self.tbl.setItem(row_index, 1, status_item)
            self.tbl.setItem(row_index, 2, source_item)
            self.tbl.setItem(row_index, 3, assigned_robot_item)
            self.tbl.setItem(row_index, 4, distance_item)
            self.tbl.setItem(row_index, 5, lat_item)
            self.tbl.setItem(row_index, 6, lon_item)

        self.tbl.resizeColumnsToContents()