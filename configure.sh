#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

cmake -S "$ROOT_DIR" \
      -B "$ROOT_DIR/build" \
      -DCMAKE_PREFIX_PATH="$HOME/Qt/6.11.1/gcc_64;$ROOT_DIR/third_party/install"

cmake --build "$ROOT_DIR/build" -j"$(nproc)"
