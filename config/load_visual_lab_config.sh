#!/usr/bin/env bash

set -a

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_FILE="${VISUAL_LAB_CONFIG:-${SCRIPT_DIR}/visual_lab_config.json}"

if [ ! -f "$CONFIG_FILE" ]; then
  echo "visual lab config not found: $CONFIG_FILE" >&2
  return 1 2>/dev/null || exit 1
fi

python3 - "$CONFIG_FILE" <<'PY'
import json
import os
import sys

config_path = sys.argv[1]

with open(config_path, 'r', encoding='utf-8') as fh:
    data = json.load(fh)

# Export only fields that the existing labs already understand.
exports = {
    'VISUAL_LAB_CAMERA_INDEX': 'camera_index',
    'VISUAL_LAB_CAMERA_DEVICE': 'camera_device',
    'VISUAL_LAB_CAMERA_BACKEND': 'camera_backend',
    'VISUAL_LAB_FRAME_WIDTH': 'frame_width',
    'VISUAL_LAB_FRAME_HEIGHT': 'frame_height',
    'VISUAL_LAB_FRAME_IMAGE': 'frame_image',
    'VISUAL_LAB_YUNET_ONNX': 'yunet_onnx',
    'VISUAL_LAB_YUNET_ENGINE': 'yunet_engine',
}

for env_name, config_key in exports.items():
    value = data.get(config_key, '')
    if value is None:
        value = ''
    if isinstance(value, bool):
        value = '1' if value else '0'
    if value == '':
        os.environ.pop(env_name, None)
    else:
        os.environ[env_name] = str(value)

os.environ['VISUAL_LAB_CONFIG'] = config_path
PY

set +a

printf 'Loaded visual lab config from %s\n' "$CONFIG_FILE"
