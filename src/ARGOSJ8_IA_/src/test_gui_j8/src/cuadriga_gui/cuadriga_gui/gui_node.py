import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QTimer
from cuadriga_gui.app.main_window import MainWindow
import os

# 1) limpia proxies heredados que están mal formados
for k in ['http_proxy','https_proxy','HTTP_PROXY','HTTPS_PROXY',
          'all_proxy','ALL_PROXY','no_proxy','NO_PROXY']:
    os.environ.pop(k, None)

# 2) fuerza a Chromium (Qt WebEngine) a NO usar proxy
os.environ['QTWEBENGINE_CHROMIUM_FLAGS'] = (
    os.environ.get('QTWEBENGINE_CHROMIUM_FLAGS','') + ' --no-proxy-server'
)



def main():
    app = QApplication(sys.argv)


    # Creamos la ventana, diferimos el arranque ROS para no bloquear la GUI
    w = MainWindow(defer_ros_start=True)
    w.show()


    # Arranca ROS tras pintar la ventana
    QTimer.singleShot(0, w.start_ros)


    sys.exit(app.exec())




if __name__ == '__main__':
    main()