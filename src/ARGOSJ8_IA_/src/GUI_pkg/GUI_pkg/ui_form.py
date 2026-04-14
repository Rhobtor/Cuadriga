# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'formTtNluR.ui'
##
## Created by: Qt User Interface Compiler version 5.15.3
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################
from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QLabel, QMainWindow,
    QPushButton, QSizePolicy, QSpacerItem, QStatusBar,
    QTextEdit, QVBoxLayout, QWidget, QComboBox)
import resources_rc

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1400, 820)
        MainWindow.setMinimumSize(QSize(1400, 820))
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.horizontalLayout_2 = QHBoxLayout(self.centralwidget)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_2)

        self.map_ = QLabel(self.centralwidget)
        self.map_.setObjectName(u"map_")
        self.map_.setMinimumSize(QSize(980, 780))
        self.map_.setMaximumSize(QSize(980, 780))
        self.map_.setPixmap(QPixmap(u":/resources/map.png"))
        self.map_.setScaledContents(True)

        self.horizontalLayout_2.addWidget(self.map_)

        self.horizontalSpacer_3 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_3)

        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(-1, -1, -1, 0)
        self.verticalSpacer_2 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer_2)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(-1, 0, 0, -1)
        self.verticalLayout_2 = QVBoxLayout()
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.start_record_path = QPushButton(self.centralwidget)
        self.start_record_path.setObjectName(u"start_record_path")

        self.verticalLayout_2.addWidget(self.start_record_path)

        self.stop_record_path = QPushButton(self.centralwidget)
        self.stop_record_path.setObjectName(u"stop_record_path")

        self.verticalLayout_2.addWidget(self.stop_record_path)

        self.toggle_view = QPushButton(self.centralwidget)
        self.toggle_view.setObjectName(u"toggle_view")

        self.verticalLayout_2.addWidget(self.toggle_view)


        self.horizontalLayout.addLayout(self.verticalLayout_2)

        self.Estop = QPushButton(self.centralwidget)
        self.Estop.setObjectName(u"Estop")
        self.Estop.setMinimumSize(QSize(0, 85))
        self.Estop.setStyleSheet(u"background-color: red")

        self.horizontalLayout.addWidget(self.Estop)


        self.verticalLayout.addLayout(self.horizontalLayout)

        self.verticalLayout_3 = QVBoxLayout()
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(-1, -1, -1, 0)
        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.horizontalLayout_3.setContentsMargins(-1, 0, -1, 0)
        self.base_gps_2 = QLabel(self.centralwidget)
        self.base_gps_2.setObjectName(u"base_gps_2")

        self.horizontalLayout_3.addWidget(self.base_gps_2)

        self.base_gps = QLabel(self.centralwidget)
        self.base_gps.setObjectName(u"base_gps")

        self.horizontalLayout_3.addWidget(self.base_gps)

        self.horizontalLayout_3.setStretch(0, 2)
        self.horizontalLayout_3.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_3)

        self.horizontalLayout_8 = QHBoxLayout()
        self.horizontalLayout_8.setObjectName(u"horizontalLayout_8")
        self.rover_gps_2 = QLabel(self.centralwidget)
        self.rover_gps_2.setObjectName(u"rover_gps_2")

        self.horizontalLayout_8.addWidget(self.rover_gps_2)

        self.rover_gps = QLabel(self.centralwidget)
        self.rover_gps.setObjectName(u"rover_gps")

        self.horizontalLayout_8.addWidget(self.rover_gps)

        self.horizontalLayout_8.setStretch(0, 2)
        self.horizontalLayout_8.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_8)

        self.horizontalLayout_9 = QHBoxLayout()
        self.horizontalLayout_9.setObjectName(u"horizontalLayout_9")
        self.robot_latitude_2 = QLabel(self.centralwidget)
        self.robot_latitude_2.setObjectName(u"robot_latitude_2")

        self.horizontalLayout_9.addWidget(self.robot_latitude_2)

        self.robot_latitude = QLabel(self.centralwidget)
        self.robot_latitude.setObjectName(u"robot_latitude")

        self.horizontalLayout_9.addWidget(self.robot_latitude)

        self.horizontalLayout_9.setStretch(0, 2)
        self.horizontalLayout_9.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_9)

        self.horizontalLayout_10 = QHBoxLayout()
        self.horizontalLayout_10.setObjectName(u"horizontalLayout_10")
        self.robot_longitude_2 = QLabel(self.centralwidget)
        self.robot_longitude_2.setObjectName(u"robot_longitude_2")

        self.horizontalLayout_10.addWidget(self.robot_longitude_2)

        self.robot_longitude = QLabel(self.centralwidget)
        self.robot_longitude.setObjectName(u"robot_longitude")

        self.horizontalLayout_10.addWidget(self.robot_longitude)

        self.horizontalLayout_10.setStretch(0, 2)
        self.horizontalLayout_10.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_10)

        self.horizontalLayout_11 = QHBoxLayout()
        self.horizontalLayout_11.setObjectName(u"horizontalLayout_11")
        self.robot_heading_2 = QLabel(self.centralwidget)
        self.robot_heading_2.setObjectName(u"robot_heading_2")

        self.horizontalLayout_11.addWidget(self.robot_heading_2)

        self.robot_heading = QLabel(self.centralwidget)
        self.robot_heading.setObjectName(u"robot_heading")

        self.horizontalLayout_11.addWidget(self.robot_heading)

        self.horizontalLayout_11.setStretch(0, 2)
        self.horizontalLayout_11.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_11)

        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.security_check = QPushButton(self.centralwidget)
        self.security_check.setObjectName(u"security_check")
        self.change_security = QComboBox(self.centralwidget)
        self.change_security.addItem("Enable")
        self.change_security.addItem("Disable")
        self.security_check.setObjectName(u"change_security")
        self.horizontalLayout_4.addWidget(self.security_check)
        self.horizontalLayout_4.addWidget(self.change_security)
        self.security_status = QLabel(self.centralwidget)
        self.security_status.setObjectName(u"security_status")
        self.horizontalLayout_4.addWidget(self.security_status)

        self.status = QLabel(self.centralwidget)
        self.status.setObjectName(u"status")

        self.horizontalLayout_4.addWidget(self.status)


        self.verticalLayout_3.addLayout(self.horizontalLayout_4)

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.sendTransition = QPushButton(self.centralwidget)
        self.sendTransition.setObjectName(u"sendTransition")

        self.horizontalLayout_5.addWidget(self.sendTransition)

        self.fsm_transition = QComboBox(self.centralwidget)
        self.fsm_transition.setObjectName(u"fsm_transition")
        self.fsm_transition.setMaximumSize(QSize(16777215, 25))
        self.fsm_transition.setAcceptDrops(True)
        #self.fsm_transition.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        #self.fsm_transition.setUndoRedoEnabled(True)
        #self.fsm_transition.setAcceptRichText(True)
        #self.fsm_transition.setTextInteractionFlags(Qt.TextEditorInteraction)

        self.horizontalLayout_5.addWidget(self.fsm_transition)

        self.horizontalLayout_5.setStretch(0, 1)
        self.horizontalLayout_5.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_5)

        self.horizontalLayout_12 = QHBoxLayout()
        self.horizontalLayout_12.setObjectName(u"horizontalLayout_12")
        self.horizontalLayout_12.setContentsMargins(-1, 0, -1, -1)
        self.lc_mission_state_2 = QLabel(self.centralwidget)
        self.lc_mission_state_2.setObjectName(u"lc_mission_state_2")

        self.horizontalLayout_12.addWidget(self.lc_mission_state_2)

        self.lc_mission_state = QLabel(self.centralwidget)
        self.lc_mission_state.setObjectName(u"lc_mission_state")

        self.horizontalLayout_12.addWidget(self.lc_mission_state)

        self.horizontalLayout_12.setStretch(0, 2)
        self.horizontalLayout_12.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_12)

        self.horizontalLayout_13 = QHBoxLayout()
        self.horizontalLayout_13.setObjectName(u"horizontalLayout_13")
        self.horizontalLayout_13.setContentsMargins(-1, 0, -1, -1)
        self.fsm_mission_state_2 = QLabel(self.centralwidget)
        self.fsm_mission_state_2.setObjectName(u"fsm_mission_state_2")

        self.horizontalLayout_13.addWidget(self.fsm_mission_state_2)

        self.fsm_mission_state = QLabel(self.centralwidget)
        self.fsm_mission_state.setObjectName(u"fsm_mission_state")

        self.horizontalLayout_13.addWidget(self.fsm_mission_state)

        self.horizontalLayout_13.setStretch(0, 2)
        self.horizontalLayout_13.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_13)

        self.horizontalLayout_19 = QHBoxLayout()
        self.horizontalLayout_19.setObjectName(u"horizontalLayout_19")
        self.horizontalLayout_19.setContentsMargins(-1, -1, -1, 0)
        self.lc_pltform_state_2 = QLabel(self.centralwidget)
        self.lc_pltform_state_2.setObjectName(u"lc_pltform_state_2")

        self.horizontalLayout_19.addWidget(self.lc_pltform_state_2)

        self.lc_pltform_state = QLabel(self.centralwidget)
        self.lc_pltform_state.setObjectName(u"lc_pltform_state")

        self.horizontalLayout_19.addWidget(self.lc_pltform_state)

        self.horizontalLayout_19.setStretch(0, 2)
        self.horizontalLayout_19.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_19)

        self.horizontalLayout_23 = QHBoxLayout()
        self.horizontalLayout_23.setObjectName(u"horizontalLayout_23")
        self.horizontalLayout_23.setContentsMargins(-1, -1, -1, 0)
        self.vehicleMode = QLabel(self.centralwidget)
        self.vehicleMode.setObjectName(u"vehicleMode")

        self.horizontalLayout_23.addWidget(self.vehicleMode)

        self.vehicleMode_val = QLabel(self.centralwidget)
        self.vehicleMode_val.setObjectName(u"vehicleMode_val")

        self.horizontalLayout_23.addWidget(self.vehicleMode_val)


        self.verticalLayout_3.addLayout(self.horizontalLayout_23)

        self.horizontalLayout_14 = QHBoxLayout()
        self.horizontalLayout_14.setObjectName(u"horizontalLayout_14")
        self.horizontalLayout_14.setContentsMargins(-1, 0, -1, -1)
        self.zoom_in = QPushButton(self.centralwidget)
        self.zoom_in.setObjectName(u"zoom_in")

        self.horizontalLayout_14.addWidget(self.zoom_in)

        self.zoom_out = QPushButton(self.centralwidget)
        self.zoom_out.setObjectName(u"zoom_out")

        self.horizontalLayout_14.addWidget(self.zoom_out)


        self.verticalLayout_3.addLayout(self.horizontalLayout_14)

        self.horizontalLayout_15 = QHBoxLayout()
        self.horizontalLayout_15.setObjectName(u"horizontalLayout_15")
        self.horizontalLayout_15.setContentsMargins(-1, 0, -1, -1)
        self.get_robot_path = QPushButton(self.centralwidget)
        self.get_robot_path.setObjectName(u"get_robot_path")

        self.horizontalLayout_15.addWidget(self.get_robot_path)

        self.clear_robot_path = QPushButton(self.centralwidget)
        self.clear_robot_path.setObjectName(u"clear_robot_path")

        self.horizontalLayout_15.addWidget(self.clear_robot_path)


        self.verticalLayout_3.addLayout(self.horizontalLayout_15)

        self.horizontalLayout_16 = QHBoxLayout()
        self.horizontalLayout_16.setObjectName(u"horizontalLayout_16")
        self.horizontalLayout_16.setContentsMargins(-1, 0, -1, -1)
        self.write_path_file = QPushButton(self.centralwidget)
        self.write_path_file.setObjectName(u"write_path_file")

        self.horizontalLayout_16.addWidget(self.write_path_file)

        self.filename_to_write = QTextEdit(self.centralwidget)
        self.filename_to_write.setObjectName(u"filename_to_write")
        self.filename_to_write.setMaximumSize(QSize(16777215, 25))
        self.filename_to_write.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.horizontalLayout_16.addWidget(self.filename_to_write)

        self.horizontalLayout_16.setStretch(0, 1)
        self.horizontalLayout_16.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_16)

        self.horizontalLayout_17 = QHBoxLayout()
        self.horizontalLayout_17.setObjectName(u"horizontalLayout_17")
        self.horizontalLayout_17.setContentsMargins(-1, 0, -1, -1)
        self.read_path_file = QPushButton(self.centralwidget)
        self.read_path_file.setObjectName(u"read_path_file")

        self.horizontalLayout_17.addWidget(self.read_path_file)

        self.filename_to_read = QTextEdit(self.centralwidget)
        self.filename_to_read.setObjectName(u"filename_to_read")
        self.filename_to_read.setMaximumSize(QSize(16777215, 25))
        self.filename_to_read.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.horizontalLayout_17.addWidget(self.filename_to_read)

        self.horizontalLayout_17.setStretch(0, 1)
        self.horizontalLayout_17.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_17)

        self.horizontalLayout_18 = QHBoxLayout()
        self.horizontalLayout_18.setObjectName(u"horizontalLayout_18")
        self.horizontalLayout_18.setContentsMargins(-1, 0, -1, -1)
        self.record_live_pos = QPushButton(self.centralwidget)
        self.record_live_pos.setObjectName(u"record_live_pos")
        self.record_live_pos.setStyleSheet(u"background-color: red")

        self.horizontalLayout_18.addWidget(self.record_live_pos)

        self.clear_live_pos = QPushButton(self.centralwidget)
        self.clear_live_pos.setObjectName(u"clear_live_pos")

        self.horizontalLayout_18.addWidget(self.clear_live_pos)


        self.verticalLayout_3.addLayout(self.horizontalLayout_18)

        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.horizontalLayout_6.setContentsMargins(-1, 0, -1, -1)
        self.K_ang = QLabel(self.centralwidget)
        self.K_ang.setObjectName(u"K_ang")

        self.horizontalLayout_6.addWidget(self.K_ang)

        self.K_ang_val = QTextEdit(self.centralwidget)
        self.K_ang_val.setObjectName(u"K_ang_val")
        self.K_ang_val.setMaximumSize(QSize(16777215, 25))
        self.K_ang_val.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        default_k_ang_val = 1.0  # Replace with your desired default number
        self.K_ang_val.setText(str(default_k_ang_val))
        self.horizontalLayout_6.addWidget(self.K_ang_val)

        self.horizontalLayout_6.setStretch(0, 1)
        self.horizontalLayout_6.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_6)

        self.horizontalLayout_7 = QHBoxLayout()
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.horizontalLayout_7.setContentsMargins(-1, 0, -1, -1)
        self.k_position = QLabel(self.centralwidget)
        self.k_position.setObjectName(u"k_position")

        self.horizontalLayout_7.addWidget(self.k_position)

        self.k_position_val = QTextEdit(self.centralwidget)
        self.k_position_val.setObjectName(u"k_position_val")
        self.k_position_val.setMaximumSize(QSize(16777215, 25))
        self.k_position_val.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        default_k_position_val = 0.2  # Replace with your desired default number
        self.k_position_val.setText(str(default_k_position_val))
        self.horizontalLayout_7.addWidget(self.k_position_val)

        self.horizontalLayout_7.setStretch(0, 1)
        self.horizontalLayout_7.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_7)

        self.horizontalLayout_20 = QHBoxLayout()
        self.horizontalLayout_20.setObjectName(u"horizontalLayout_20")
        self.horizontalLayout_20.setContentsMargins(-1, 0, -1, -1)
        self.look_ahead_dist = QLabel(self.centralwidget)
        self.look_ahead_dist.setObjectName(u"look_ahead_dist")

        self.horizontalLayout_20.addWidget(self.look_ahead_dist)

        self.textEdit_6 = QTextEdit(self.centralwidget)
        self.textEdit_6.setObjectName(u"textEdit_6")
        self.textEdit_6.setMaximumSize(QSize(16777215, 25))
        self.textEdit_6.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        default_look_ahead_dist = 1.0  # Replace with your desired default number
        self.textEdit_6.setText(str(default_look_ahead_dist))
        self.horizontalLayout_20.addWidget(self.textEdit_6)

        self.horizontalLayout_20.setStretch(0, 1)
        self.horizontalLayout_20.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_20)

        self.horizontalLayout_24 = QHBoxLayout()
        self.horizontalLayout_24.setObjectName(u"horizontalLayout_24")
        self.horizontalLayout_24.setContentsMargins(-1, -1, -1, 0)
        self.v_forward = QLabel(self.centralwidget)
        self.v_forward.setObjectName(u"v_forward")

        self.horizontalLayout_24.addWidget(self.v_forward)

        self.v_forward_val = QTextEdit(self.centralwidget)
        self.v_forward_val.setObjectName(u"v_forward_val")
        self.v_forward_val.setMaximumSize(QSize(16777215, 25))
        self.v_forward_val.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.v_forward_val.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        default_v_forward_val = 1.0  # Replace with your desired default number
        self.v_forward_val.setText(str(default_look_ahead_dist))
        self.horizontalLayout_24.addWidget(self.v_forward_val)

        self.horizontalLayout_24.setStretch(0, 1)
        self.horizontalLayout_24.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_24)

        self.horizontalLayout_21 = QHBoxLayout()
        self.horizontalLayout_21.setObjectName(u"horizontalLayout_21")
        self.horizontalLayout_21.setContentsMargins(-1, 0, -1, -1)
        self.points_window = QLabel(self.centralwidget)
        self.points_window.setObjectName(u"points_window")

        self.horizontalLayout_21.addWidget(self.points_window)

        self.points_window_val = QTextEdit(self.centralwidget)
        self.points_window_val.setObjectName(u"points_window_val")
        self.points_window_val.setMaximumSize(QSize(16777215, 25))
        self.points_window_val.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        default_points_window_val = 40  # Replace with your desired default number
        self.points_window_val.setText(str(default_points_window_val))
        self.horizontalLayout_21.addWidget(self.points_window_val)

        self.horizontalLayout_21.setStretch(0, 1)
        self.horizontalLayout_21.setStretch(1, 1)

        self.verticalLayout_3.addLayout(self.horizontalLayout_21)

        self.horizontalLayout_22 = QHBoxLayout()
        self.horizontalLayout_22.setObjectName(u"horizontalLayout_22")
        self.horizontalLayout_22.setContentsMargins(-1, 0, -1, -1)
        self.send_ctl_congif = QPushButton(self.centralwidget)
        self.send_ctl_congif.setObjectName(u"send_ctl_congif")

        self.horizontalLayout_22.addWidget(self.send_ctl_congif)

        self.quit_btn = QPushButton(self.centralwidget)
        self.quit_btn.setObjectName(u"quit_btn")

        self.horizontalLayout_22.addWidget(self.quit_btn)


        self.verticalLayout_3.addLayout(self.horizontalLayout_22)


        self.verticalLayout.addLayout(self.verticalLayout_3)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)


        self.horizontalLayout_2.addLayout(self.verticalLayout)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer)

        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.send_ctl_congif.pressed.connect(MainWindow.send_config_ctl)
        self.quit_btn.pressed.connect(MainWindow.quit_app)
        self.clear_live_pos.pressed.connect(MainWindow.del_live_pos)
        self.record_live_pos.pressed.connect(MainWindow.save_live_pos_callback)
        self.write_path_file.pressed.connect(MainWindow.path_file_write)
        self.read_path_file.pressed.connect(MainWindow.path_file_read)
        self.start_record_path.pressed.connect(MainWindow.start_recording)
        self.stop_record_path.pressed.connect(MainWindow.stop_recording)
        self.Estop.pressed.connect(MainWindow.emergency_stop)
        self.security_check.pressed.connect(MainWindow.toggle_security_check)
        self.change_security.currentIndexChanged.connect(MainWindow.on_security_changed)
        self.zoom_in.pressed.connect(MainWindow.zoom_in_callback)
        self.zoom_out.pressed.connect(MainWindow.zoom_out)
        self.get_robot_path.pressed.connect(MainWindow.get_path)
        self.clear_robot_path.pressed.connect(MainWindow.clear_path)
        self.sendTransition.pressed.connect(MainWindow.send_transition)
        self.toggle_view.pressed.connect(MainWindow.toggle_view_)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.map_.setText("")
        self.start_record_path.setText(QCoreApplication.translate("MainWindow", u"Start record path", None))
        self.stop_record_path.setText(QCoreApplication.translate("MainWindow", u"Stop record path", None))
        self.toggle_view.setText(QCoreApplication.translate("MainWindow", u"Toggle View", None))
        self.Estop.setText(QCoreApplication.translate("MainWindow", u"E-Stop", None))
        self.base_gps_2.setText(QCoreApplication.translate("MainWindow", u"Base GPS Covariance:", None))
        self.base_gps.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.rover_gps_2.setText(QCoreApplication.translate("MainWindow", u"Rover GPS Covariance:", None))
        self.rover_gps.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.robot_latitude_2.setText(QCoreApplication.translate("MainWindow", u"Robot Latitude:", None))
        self.robot_latitude.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.robot_longitude_2.setText(QCoreApplication.translate("MainWindow", u"Robot Longitude: ", None))
        self.robot_longitude.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.robot_heading_2.setText(QCoreApplication.translate("MainWindow", u"Robot Heading:", None))
        self.robot_heading.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.security_check.setText(QCoreApplication.translate("MainWindow", u"Change security ", None))
        self.security_status.setText(QCoreApplication.translate("MainWindow", u"Status: Enabled ", None))

        self.status.setText("")
        self.sendTransition.setText(QCoreApplication.translate("MainWindow", u"Change Mode", None))
        self.lc_mission_state_2.setText(QCoreApplication.translate("MainWindow", u"LC Ctl Mission State:", None))
        self.lc_mission_state.setText("")
        self.fsm_mission_state_2.setText(QCoreApplication.translate("MainWindow", u"FSM Ctl Mission State:", None))
        self.fsm_mission_state.setText("")
        self.lc_pltform_state_2.setText(QCoreApplication.translate("MainWindow", u"LC Ctl Plataform State:", None))
        self.lc_pltform_state.setText("")
        self.vehicleMode.setText(QCoreApplication.translate("MainWindow", u"Vehicle Mode:", None))
        self.vehicleMode_val.setText("")
        self.zoom_in.setText(QCoreApplication.translate("MainWindow", u"Zoom In", None))
        self.zoom_out.setText(QCoreApplication.translate("MainWindow", u"Zoom Out", None))
        self.get_robot_path.setText(QCoreApplication.translate("MainWindow", u"Get Robot Path", None))
        self.clear_robot_path.setText(QCoreApplication.translate("MainWindow", u"Clear Robot Path", None))
        self.write_path_file.setText(QCoreApplication.translate("MainWindow", u"Write path to file", None))
        self.read_path_file.setText(QCoreApplication.translate("MainWindow", u"Read path from file", None))
        self.record_live_pos.setText(QCoreApplication.translate("MainWindow", u"Rec Live Position", None))
        self.clear_live_pos.setText(QCoreApplication.translate("MainWindow", u"Clear live position", None))
        self.K_ang.setText(QCoreApplication.translate("MainWindow", u"K_ang", None))
        self.k_position.setText(QCoreApplication.translate("MainWindow", u"k_position", None))
        self.look_ahead_dist.setText(QCoreApplication.translate("MainWindow", u"look_ahead_dist", None))
        self.v_forward.setText(QCoreApplication.translate("MainWindow", u"v_forward", None))
        self.points_window.setText(QCoreApplication.translate("MainWindow", u"n_windows_point", None))
        self.send_ctl_congif.setText(QCoreApplication.translate("MainWindow", u"Send Ctl Config", None))
        self.quit_btn.setText(QCoreApplication.translate("MainWindow", u"Quit App", None))
    # retranslateUi

