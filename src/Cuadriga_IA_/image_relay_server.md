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
- `rgb_overlay_topic=/fanet/segmentation/overlay`
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
  -p rgb_overlay_topic:=/fanet/segmentation/overlay \
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

## Notas

- El relay consume ROS 2 en la red local y expone HTTP simple; no expone ROS 2 hacia fuera.
- El stream MJPEG esta pensado para visualizacion remota con baja latencia, no para transporte cientifico sin perdida.
- Para overlays termicos reales basta con cambiar `thermal_overlay_topic` al topic correcto cuando exista.