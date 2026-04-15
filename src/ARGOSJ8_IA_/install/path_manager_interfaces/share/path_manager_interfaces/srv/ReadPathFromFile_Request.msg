## ReadPathFromFile.srv
##
## Solicita cargar una ruta desde un fichero en disco.
##
## - filename: nombre del fichero (puede ser relativo a una carpeta de rutas del
##   paquete o absoluto, según implementación).
##
## Response:
## - success: true si el fichero se encontró y se cargó.

string filename
