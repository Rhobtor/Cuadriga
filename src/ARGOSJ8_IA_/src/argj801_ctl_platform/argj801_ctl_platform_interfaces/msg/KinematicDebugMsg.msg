## KinematicDebugMsg.msg
##
## Estado de depuracion del modelo cinemático usado para generar
## el mando low-level a partir de cmd_vel.
##
## Todos los campos escalarizados de throttle/steering se publican en las
## mismas unidades que CmdThrottleMsg. Las aceleraciones solicitadas se
## expresan antes de aplicar la saturacion configurada.

float32 throttle
float32 steering
float32 requested_throttle_acc
float32 requested_steering_acc
bool throttle_limited
bool steering_limited