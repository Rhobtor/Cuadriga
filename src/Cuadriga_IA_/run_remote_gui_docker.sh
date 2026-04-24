#!/usr/bin/env bash

set -euo pipefail

IMAGE_NAME="${CUADRIGA_IMAGE:-cuadriga-cuadriga}"
ZT_INTERFACE="${ZT_INTERFACE:-ztpp6e24bc}"
XAUTH_FILE="${XAUTHORITY:-${HOME}/.Xauthority}"
IMAGE_RELAY_PORT="${IMAGE_RELAY_PORT:-8080}"
IMAGE_RELAY_HOST="${IMAGE_RELAY_HOST:-${ROBOT_ZT_IP:-}}"
IMAGE_RELAY_BASE_URL="${IMAGE_RELAY_BASE_URL:-}"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_XML="${ROOT_DIR}/local_cyclonedds_vpn_lan.xml"
GENERATED_XML="$(mktemp /tmp/cuadriga_gui_dual_dds.XXXXXX.xml)"

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

if [[ ! -f "${TEMPLATE_XML}" ]]; then
  echo "DDS template not found: ${TEMPLATE_XML}" >&2
  exit 1
fi

ZT_IP="$(pick_zt_ip)"
if [[ -z "${ZT_IP}" ]]; then
  echo "could not detect ZeroTier IPv4 on ${ZT_INTERFACE}; export ZT_IP manually" >&2
  exit 1
fi

LOCAL_DDS_IP="${LOCAL_DDS_IP:-${ZT_IP}}"

if [[ -z "${IMAGE_RELAY_BASE_URL}" ]]; then
  : "${IMAGE_RELAY_HOST:?Export IMAGE_RELAY_HOST or ROBOT_ZT_IP with the robot VPN IP, e.g. 10.142.47.160}"
  IMAGE_RELAY_BASE_URL="http://${IMAGE_RELAY_HOST}:${IMAGE_RELAY_PORT}"
fi

sed \
  -e "s|\${ZT_IP}|${ZT_IP}|g" \
  -e "s|\${LOCAL_DDS_IP}|${LOCAL_DDS_IP}|g" \
  "${TEMPLATE_XML}" > "${GENERATED_XML}"

echo "[remote-gui] ZeroTier IP local: ${ZT_IP}"
echo "[remote-gui] Local DDS IP: ${LOCAL_DDS_IP}"
echo "[remote-gui] Relay HTTP remoto: ${IMAGE_RELAY_BASE_URL}"
echo "[remote-gui] Generated DDS XML: ${GENERATED_XML}"

docker_args=(
  run --rm -it
  --net=host
  --ipc=host
  -e "ROS_DOMAIN_ID=${ROS_DOMAIN_ID:-0}"
  -e "ROS_LOCALHOST_ONLY=0"
  -e "RMW_IMPLEMENTATION=rmw_cyclonedds_cpp"
  -e "CYCLONEDDS_URI=file:///dds/local_cyclonedds_vpn_lan.xml"
  -e "IMAGE_RELAY_BASE_URL=${IMAGE_RELAY_BASE_URL}"
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
  /bin/bash -lc "source /opt/ros/humble/setup.bash && source /ros2_ws/install/setup.bash && ros2 run cuadriga_gui gui"