# image_relay_server

Servicio ROS 2 Python para convertir topics `sensor_msgs/msg/Image` en snapshots JPEG y streams MJPEG por HTTP con estrategia `latest frame only`.

## Endpoints

- `/health`
- `/snapshot/thermal`
- `/snapshot/thermal_overlay`
- `/snapshot/rgb_overlay`
- `/stream/thermal`
- `/stream/thermal_overlay`
- `/stream/rgb_overlay`

Si todavia no hay frame disponible en un stream, el endpoint responde `no frame available` con `503`.

## Defaults actuales

- `thermal_topic=/fanet/raw/thermal`
- `thermal_overlay_topic=/fanet/raw/rgb`
- `rgb_overlay_topic=/fanet/gui/rgb_annotated`
- `rgb_original_topic=/zed/zed_node/left/image_rect_color`
- `host=0.0.0.0`
- `port=8080`
- `jpeg_quality=80`
- `resize_width=640`
- `enable_resize=false`
- `log_fps=false`

## Build en el workspace actual

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
source /opt/ros/humble/setup.bash
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install --packages-select image_relay_server
source install/setup.bash
ros2 run image_relay_server relay_server
```

Con parametros ROS 2:

```bash
ros2 run image_relay_server relay_server --ros-args \
  -p thermal_topic:=/fanet/raw/thermal \
  -p thermal_overlay_topic:=/zed/zed_node/rgb/image_rect_color \
  -p rgb_overlay_topic:=/fanet/gui/rgb_annotated \
  -p port:=8080 \
  -p jpeg_quality:=75 \
  -p enable_resize:=true \
  -p resize_width:=800
```

## Docker Humble

Este contenedor usa como base la imagen local `rhobtor/cuadriga_j8:latest` para evitar pulls a Docker Hub y reutilizar el entorno Humble que ya tienes construido.

Build:

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
docker build -f dockerfile.image_relay_server.humble -t image-relay-server:humble .
```

Run:

```bash
docker run --rm -it \
  --net=host \
  -e ROS_DOMAIN_ID=0 \
  -e ROS_LOCALHOST_ONLY=0 \
  image-relay-server:humble \
  /bin/bash -lc "source /opt/ros/humble/setup.bash && python3 -m image_relay_server.relay_server --ros-args -p host:=0.0.0.0 -p port:=8080"
```

O con el helper:

```bash
bash run_image_relay_server.sh
```

## Consumo remoto esperado

- `http://IP_VPN_PC_ROBOT:8080/stream/thermal`
- `http://IP_VPN_PC_ROBOT:8080/stream/thermal_overlay`
- `http://IP_VPN_PC_ROBOT:8080/stream/rgb_overlay`

## GUI remota desde tu portatil

Si el relay corre en el robot y la GUI corre en tu ordenador por VPN, la GUI no debe usar `127.0.0.1`. Debe apuntar a la IP VPN del robot.

1. En el robot, lanza el relay como ya haces:

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
export LOCAL_DDS_IP=192.168.2.115
bash run_image_relay_server.sh
```

2. Desde tu ordenador, comprueba que el relay responde por la VPN:

```bash
curl http://10.142.47.160:8080/health
```

3. En tu ordenador, lanza solo la GUI con DDS dual y el relay remoto:

```bash
cd /home/cuadriga/cuadriga_ws/Cuadriga/src/Cuadriga_IA_
export ROBOT_ZT_IP=10.142.47.160
bash run_remote_gui_docker.sh
```

Si tu portatil tambien esta en la LAN del robot, puedes exportar `LOCAL_DDS_IP` para anunciar esa interfaz adicional. Si no, el script usa automaticamente tu IP local de ZeroTier y sirve igual para pruebas solo por VPN.

Tambien puedes pasar la URL completa si no quieres usar `ROBOT_ZT_IP`:

```bash
export IMAGE_RELAY_BASE_URL=http://10.142.47.160:8080
bash run_remote_gui_docker.sh
```

Si prefieres seguir usando `run_robot_docker_dual_dds.sh` con `USE_GUI=true`, ahora tambien acepta:

```bash
export IMAGE_RELAY_BASE_URL=http://10.142.47.160:8080
export USE_GUI=true
bash run_robot_docker_dual_dds.sh
```

## Notas

- El relay consume ROS 2 en la red local y expone HTTP simple; no expone ROS 2 hacia fuera.
- El stream MJPEG esta pensado para visualizacion remota con baja latencia, no para transporte cientifico sin perdida.
- Para overlays termicos reales basta con cambiar `thermal_overlay_topic` al topic correcto cuando exista.
- `run_remote_gui_docker.sh` es el lanzador recomendado cuando solo quieres la GUI en tu ordenador y el robot sigue corriendo en su propia red.