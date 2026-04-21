#!/usr/bin/env bash
set -euo pipefail

# Auxiliary listener container for Jetson ROS 2 discovery on the local switch LAN.
# This does NOT modify or replace the main Cuadriga container.
#
# Before using:
# 1. Set LOCAL_DDS_IP to the IPv4 of this PC on the Jetson/switch network.
#    Example: export LOCAL_DDS_IP=192.168.2.120
# 2. Build the image once:
#    docker build -f dockerfile.jetson.cuadriga -t rhobtor/cuadriga_j8:jetson-cuadriga .
#
# Then run this script. Inside the container you can use:
#   source /opt/ros/humble/setup.bash
#   ros2 topic list
#   ros2 topic echo /some/topic

: "${LOCAL_DDS_IP:?Export LOCAL_DDS_IP with this PC IP on the Jetson LAN, e.g. 192.168.2.120}"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

exec docker run --rm -it \
  --name jetson_listener_cuadriga \
  --net=host \
  -e ROS_DOMAIN_ID="${ROS_DOMAIN_ID:-0}" \
  -e ROS_LOCALHOST_ONLY=0 \
  -e RMW_IMPLEMENTATION=rmw_cyclonedds_cpp \
  -e LOCAL_DDS_IP="${LOCAL_DDS_IP}" \
  -e CYCLONEDDS_URI=file:///dds/jetson_cyclonedds.xml \
  -v "${ROOT_DIR}/jetson_cyclonedds.xml:/dds/jetson_cyclonedds.xml:ro" \
  -v "${ROOT_DIR}:/workspaces/Cuadriga_IA_:rw" \
  --workdir /workspaces/Cuadriga_IA_ \
  rhobtor/cuadriga_j8:jetson-cuadriga
