#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

cmake --preset local -S "$ROOT_DIR"
cmake --build --preset local-build
