# GUI remota de Cuadriga por VPN

Este documento describe como usar la GUI de Cuadriga desde tu ordenador mientras el robot sigue ejecutandose en su propia red local. La idea es separar dos planos:

- ROS 2 y DDS para telemetria, estados y detecciones.
- HTTP para las imagenes del tab Video, usando `image_relay_server` en el robot.

Con este flujo, la GUI corre en tu portatil y las imagenes se piden al robot por la IP de ZeroTier o de la VPN.

## Arquitectura

- En el robot corre el stack ROS 2 normal.
- En el robot corre `run_image_relay_server.sh` y expone HTTP en el puerto `8080`.
- En tu ordenador corre `run_remote_gui_docker.sh`.
- La GUI usa DDS para los topics ROS 2 y `IMAGE_RELAY_BASE_URL` para descargar snapshots HTTP.

## Requisitos

En el robot:

- Docker funcionando.
- Stack de Cuadriga levantado o al menos los nodos que publican los topics necesarios.
- Conectividad ZeroTier o VPN activa.
- Acceso a la red local del robot para los topics internos.

En tu ordenador:

- Docker funcionando.
- Acceso grafico X11 disponible (`DISPLAY` y `XAUTHORITY` validos).
- Conectividad a la misma red ZeroTier o VPN que el robot.
- Acceso DDS por ZeroTier o, si estas tambien en la LAN del robot, por ambas interfaces.

## Topics esperados

El relay HTTP usa por defecto estos topics:

- `thermal_topic=/fanet/raw/thermal`
- `thermal_overlay_topic=/fanet/raw/rgb`
- `rgb_overlay_topic=/fanet/gui/rgb_annotated`
- `rgb_original_topic=/zed/zed_node/left/image_rect_color`

La GUI remota usa esos streams HTTP:

- `/snapshot/thermal`
- `/snapshot/thermal_overlay`
- `/snapshot/rgb_overlay`
- `/snapshot/rgb_original`

## Paso 1: arrancar el relay en el robot

En el robot:

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
export LOCAL_DDS_IP=192.168.2.115
bash run_image_relay_server.sh
```

Ejemplo de salida esperada:

```text
[relay-dds] ZeroTier IP: 10.142.47.160
[relay-dds] Local DDS IP: 192.168.2.115
[INFO] [image_relay_server]: relay thermal <- /fanet/raw/thermal [Image]
[INFO] [image_relay_server]: relay thermal_overlay <- /fanet/raw/rgb [Image]
[INFO] [image_relay_server]: relay rgb_overlay <- /fanet/gui/rgb_annotated [Image]
[INFO] [image_relay_server]: relay rgb_original <- /zed/zed_node/left/image_rect_color [Image]
[INFO] [image_relay_server]: HTTP relay listening on http://0.0.0.0:8080
```

Si quieres ejecutar el relay en otro puerto:

```bash
export IMAGE_RELAY_PORT=8081
bash run_image_relay_server.sh
```

## Paso 2: comprobar que el relay responde desde tu ordenador

Desde tu ordenador, usa la IP ZeroTier del robot:

```bash
curl http://10.142.47.160:8080/health
```

Si todo va bien deberias recibir un JSON con el estado de los streams.

Tambien puedes probar una imagen concreta:

```bash
curl -I http://10.142.47.160:8080/snapshot/rgb_overlay
curl -I http://10.142.47.160:8080/snapshot/thermal
```

Si responde `503`, el relay esta vivo pero todavia no ha recibido frames para ese stream.

## Paso 3: arrancar la GUI remota en tu ordenador

En tu ordenador:

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
export ROBOT_ZT_IP=10.142.47.160
bash run_remote_gui_docker.sh
```

El script hace esto:

- Detecta tu IP local de ZeroTier.
- Genera un XML DDS dual a partir de `local_cyclonedds_vpn_lan.xml`.
- Pasa `IMAGE_RELAY_BASE_URL=http://10.142.47.160:8080` al contenedor.
- Arranca solo la GUI, sin lanzar el stack completo del robot.

Si tu ordenador tambien esta conectado a la LAN del robot, puedes anunciar esa interfaz adicional:

```bash
export LOCAL_DDS_IP=192.168.2.120
export ROBOT_ZT_IP=10.142.47.160
bash run_remote_gui_docker.sh
```

Si quieres pasar la URL completa manualmente:

```bash
export IMAGE_RELAY_BASE_URL=http://10.142.47.160:8080
bash run_remote_gui_docker.sh
```

