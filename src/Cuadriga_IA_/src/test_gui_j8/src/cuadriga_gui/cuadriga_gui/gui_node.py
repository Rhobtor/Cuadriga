import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QTimer
from cuadriga_gui.app.main_window import MainWindow
import os

DEFAULT_CYCLONEDDS_URI = 'file:///etc/cyclonedds/local_cyclonedds.xml'

# 1) limpia proxies heredados que están mal formados
for k in ['http_proxy','https_proxy','HTTP_PROXY','HTTPS_PROXY',
          'all_proxy','ALL_PROXY','no_proxy','NO_PROXY']:
    os.environ.pop(k, None)

# 2) fuerza a Chromium (Qt WebEngine) a NO usar proxy
os.environ['QTWEBENGINE_CHROMIUM_FLAGS'] = (
    os.environ.get('QTWEBENGINE_CHROMIUM_FLAGS','') + ' --no-proxy-server'
)


def _sanitize_dds_environment():
    os.environ.setdefault('RMW_IMPLEMENTATION', 'rmw_cyclonedds_cpp')

    requested_uri = os.environ.get('CYCLONEDDS_URI', '').strip()
    default_path = DEFAULT_CYCLONEDDS_URI.removeprefix('file://')

    if requested_uri.startswith('file://'):
        requested_path = requested_uri.removeprefix('file://')
        if os.path.isfile(requested_path):
            return

        if os.path.isfile(default_path):
            print(
                f'[GUI] DDS config no disponible en {requested_path}; '
                f'se usara {DEFAULT_CYCLONEDDS_URI}'
            )
            os.environ['CYCLONEDDS_URI'] = DEFAULT_CYCLONEDDS_URI
        return

    if not requested_uri and os.path.isfile(default_path):
        os.environ['CYCLONEDDS_URI'] = DEFAULT_CYCLONEDDS_URI



def main():
    _sanitize_dds_environment()
    app = QApplication(sys.argv)


    # Creamos la ventana, diferimos el arranque ROS para no bloquear la GUI
    w = MainWindow(defer_ros_start=True)
    w.show()


    # Arranca ROS tras pintar la ventana
    QTimer.singleShot(0, w.start_ros)


    sys.exit(app.exec())




if __name__ == '__main__':
    main()