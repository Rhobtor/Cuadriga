import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga/install/gpx_loader'
