#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/.." && pwd)"
CONFIG_FILE="${VISUAL_LAB_CONFIG:-${SCRIPT_DIR}/visual_lab_config.json}"

if [ ! -f "$CONFIG_FILE" ]; then
  echo "config file not found: $CONFIG_FILE" >&2
  exit 1
fi

# Load env vars from config before launching any lab.
set -a
source "${SCRIPT_DIR}/load_visual_lab_config.sh"
set +a

if [ "$#" -lt 1 ]; then
  echo "Usage: source ${SCRIPT_DIR}/run_lab.sh <lab1|lab2|lab3|lab4|lab5|lab6|lab7>" >&2
  echo "       or: bash ${SCRIPT_DIR}/run_lab.sh lab1" >&2
  exit 1
fi

LAB_NAME="${1,,}"
case "$LAB_NAME" in
  lab1|lab2|lab3|lab4|lab5|lab6|lab7)
    ;;
  *)
    echo "Unknown lab: $1" >&2
    echo "Available: lab1 lab2 lab3 lab4 lab5 lab6 lab7" >&2
    exit 1
    ;;
 esac

LAB_DIR="${REPO_ROOT}/${LAB_NAME}"
if [ ! -d "$LAB_DIR" ]; then
  echo "lab directory not found: $LAB_DIR" >&2
  exit 1
fi

BINARY="${LAB_DIR}/${LAB_NAME}"
if [ ! -x "$BINARY" ]; then
  echo "Binary not found: $BINARY"
  echo "Building ${LAB_NAME} first..."
  (cd "$LAB_DIR" && qmake "${LAB_NAME}.pro" >/dev/null 2>&1 || true)
  (cd "$LAB_DIR" && make -j"$(nproc 2>/dev/null || echo 2)" )
fi

if [ ! -x "$BINARY" ]; then
  echo "Failed to build or locate executable: $BINARY" >&2
  exit 1
fi

cd "$LAB_DIR"
export VISUAL_LAB_CONFIG="$CONFIG_FILE"
exec "./${LAB_NAME}"
