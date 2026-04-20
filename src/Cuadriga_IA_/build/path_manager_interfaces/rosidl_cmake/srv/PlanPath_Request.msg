## PlanPath.srv
##
## Solicita una planificación de ruta desde un punto de inicio a un punto objetivo.
##
## Importante: la semántica de los puntos depende de la implementación. En el
## código actual se interpretan como coordenadas tipo UTM (x/y) y luego se
## convierten a lat/lon para devolver un nav_msgs/Path.
##
## Request:
##   - start: punto inicio (x,y,z)
##   - goal:  punto objetivo (x,y,z)
## Response:
##   - ack:  indica que se aceptó la petición.
##   - path: ruta planificada.

geometry_msgs/Point start
geometry_msgs/Point goal
