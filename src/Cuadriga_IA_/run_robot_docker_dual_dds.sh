#!/usr/bin/env bash

set -euo pipefail

IMAGE_NAME="${CUADRIGA_IMAGE:-cuadriga-cuadriga}"
SERIAL_DEVICE="${SERIAL_DEVICE:-/dev/ttyUSB0}"
FIXPOSITION_IP="${FIXPOSITION_IP:-192.168.2.113}"
FIXPOSITION_PORT="${FIXPOSITION_PORT:-21000}"
USE_GUI="${USE_GUI:-false}"
XAUTH_FILE="${XAUTHORITY:-${HOME}/.Xauthority}"
ZT_INTERFACE="${ZT_INTERFACE:-ztpp6e24bc}"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_XML="${ROOT_DIR}/local_cyclonedds_vpn_lan.xml"
GENERATED_XML="$(mktemp /tmp/cuadriga_dual_dds.XXXXXX.xml)"

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

if ! command -v docker >/dev/null 2>&1; then
  echo "docker is not installed or not in PATH" >&2
  exit 1
fi

if [[ ! -e "${SERIAL_DEVICE}" ]]; then
  echo "serial device not found: ${SERIAL_DEVICE}" >&2
  exit 1
fi

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

echo "[dual-dds] ZeroTier IP: ${ZT_IP}"
echo "[dual-dds] Local DDS IP: ${LOCAL_DDS_IP}"
echo "[dual-dds] Generated DDS XML: ${GENERATED_XML}"

docker_args=(
  run --rm -it
  --net=host
  --ipc=host
  --device="${SERIAL_DEVICE}:${SERIAL_DEVICE}"
  -e "ROS_DOMAIN_ID=${ROS_DOMAIN_ID:-0}"
  -e "ROS_LOCALHOST_ONLY=0"
  -e "RMW_IMPLEMENTATION=rmw_cyclonedds_cpp"
  -e "CYCLONEDDS_URI=file:///dds/local_cyclonedds_vpn_lan.xml"
  -v "${GENERATED_XML}:/dds/local_cyclonedds_vpn_lan.xml:ro"
)

if [[ -n "${DISPLAY:-}" ]]; then
  docker_args+=( -e "DISPLAY=${DISPLAY}" )

  if [[ -S /tmp/.X11-unix/X0 || -d /tmp/.X11-unix ]]; then
    docker_args+=( -v /tmp/.X11-unix:/tmp/.X11-unix )
  fi

  if [[ -f "${XAUTH_FILE}" ]]; then
    docker_args+=( -e "XAUTHORITY=${XAUTH_FILE}" -v "${XAUTH_FILE}:${XAUTH_FILE}:ro" )
  else
    echo "warning: XAUTHORITY file not found at ${XAUTH_FILE}; GUI apps may fail to open" >&2
  fi
fi

exec docker "${docker_args[@]}" \
  "${IMAGE_NAME}" \
  ros2 launch cuadriga_setup cuadriga_launch.py \
    robot:=true \
    use_gui:="${USE_GUI}" \
    fixposition_ip:="${FIXPOSITION_IP}" \
    fixposition_port:="${FIXPOSITION_PORT}"