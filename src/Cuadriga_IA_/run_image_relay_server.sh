#!/usr/bin/env bash

set -euo pipefail

IMAGE_NAME="${IMAGE_RELAY_IMAGE:-image-relay-server:humble}"
HOST_PORT="${IMAGE_RELAY_PORT:-8080}"
ZT_INTERFACE="${ZT_INTERFACE:-ztpp6e24bc}"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_XML="${ROOT_DIR}/local_cyclonedds_vpn_lan.xml"
GENERATED_XML="$(mktemp /tmp/image_relay_dual_dds.XXXXXX.xml)"

cleanup() {
  rm -f "${GENERATED_XML}"
}

trap cleanup EXIT

pick_zt_ip() {
  if [[ -n "${ZT_IP:-}" ]]; then
    printf '%s\n' "${ZT_IP}"
    return 0
  fi

  ip -4 -o addr show dev "${ZT_INTERFACE}" 2>/dev/null | awk 'NR==1 {split($4, ip, "/"); print ip[1]}'
}

if [[ ! -f "${TEMPLATE_XML}" ]]; then
  echo "DDS template not found: ${TEMPLATE_XML}" >&2
  exit 1
fi

: "${LOCAL_DDS_IP:?Export LOCAL_DDS_IP with this PC IP on the Jetson LAN, e.g. 192.168.2.120}"

ZT_IP="$(pick_zt_ip)"
if [[ -z "${ZT_IP}" ]]; then
  echo "could not detect ZeroTier IPv4 on ${ZT_INTERFACE}; export ZT_IP manually" >&2
  exit 1
fi

sed \
  -e "s|\${ZT_IP}|${ZT_IP}|g" \
  -e "s|\${LOCAL_DDS_IP}|${LOCAL_DDS_IP}|g" \
  "${TEMPLATE_XML}" > "${GENERATED_XML}"

echo "[relay-dds] ZeroTier IP: ${ZT_IP}"
echo "[relay-dds] Local DDS IP: ${LOCAL_DDS_IP}"
echo "[relay-dds] Generated DDS XML: ${GENERATED_XML}"

exec docker run --rm -it \
  --net=host \
  --ipc=host \
  -e ROS_DOMAIN_ID="${ROS_DOMAIN_ID:-0}" \
  -e ROS_LOCALHOST_ONLY=0 \
  -e RMW_IMPLEMENTATION=rmw_cyclonedds_cpp \
  -e CYCLONEDDS_URI=file:///dds/local_cyclonedds_vpn_lan.xml \
  -v "${GENERATED_XML}:/dds/local_cyclonedds_vpn_lan.xml:ro" \
  "${IMAGE_NAME}" \
  /bin/bash -lc "source /opt/ros/humble/setup.bash && python3 -m image_relay_server.relay_server --ros-args -p host:=0.0.0.0 -p port:=${HOST_PORT}"