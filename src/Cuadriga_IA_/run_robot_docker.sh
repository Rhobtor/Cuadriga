#!/usr/bin/env bash

set -euo pipefail

IMAGE_NAME="${CUADRIGA_IMAGE:-cuadriga-cuadriga}"
SERIAL_DEVICE="${SERIAL_DEVICE:-/dev/ttyUSB0}"
FIXPOSITION_IP="${FIXPOSITION_IP:-192.168.2.113}"
FIXPOSITION_PORT="${FIXPOSITION_PORT:-21000}"
USE_GUI="${USE_GUI:-false}"
XAUTH_FILE="${XAUTHORITY:-${HOME}/.Xauthority}"

if ! command -v docker >/dev/null 2>&1; then
  echo "docker is not installed or not in PATH" >&2
  exit 1
fi

if [[ ! -e "${SERIAL_DEVICE}" ]]; then
  echo "serial device not found: ${SERIAL_DEVICE}" >&2
  exit 1
fi

docker_args=(
  run --rm -it
  --net=host
  --ipc=host
  --device="${SERIAL_DEVICE}:${SERIAL_DEVICE}"
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