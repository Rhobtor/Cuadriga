from __future__ import annotations

from typing import Iterable

from PySide6.QtCore import Qt, Signal
from PySide6.QtGui import QColor
from PySide6.QtWidgets import (
    QComboBox,
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
}

STATUS_COLORS = {
    'detected': QColor('#dc2626'),
}


class PersonsWidget(QWidget):
    assignmentRequested = Signal(int, str)

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
        self.cmb_filter.currentIndexChanged.connect(self._refresh_table)
        controls.addWidget(self.cmb_filter)

        self.lbl_summary = QLabel('0 detectadas')
        controls.addWidget(self.lbl_summary, 1)
        root.addLayout(controls)

        assign_row = QHBoxLayout()
        self.lbl_current_robot = QLabel('Robot actual: cuadriga')
        assign_row.addWidget(self.lbl_current_robot)
        self.lbl_current_role = QLabel('Rol: explorador')
        assign_row.addWidget(self.lbl_current_role)
        assign_row.addStretch(1)
        root.addLayout(assign_row)

        self.tbl = QTableWidget(0, 9)
        self.tbl.setHorizontalHeaderLabels(['ID', 'Estado', 'Origen', 'Robot', 'Dist (m)', 'X fwd', 'Y left', 'Lat', 'Lon'])
        self.tbl.setSelectionBehavior(QTableWidget.SelectRows)
        self.tbl.setSelectionMode(QTableWidget.SingleSelection)
        self.tbl.setEditTriggers(QTableWidget.NoEditTriggers)
        root.addWidget(self.tbl)

        actions = QHBoxLayout()
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

    def _emit_assignment_current_robot(self):
        self._emit_assignment(self._current_robot)

    def _emit_assignment(self, robot_name: str):
        person_id = self._selected_person_id()
        if person_id is None:
            return
        self.assignmentRequested.emit(person_id, str(robot_name or '').strip())

    def set_current_robot(self, robot_name: str):
        self._current_robot = str(robot_name or '').strip() or 'cuadriga'
        self.lbl_current_robot.setText(f'Robot actual: {self._current_robot}')

    def set_current_role(self, role_name: str):
        self._current_role = str(role_name or '').strip() or 'explorador'
        self.lbl_current_role.setText(f'Rol: {self._current_role}')

    def _refresh_table(self):
        filter_value = self.cmb_filter.currentData()
        rows = []
        detected_count = 0

        for record in self._records:
            if filter_value not in (None, 'all'):
                continue
            detected_count += 1
            rows.append(record)

        self.lbl_summary.setText(f'{detected_count} detectadas')

        self.tbl.setRowCount(len(rows))
        for row_index, record in enumerate(rows):
            id_item = QTableWidgetItem(str(int(record.get('id', 0))))
            status = 'detected'
            status_item = QTableWidgetItem(STATUS_LABELS.get(status, status))
            source_value = str(record.get('origin', 'auto'))
            source_item = QTableWidgetItem(source_value.capitalize())
            assigned_robot_item = QTableWidgetItem(str(record.get('assigned_robot', '') or '-'))
            distance_m = record.get('distance_m', None)
            if distance_m is None:
                distance_item = QTableWidgetItem('-')
            else:
                distance_item = QTableWidgetItem(f"{float(distance_m):.1f}")
            local_x = record.get('local_x', None)
            local_y = record.get('local_y', None)
            local_x_item = QTableWidgetItem('-' if local_x is None else f"{float(local_x):.2f}")
            local_y_item = QTableWidgetItem('-' if local_y is None else f"{float(local_y):.2f}")
            color = STATUS_COLORS.get(status)
            if color is not None:
                status_item.setForeground(color)
            lat_value = record.get('lat', None)
            lon_value = record.get('lon', None)
            lat_item = QTableWidgetItem('-' if lat_value is None else f"{float(lat_value):.7f}")
            lon_item = QTableWidgetItem('-' if lon_value is None else f"{float(lon_value):.7f}")

            id_item.setTextAlignment(Qt.AlignCenter)
            status_item.setTextAlignment(Qt.AlignCenter)
            source_item.setTextAlignment(Qt.AlignCenter)
            assigned_robot_item.setTextAlignment(Qt.AlignCenter)
            distance_item.setTextAlignment(Qt.AlignCenter)
            local_x_item.setTextAlignment(Qt.AlignCenter)
            local_y_item.setTextAlignment(Qt.AlignCenter)
            lat_item.setTextAlignment(Qt.AlignCenter)
            lon_item.setTextAlignment(Qt.AlignCenter)

            self.tbl.setItem(row_index, 0, id_item)
            self.tbl.setItem(row_index, 1, status_item)
            self.tbl.setItem(row_index, 2, source_item)
            self.tbl.setItem(row_index, 3, assigned_robot_item)
            self.tbl.setItem(row_index, 4, distance_item)
            self.tbl.setItem(row_index, 5, local_x_item)
            self.tbl.setItem(row_index, 6, local_y_item)
            self.tbl.setItem(row_index, 7, lat_item)
            self.tbl.setItem(row_index, 8, lon_item)

        self.tbl.resizeColumnsToContents()