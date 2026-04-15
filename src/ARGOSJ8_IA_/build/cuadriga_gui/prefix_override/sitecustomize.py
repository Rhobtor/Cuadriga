import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/rhobtor/PHD/Cuadriga_j8/src/Cuadriga/src/ARGOSJ8_IA_/install/cuadriga_gui'
