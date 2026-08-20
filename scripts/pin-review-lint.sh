#!/usr/bin/env bash
# pin-review-lint: no bare gcc-pin bumps.
#
# HANDOFF §1(4) / review-wave chronic (waves 5-9): gcc submodule pins were
# repeatedly advanced with zero review record ("bare bumps", e.g. 9db53b60,
# 1f33061d, de00b48e, 5a1cfe6).  This lint makes the rule structural: any
# commit that moves the `gcc` gitlink MUST, in the same commit, carry
#   docs/handoff-20260817/review_records/PIN_REVIEW-<newsha12>.md
# quoting the FULL 40-hex new gcc sha.  The record's substance (what was
# reviewed, by whom, gate evidence) is templated in that directory's
# README; this lint enforces existence + pin match, the same discipline
# tt-metal's sweep preflight enforces for the installed cc1plus.
#
# Usage:
#   scripts/pin-review-lint.sh [<rev-range>]   # default: HEAD^..HEAD
#   scripts/pin-review-lint.sh --selftest      # fixture repo, red+green legs
#
# Exit 0 = every pin bump in range carries its record; 1 = violation;
# 2 = usage/environment error.  Merge commits are diffed against their
# first parent (a merge that lands a bump without a record fails).

set -u

RECORD_DIR=docs/handoff-20260817/review_records

lint_range () {
  local range=$1 rc=0 c new record
  local commits
  commits=$(git rev-list --first-parent "$range" 2>/dev/null) || {
    echo "pin-review-lint: bad range '$range'" >&2; return 2; }
  for c in $commits; do
    # Does this commit move the gcc gitlink vs its first parent?
    if ! git rev-parse -q --verify "$c^" >/dev/null 2>&1; then
      continue  # root commit
    fi
    local old_ptr new_ptr
    old_ptr=$(git rev-parse -q --verify "$c^:gcc" 2>/dev/null || echo none)
    new_ptr=$(git rev-parse -q --verify "$c:gcc" 2>/dev/null || echo none)
    [ "$old_ptr" = "$new_ptr" ] && continue
    [ "$new_ptr" = none ] && continue  # submodule removed: not a bump
    new=$new_ptr
    record=$RECORD_DIR/PIN_REVIEW-${new:0:12}.md
    if ! git cat-file -e "$c:$record" 2>/dev/null; then
      echo "pin-review-lint: FAIL — commit ${c:0:12} advances gcc pin to" \
           "${new:0:12} with NO review record ($record missing in that" \
           "commit's tree).  Pin bumps carry their review record in the" \
           "SAME commit (HANDOFF §1(4))." >&2
      rc=1
      continue
    fi
    if ! git show "$c:$record" | grep -q "$new"; then
      echo "pin-review-lint: FAIL — $record in commit ${c:0:12} does not" \
           "quote the full new gcc sha $new (stale or mismatched record)." >&2
      rc=1
      continue
    fi
    echo "pin-review-lint: OK — ${c:0:12} bumps gcc to ${new:0:12}," \
         "record present and pin-exact."
  done
  return $rc
}

selftest () {
  local tmp rc=0
  tmp=$(mktemp -d) || return 2
  trap 'rm -rf "$tmp"' RETURN
  local script
  script=$(cd "$(dirname "$0")" && pwd)/$(basename "$0")
  (
    set -e
    cd "$tmp"
    git init -q fix && cd fix
    git config user.email selftest@example.invalid
    git config user.name selftest
    mkdir -p "$RECORD_DIR" scripts
    cp "$script" scripts/
    # commit 1: baseline with a fake gitlink for gcc.
    A=1111111111111111111111111111111111111111
    B=2222222222222222222222222222222222222222
    echo base > base.txt
    git add base.txt scripts
    git update-index --add --cacheinfo 160000,$A,gcc
    git commit -qm baseline
    # commit 2 (RED leg): bump the gitlink, NO record.
    git update-index --add --cacheinfo 160000,$B,gcc
    git commit -qm "bare bump"
    if bash scripts/pin-review-lint.sh HEAD^..HEAD >/dev/null 2>&1; then
      echo "SELFTEST FAIL: bare bump passed the lint" >&2; exit 1
    fi
    echo "SELFTEST PASS: bare pin bump refused"
    # commit 3 (RED leg 2): record exists but quotes the WRONG sha.
    git reset -q --hard HEAD^
    git update-index --add --cacheinfo 160000,$B,gcc
    mkdir -p "$RECORD_DIR"
    echo "review of pin $A only" > "$RECORD_DIR/PIN_REVIEW-${B:0:12}.md"
    git add "$RECORD_DIR"
    git commit -qm "bump with stale record"
    if bash scripts/pin-review-lint.sh HEAD^..HEAD >/dev/null 2>&1; then
      echo "SELFTEST FAIL: stale record passed the lint" >&2; exit 1
    fi
    echo "SELFTEST PASS: stale/mismatched record refused"
    # commit 4 (GREEN leg): bump + same-commit record quoting the full sha.
    git reset -q --hard HEAD~1
    git update-index --add --cacheinfo 160000,$B,gcc
    mkdir -p "$RECORD_DIR"
    printf 'PIN_REVIEW for gcc %s\nreviewer: selftest\ngates: fixture\n' "$B" \
      > "$RECORD_DIR/PIN_REVIEW-${B:0:12}.md"
    git add "$RECORD_DIR"
    git commit -qm "bump with record"
    bash scripts/pin-review-lint.sh HEAD^..HEAD >/dev/null 2>&1 || {
      echo "SELFTEST FAIL: recorded bump refused" >&2; exit 1; }
    echo "SELFTEST PASS: same-commit recorded bump accepted"
    # commit 5 (GREEN leg): unrelated commit, no gitlink motion.
    echo x > unrelated.txt && git add unrelated.txt && git commit -qm unrelated
    bash scripts/pin-review-lint.sh HEAD^..HEAD >/dev/null 2>&1 || {
      echo "SELFTEST FAIL: non-bump commit refused" >&2; exit 1; }
    echo "SELFTEST PASS: non-bump commit ignored"
  ) || rc=1
  if [ $rc -eq 0 ]; then
    echo "pin-review-lint self-test: ALL GREEN (bare-bump RED, stale-record RED, recorded-bump GREEN, non-bump GREEN)"
  else
    echo "pin-review-lint self-test: FAILED" >&2
  fi
  return $rc
}

case "${1:-}" in
  --selftest) selftest ;;
  "") lint_range HEAD^..HEAD ;;
  *) lint_range "$1" ;;
esac
