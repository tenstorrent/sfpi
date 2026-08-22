#!/usr/bin/env bash
# pin-install-fast.sh — install a GATE-PROVEN gcc build into the shared sfpi
# toolchain in minutes, no rebuild.  Owner-ratified 2026-08-20: the trust
# anchor is the sha256 of the installed binaries (recorded + re-verified),
# never the build path.  The build you install MUST already have passed the
# union gates (DejaGnu frozen-set, corpus flags-off byte-identity, CRAQ) —
# this script verifies and installs, it does not judge.
#
# Usage:
#   pin-install-fast.sh <gcc-build-dir> <install-root> [--expect-cc1plus <sha256>] [--flags <comma-list>] [--dry-run]
#     <gcc-build-dir>  e.g. ~/sfpi-uplift/gcc-build-pin14merge  (needs gcc/{cc1plus,cc1,lto1,xg++,xgcc})
#     <install-root>   e.g. ~/sfpi-uplift/sfpi/build/sfpi/compiler
#     --expect-cc1plus verify the build's cc1plus matches this sha BEFORE installing (ties install to the gated binary)
#     --flags          smoke-test each flag is accepted post-install (e.g. int-abs,repr-prop; prefix -mtt-tensix-optimize- added)
#     --dry-run        report what would happen, change nothing
#
# Behavior: backs up every file it replaces to <install-root>/.pin-backup-<oldsha8>/,
# installs cc1plus + cc1 + lto1 + drivers (xg++->riscv-tt-elf-g++, xgcc->riscv-tt-elf-gcc)
# TOGETHER (installing cc1plus alone breaks new .opt flags — the driver embeds
# option tables), verifies shas + flag acceptance + a default compile, writes
# PIN-INSTALL-MANIFEST.txt, and ROLLS BACK EVERYTHING on any failure.
set -u

die_rollback() {
  echo "FAIL: $1" >&2
  if [ -n "${BK:-}" ] && [ -d "$BK" ] && [ "${DRY:-0}" = 0 ]; then
    echo "ROLLING BACK from $BK" >&2
    for f in "$BK"/*; do b=$(basename "$f"); rm -f "${RESTORE[$b]}" && cp "$f" "${RESTORE[$b]}"; done
    echo "rollback complete — install unchanged" >&2
  fi
  exit 1
}

BUILD=${1:?gcc-build-dir}; INSTALL=${2:?install-root}; shift 2
EXPECT=""; FLAGS=""; DRY=0
while [ $# -gt 0 ]; do case "$1" in
  --expect-cc1plus)
    # A missing/empty/malformed sha must ERROR LOUDLY: an empty value used
    # to silently disable the verify gate ([ -n "$EXPECT" ] below), and a
    # missing value shifted the NEXT option into EXPECT (arg-shift misparse).
    if [ $# -lt 2 ] || ! printf '%s' "$2" | grep -qE '^[0-9a-f]{64}$'; then
      echo "--expect-cc1plus needs a full 64-hex lowercase sha256 (got: '${2:-<missing>}') — refusing (an empty/malformed sha would skip the verify gate)" >&2
      exit 2
    fi
    EXPECT=$2; shift 2;;
  --flags)
    if [ $# -lt 2 ] || [ -z "$2" ] || [ "${2#--}" != "$2" ]; then
      echo "--flags needs a non-empty comma-list value (got: '${2:-<missing>}')" >&2
      exit 2
    fi
    FLAGS=$2; shift 2;;
  --dry-run) DRY=1; shift;;
  *) echo "unknown arg $1" >&2; exit 2;;
esac; done

G=$BUILD/gcc
LIBEXEC=$INSTALL/libexec/gcc/riscv-tt-elf/15.1.0
BIN=$INSTALL/bin
for p in "$G/cc1plus" "$G/cc1" "$G/lto1" "$G/xg++" "$G/xgcc" "$LIBEXEC" "$BIN"; do
  [ -e "$p" ] || { echo "missing: $p" >&2; exit 2; }
done

NEWSHA=$(sha256sum "$G/cc1plus" | cut -d' ' -f1)
[ -n "$EXPECT" ] && [ "$NEWSHA" != "$EXPECT" ] && { echo "cc1plus sha $NEWSHA != expected $EXPECT — refusing (wrong build?)" >&2; exit 3; }
OLDSHA=$(sha256sum "$LIBEXEC/cc1plus" | cut -d' ' -f1)
echo "install: cc1plus $OLDSHA -> $NEWSHA"
[ "$DRY" = 1 ] && { echo "dry-run: would install cc1plus,cc1,lto1 + drivers, then verify"; exit 0; }

# single-writer guard: refuse if anything is actively compiling with the install
if pgrep -f "$LIBEXEC/cc1plus" >/dev/null 2>&1; then echo "live compiles using this install — refusing (never swap under a live compile)" >&2; exit 4; fi

BK=$INSTALL/.pin-backup-${OLDSHA:0:8}; mkdir -p "$BK"
declare -A RESTORE
backup_install() { # src dst name
  RESTORE[$3]=$2
  cp -f "$2" "$BK/$3" || die_rollback "backup of $3 failed"
  # rm-then-cp, NEVER cp -f in place: cp -f opens the dest for write, so a
  # dest hardlinked into cp -al hybrid toolchains would be corrupted THROUGH
  # the link (the 2026-08-17 laneBH shared-cc1plus incident class).  rm
  # breaks the link first; the hybrids keep their old inode.
  { rm -f "$2" && cp "$1" "$2"; } || die_rollback "install of $3 failed"
}
backup_install "$G/cc1plus" "$LIBEXEC/cc1plus" cc1plus
backup_install "$G/cc1"     "$LIBEXEC/cc1"     cc1
backup_install "$G/lto1"    "$LIBEXEC/lto1"    lto1
backup_install "$G/xg++"    "$BIN/riscv-tt-elf-g++" g++
backup_install "$G/xgcc"    "$BIN/riscv-tt-elf-gcc" gcc

# verify: sha, flag acceptance, default compile
[ "$(sha256sum "$LIBEXEC/cc1plus" | cut -d' ' -f1)" = "$NEWSHA" ] || die_rollback "post-install sha mismatch"
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
echo 'int main(){return 0;}' > "$T/t.c"
"$BIN/riscv-tt-elf-g++" -O2 -c "$T/t.c" -o "$T/t.o" 2>"$T/err" || die_rollback "default compile broken: $(head -1 "$T/err")"
if [ -n "$FLAGS" ]; then
  for f in ${FLAGS//,/ }; do
    "$BIN/riscv-tt-elf-g++" "-mtt-tensix-optimize-$f" -c "$T/t.c" -o /dev/null 2>"$T/err"
    grep -q unrecognized "$T/err" && die_rollback "flag -mtt-tensix-optimize-$f REJECTED (driver/option-table mismatch)"
  done
fi

M=$INSTALL/PIN-INSTALL-MANIFEST.txt
{ echo "== pin-install-fast $(date -u +%Y-%m-%dT%H:%M:%SZ) =="
  echo "build-dir: $BUILD"
  echo "prev cc1plus: $OLDSHA (backup: $BK)"
  for n in cc1plus cc1 lto1; do echo "$n: $(sha256sum "$LIBEXEC/$n" | cut -d' ' -f1)"; done
  echo "g++: $(sha256sum "$BIN/riscv-tt-elf-g++" | cut -d' ' -f1)"
  echo "gcc: $(sha256sum "$BIN/riscv-tt-elf-gcc" | cut -d' ' -f1)"
  echo "flags-verified: ${FLAGS:-none}"
} >> "$M"
echo "OK — installed + verified; manifest appended to $M"
