#!/usr/bin/env bash
set -e

DEFAULT_CYCLONEDDS_URI="file:///etc/cyclonedds/local_cyclonedds.xml"
TEMPLATE_CYCLONEDDS_PATH="/etc/cyclonedds/local_cyclonedds.template.xml"
GENERATED_CYCLONEDDS_PATH="/etc/cyclonedds/local_cyclonedds.xml"

pick_fallback_ipv4() {
  local preferred_interface="$1"
  local candidates
  local candidate
  local lan_candidate=""

  candidates=$(hostname -I 2>/dev/null | tr ' ' '\n' | awk 'NF && $1 ~ /^[0-9]+\./ && $1 != "127.0.0.1" && $1 != "172.17.0.1"')

  if [ -z "$candidates" ]; then
    return 1
  fi

  if [[ "$preferred_interface" == zt* ]]; then
    while read -r candidate; do
      if [[ "$candidate" == 10.* ]]; then
        printf '%s\n' "$candidate"
        return 0
      fi
    done <<< "$candidates"
  fi

  while read -r candidate; do
    if [[ "$candidate" == 192.168.* ]] || [[ "$candidate" == 10.* ]]; then
      lan_candidate="$candidate"
      break
    fi
  done <<< "$candidates"

  if [ -n "$lan_candidate" ]; then
    printf '%s\n' "$lan_candidate"
    return 0
  fi

  printf '%s\n' "$(printf '%s\n' "$candidates" | head -n 1)"
}

export ROS_DOMAIN_ID="${ROS_DOMAIN_ID:-0}"
export RMW_IMPLEMENTATION="${RMW_IMPLEMENTATION:-rmw_cyclonedds_cpp}"

REQUESTED_CYCLONEDDS_URI="${CYCLONEDDS_URI:-$DEFAULT_CYCLONEDDS_URI}"
ZT_INTERFACE="${ZT_INTERFACE:-ztpp6e24bc}"
ZT_WAIT_SECONDS="${ZT_WAIT_SECONDS:-30}"

echo "[entrypoint] CYCLONEDDS_URI solicitado=${REQUESTED_CYCLONEDDS_URI}"

if command -v dbus-daemon >/dev/null 2>&1; then
  mkdir -p /run/dbus
  if [ ! -S /run/dbus/system_bus_socket ]; then
    echo "[entrypoint] iniciando dbus system bus"
    dbus-daemon --system --fork
  fi
fi

USE_REQUESTED_CYCLONEDDS_URI=false
REQUESTED_CYCLONEDDS_PATH=""

if [[ "$REQUESTED_CYCLONEDDS_URI" == file://* ]]; then
  REQUESTED_CYCLONEDDS_PATH="${REQUESTED_CYCLONEDDS_URI#file://}"
fi

if [ -n "$REQUESTED_CYCLONEDDS_PATH" ] && [ -f "$REQUESTED_CYCLONEDDS_PATH" ]; then
  USE_REQUESTED_CYCLONEDDS_URI=true
elif [ "$REQUESTED_CYCLONEDDS_URI" != "$DEFAULT_CYCLONEDDS_URI" ]; then
  echo "[entrypoint] aviso: no existe ${REQUESTED_CYCLONEDDS_PATH} dentro del contenedor; se usara la configuracion interna"
fi

if [ "$USE_REQUESTED_CYCLONEDDS_URI" = true ]; then
  export CYCLONEDDS_URI="$REQUESTED_CYCLONEDDS_URI"
  echo "[entrypoint] usando configuracion DDS externa en ${CYCLONEDDS_URI}"
elif [ -f "$TEMPLATE_CYCLONEDDS_PATH" ]; then
  if [ -z "${ZT_IP:-}" ]; then
    for i in $(seq 1 "$ZT_WAIT_SECONDS"); do
      ZT_IP=$(ip -4 -o addr show dev "$ZT_INTERFACE" 2>/dev/null | awk 'NR==1 {split($4, ip, "/"); print ip[1]}')
      if [ -n "$ZT_IP" ]; then
        break
      fi
      sleep 1
    done
  fi

  if [ -z "${ZT_IP:-}" ]; then
    ZT_IP=$(pick_fallback_ipv4 "$ZT_INTERFACE" || true)
    if [ -n "$ZT_IP" ]; then
      echo "[entrypoint] aviso: no se pudo leer la IPv4 de ${ZT_INTERFACE} con iproute2; se usara la IP detectada por hostname -I: ${ZT_IP}"
    fi
  fi

  if [ -z "${ZT_IP:-}" ]; then
    echo "[entrypoint] ERROR: no se pudo obtener IPv4 de ${ZT_INTERFACE}; exporta ZT_IP manualmente o monta un CYCLONEDDS_URI valido"
    exit 1
  fi

  export ZT_IP
  echo "[entrypoint] ZeroTier IP: ${ZT_IP}"

  envsubst < "$TEMPLATE_CYCLONEDDS_PATH" > "$GENERATED_CYCLONEDDS_PATH"
  export CYCLONEDDS_URI="$DEFAULT_CYCLONEDDS_URI"

  echo "[entrypoint] XML generado:"
  cat "$GENERATED_CYCLONEDDS_PATH"
else
  echo "[entrypoint] ERROR: no existe ${TEMPLATE_CYCLONEDDS_PATH} y no se proporciono un CYCLONEDDS_URI valido"
  exit 1
fi

echo "[entrypoint] CYCLONEDDS_URI efectivo=${CYCLONEDDS_URI}"

source /opt/ros/humble/setup.bash
source /ros2_ws/install/setup.bash

exec "$@"