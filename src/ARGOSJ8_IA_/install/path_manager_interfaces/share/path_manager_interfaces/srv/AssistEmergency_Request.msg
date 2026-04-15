## AssistEmergency.srv
##
## Servicio para generar una ruta de asistencia/emergencia desde start -> goal.
## En la implementación actual puede devolver una ruta "simple" (línea recta)
## convertida a lat/lon, o usar un planificador más complejo si se habilita.
##
## Request:
##   - start: punto inicial (coordenadas según implementación)
##   - goal: punto objetivo
## Response:
##   - success: true si se generó ruta
##   - path: nav_msgs/Path resultante

geometry_msgs/Point start
geometry_msgs/Point goal
