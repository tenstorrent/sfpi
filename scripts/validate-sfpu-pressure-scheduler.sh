#!/usr/bin/env bash

# Canonical entry point for the generic SFPU scheduler validation.  Keep the
# historical Welford-named driver as a compatibility implementation while the
# test also exercises a separate, algorithm-agnostic fused arithmetic DFG.

set -euo pipefail
repo=$(cd "$(dirname "$0")/.." && pwd)
exec "$repo/scripts/validate-welford-scheduler.sh" "$@"