## Paso 4: comprobar la GUI

Dentro de la GUI:

- El tab `Video` debe mostrar `ZED RGB original`, `RGB anotada`, `Termica anotada` y `Termica original`.
- El tab `Personas` debe seguir actualizando el numero de detecciones.
- En el estado del video deberias ver mensajes del tipo `relay 120 ms seq 34`.

## Variables de entorno utiles

- `ROBOT_ZT_IP`: IP ZeroTier del robot. Ejemplo `10.142.47.160`.
- `IMAGE_RELAY_BASE_URL`: URL completa del relay. Ejemplo `http://10.142.47.160:8080`.
- `IMAGE_RELAY_PORT`: puerto HTTP del relay. Por defecto `8080`.
- `LOCAL_DDS_IP`: IP de tu ordenador en la LAN del robot. Opcional para la GUI remota.
- `ZT_IP`: fuerza manualmente la IP de ZeroTier local si la autodeteccion falla.
- `ZT_INTERFACE`: nombre de la interfaz ZeroTier. Por defecto `ztpp6e24bc`.
- `ROS_DOMAIN_ID`: dominio ROS 2. Por defecto `0`.

## Comandos alternativos

Si quieres seguir usando el lanzador grande del robot con GUI dentro del mismo contenedor, tambien puedes hacerlo, pero no es el flujo recomendado para pruebas remotas:

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
export IMAGE_RELAY_BASE_URL=http://10.142.47.160:8080
export USE_GUI=true
bash run_robot_docker_dual_dds.sh
```

Ese modo levanta el stack del robot y la GUI juntos. Para uso remoto normalmente interesa mas `run_remote_gui_docker.sh`.

## Resolucion de problemas

### La GUI abre pero no se ven imagenes

Comprueba primero el relay:

```bash
curl http://10.142.47.160:8080/health
```

Si `curl` no responde:

- El relay no esta arrancado.
- La IP ZeroTier del robot no es la correcta.
- Hay un bloqueo de red o firewall en el puerto `8080`.

Si `curl` responde pero las imagenes siguen sin verse:

- Revisa que los topics configurados por defecto existan realmente en el robot.
- Si el endpoint responde `503`, faltan frames en ese stream.
- Asegurate de que la GUI este usando la IP del robot y no `127.0.0.1`.

### Veo ROS 2 pero no Video

Eso indica normalmente que DDS funciona pero HTTP no.

Comprueba:

- `curl http://10.142.47.160:8080/health`
- `curl -I http://10.142.47.160:8080/snapshot/rgb_overlay`

### Veo Video pero no detecciones ROS

Eso indica normalmente que HTTP funciona pero DDS no.

Comprueba:

- Que tu ordenador esta en la misma red ZeroTier que el robot.
- Que `ROS_DOMAIN_ID` coincide.
- Que la interfaz ZeroTier detectada por el script es la correcta.
- Que, si hace falta, has exportado `ZT_IP` y `ZT_INTERFACE` manualmente.

### `run_remote_gui_docker.sh` falla al detectar la IP ZeroTier

Fuerza la IP a mano:

```bash
export ZT_IP=10.142.47.23
export ROBOT_ZT_IP=10.142.47.160
bash run_remote_gui_docker.sh
```

Si la interfaz no se llama `ztpp6e24bc`, indica su nombre real:

```bash
export ZT_INTERFACE=ztxxxxxxxx
bash run_remote_gui_docker.sh
```

### La GUI no abre por X11

Comprueba:

- Que `DISPLAY` existe en tu sesion.
- Que `XAUTHORITY` apunta a un fichero valido.
- Que tu servidor X permite conexiones del contenedor.

Ejemplo:

```bash
echo "$DISPLAY"
echo "$XAUTHORITY"
```

## Flujo recomendado

1. Arranca el stack ROS 2 del robot en su red local.
2. Arranca el relay en el robot con `run_image_relay_server.sh`.
3. Desde tu ordenador, verifica `curl http://ROBOT_ZT_IP:8080/health`.
4. Arranca la GUI con `run_remote_gui_docker.sh`.
5. Si falla Video, depura HTTP.
6. Si fallan detecciones/telemetria, depura DDS.

## Archivos relacionados

- `run_image_relay_server.sh`
- `run_remote_gui_docker.sh`
- `run_robot_docker_dual_dds.sh`
- `image_relay_server.md`
- `local_cyclonedds_vpn_lan.xml`
