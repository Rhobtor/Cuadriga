#!/usr/bin/env bash

DEFAULT_CYCLONEDDS_URI="file:///etc/cyclonedds/local_cyclonedds.xml"
DEFAULT_CYCLONEDDS_PATH="/etc/cyclonedds/local_cyclonedds.xml"

export RMW_IMPLEMENTATION="${RMW_IMPLEMENTATION:-rmw_cyclonedds_cpp}"

if [[ -n "${CYCLONEDDS_URI:-}" && "${CYCLONEDDS_URI}" == file://* ]]; then
  requested_path="${CYCLONEDDS_URI#file://}"
  if [[ ! -f "${requested_path}" && -f "${DEFAULT_CYCLONEDDS_PATH}" ]]; then
    if [[ -z "${CUADRIGA_DDS_SANITIZER_QUIET:-}" ]]; then
      echo "[shell] DDS config no disponible en ${requested_path}; se usara ${DEFAULT_CYCLONEDDS_URI}" >&2
    fi
    export CYCLONEDDS_URI="${DEFAULT_CYCLONEDDS_URI}"
  fi
elif [[ -z "${CYCLONEDDS_URI:-}" && -f "${DEFAULT_CYCLONEDDS_PATH}" ]]; then
  export CYCLONEDDS_URI="${DEFAULT_CYCLONEDDS_URI}"
fi