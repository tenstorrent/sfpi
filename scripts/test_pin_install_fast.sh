#!/usr/bin/env bash
# Focused, filesystem-only regression tests for pin-install-fast.sh.
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PIN_INSTALL=$SCRIPT_DIR/pin-install-fast.sh
ROOT=$(mktemp -d)
trap 'rm -rf "$ROOT"' EXIT

fail() {
  echo "FAIL: $*" >&2
  exit 1
}

sha() {
  sha256sum "$1" | cut -d' ' -f1
}

write_driver() {
  local path=$1 label=$2
  {
    echo '#!/usr/bin/env bash'
    printf '# fixture %s\n' "$label"
    cat <<'EOF'
if [ -n "${FAKE_DRIVER_FAIL_MATCH:-}" ]; then
  for arg in "$@"; do
    case "$arg" in
      *"$FAKE_DRIVER_FAIL_MATCH"*)
        echo "fixture requested flag compile failure" >&2
        exit 23
        ;;
    esac
  done
fi
exit 0
EOF
  } > "$path"
  chmod +x "$path"
}

make_build() {
  local root=$1 label=$2
  mkdir -p "$root/gcc"
  printf '%s\n' "$label-cc1plus" > "$root/gcc/cc1plus"
  printf '%s\n' "$label-cc1" > "$root/gcc/cc1"
  printf '%s\n' "$label-lto1" > "$root/gcc/lto1"
  write_driver "$root/gcc/xg++" "$label-gxx"
  write_driver "$root/gcc/xgcc" "$label-gcc"
}

make_install() {
  local root=$1 label=$2
  local libexec=$root/libexec/gcc/riscv-tt-elf/15.1.0
  mkdir -p "$libexec" "$root/bin"
  printf '%s\n' "$label-cc1plus" > "$libexec/cc1plus"
  printf '%s\n' "$label-cc1" > "$libexec/cc1"
  printf '%s\n' "$label-lto1" > "$libexec/lto1"
  write_driver "$root/bin/riscv-tt-elf-g++" "$label-gxx"
  write_driver "$root/bin/riscv-tt-elf-gcc" "$label-gcc"
}

expected_args() {
  local build=$1
  printf '%s\n' \
    --expect-cc1plus "$(sha "$build/gcc/cc1plus")" \
    --expect-cc1 "$(sha "$build/gcc/cc1")" \
    --expect-lto1 "$(sha "$build/gcc/lto1")" \
    --expect-gxx "$(sha "$build/gcc/xg++")" \
    --expect-gcc "$(sha "$build/gcc/xgcc")"
}

assert_install_matches() {
  local build=$1 install=$2
  [ "$(sha "$build/gcc/cc1plus")" = "$(sha "$install/libexec/gcc/riscv-tt-elf/15.1.0/cc1plus")" ] || fail "cc1plus mismatch"
  [ "$(sha "$build/gcc/cc1")" = "$(sha "$install/libexec/gcc/riscv-tt-elf/15.1.0/cc1")" ] || fail "cc1 mismatch"
  [ "$(sha "$build/gcc/lto1")" = "$(sha "$install/libexec/gcc/riscv-tt-elf/15.1.0/lto1")" ] || fail "lto1 mismatch"
  [ "$(sha "$build/gcc/xg++")" = "$(sha "$install/bin/riscv-tt-elf-g++")" ] || fail "g++ mismatch"
  [ "$(sha "$build/gcc/xgcc")" = "$(sha "$install/bin/riscv-tt-elf-gcc")" ] || fail "gcc mismatch"
}

test_full_hash_pin_and_manifest() {
  local build=$ROOT/full-build install=$ROOT/full-install log=$ROOT/full.log
  local args=()
  make_build "$build" full-new
  make_install "$install" full-old
  mapfile -t args < <(expected_args "$build")
  "$PIN_INSTALL" "$build" "$install" "${args[@]}" --flags accepted >"$log" 2>&1
  assert_install_matches "$build" "$install"
  grep -q "expected-cc1: $(sha "$build/gcc/cc1")" "$install/PIN-INSTALL-MANIFEST.txt" || fail "manifest omitted expected cc1"
  grep -q "expected-lto1: $(sha "$build/gcc/lto1")" "$install/PIN-INSTALL-MANIFEST.txt" || fail "manifest omitted expected lto1"
  grep -q "expected-g++: $(sha "$build/gcc/xg++")" "$install/PIN-INSTALL-MANIFEST.txt" || fail "manifest omitted expected g++"
  grep -q "expected-gcc: $(sha "$build/gcc/xgcc")" "$install/PIN-INSTALL-MANIFEST.txt" || fail "manifest omitted expected gcc"
  echo "PASS full companion hash pins and manifest"
}

test_companion_mismatch_refuses_before_install() {
  local build=$ROOT/mismatch-build install=$ROOT/mismatch-install log=$ROOT/mismatch.log
  local old_cc1
  make_build "$build" mismatch-new
  make_install "$install" mismatch-old
  old_cc1=$(sha "$install/libexec/gcc/riscv-tt-elf/15.1.0/cc1")
  if "$PIN_INSTALL" "$build" "$install" \
      --expect-cc1plus "$(sha "$build/gcc/cc1plus")" \
      --expect-cc1 0000000000000000000000000000000000000000000000000000000000000000 \
      >"$log" 2>&1; then
    fail "companion mismatch unexpectedly installed"
  fi
  [ "$old_cc1" = "$(sha "$install/libexec/gcc/riscv-tt-elf/15.1.0/cc1")" ] || fail "mismatch changed install"
  [ ! -e "$install/PIN-INSTALL-MANIFEST.txt" ] || fail "mismatch wrote manifest"
  grep -q 'cc1 sha .* != expected' "$log" || fail "mismatch lacked precise diagnostic"
  echo "PASS companion mismatch refuses before mutation"
}

test_flag_compile_failure_rolls_back_every_binary() {
  local build=$ROOT/rollback-build install=$ROOT/rollback-install log=$ROOT/rollback.log
  local before=$ROOT/before.sha after=$ROOT/after.sha
  local args=()
  make_build "$build" rollback-new
  make_install "$install" rollback-old
  mapfile -t args < <(expected_args "$build")
  find "$install/bin" "$install/libexec" -type f -print0 | sort -z | xargs -0 sha256sum > "$before"
  if FAKE_DRIVER_FAIL_MATCH=optimize-broken \
      "$PIN_INSTALL" "$build" "$install" "${args[@]}" --flags broken \
      >"$log" 2>&1; then
    fail "flag compile failure unexpectedly succeeded"
  fi
  find "$install/bin" "$install/libexec" -type f -print0 | sort -z | xargs -0 sha256sum > "$after"
  cmp -s "$before" "$after" || fail "rollback did not restore every binary"
  [ ! -e "$install/PIN-INSTALL-MANIFEST.txt" ] || fail "failed smoke wrote manifest"
  grep -q 'flag -mtt-tensix-optimize-broken compile failed' "$log" || fail "flag failure lacked diagnostic"
  grep -q 'rollback complete' "$log" || fail "flag failure did not report rollback"
  echo "PASS flag compile failure rolls back every binary"
}

test_backup_names_are_collision_safe() {
  local build=$ROOT/collision-build install=$ROOT/collision-install
  local args=() prefix count
  make_build "$build" collision-new
  make_install "$install" collision-old
  mapfile -t args < <(expected_args "$build")
  "$PIN_INSTALL" "$build" "$install" "${args[@]}" >/dev/null
  prefix=$(sha "$build/gcc/cc1plus")
  prefix=${prefix:0:8}
  "$PIN_INSTALL" "$build" "$install" "${args[@]}" >/dev/null
  "$PIN_INSTALL" "$build" "$install" "${args[@]}" >/dev/null
  count=$(find "$install" -maxdepth 1 -type d -name ".pin-backup-$prefix.*" | wc -l)
  [ "$count" -eq 2 ] || fail "expected two unique same-prefix backups, found $count"
  echo "PASS backup directories are collision-safe"
}

test_legacy_cli_remains_accepted() {
  local build=$ROOT/legacy-build install=$ROOT/legacy-install
  make_build "$build" legacy-new
  make_install "$install" legacy-old
  "$PIN_INSTALL" "$build" "$install" \
    --expect-cc1plus "$(sha "$build/gcc/cc1plus")" --dry-run >/dev/null
  echo "PASS legacy cc1plus-only CLI remains accepted"
}

test_full_hash_pin_and_manifest
test_companion_mismatch_refuses_before_install
test_flag_compile_failure_rolls_back_every_binary
test_backup_names_are_collision_safe
test_legacy_cli_remains_accepted
echo "PASS all pin-install-fast selftests"
