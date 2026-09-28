# Portability patch validation — 2026-09-28

Base: workflow `15ec9e7e92a92258d12956a75a802a1b705fe7f3`.
Changes were exercised in a separate detached worktree; workflow `main` was
neither edited nor committed/pushed.

Completed checks:

- Consolidated command guide is root `HANDOFF.md`; duplicate reproduction
  README and obsolete pin-47 handoff removed (recoverable in Git history).
- Executed the handoff's flag-construction snippet, without hardware, for all
  nine documented OP choices against the actual recorded manifest: each
  changes exactly the intended single option. Shell and Python syntax pass.

- `git apply --check workflow-portability.patch` against the untouched base: PASS.
- `python3 -m unittest discover -s scripts -p 'test_*.py'` on patched tree:
  57 tests, PASS.
- `bash scripts/test_toolchain_provenance.sh`: PASS.
- Extended source-bundle round trip verifies no WORK assignment or old restore
  path in generated pins; sources a relocated file with two caller-selected
  roots, including one containing spaces; both preserved exactly.
- Shell syntax and embedded Python syntax checked for reproduction examples;
  source-document local links checked: PASS.
- Patched headline `FILES.sha256` verification: PASS. The patch updates the
  README checksum and adds the portable environment checksum; archived
  manifests, results, reports, raw archive and its per-file hashes are unchanged.

To repeat host checks, apply the patch as documented and run the two test
commands above. The patch intentionally leaves tracked workflow changes in
that local checkout. Preserve the patch with the source branch; if later
exporting a clean source bundle, commit those local workflow changes first.
No push to workflow main is needed.

Limits: this change did not perform a new empty-node toolchain build or silicon
sweep. Earlier source-restore, clean-build and hardware evidence remains bound
to its recorded source and binary identities. The new recipe closes the
documented first-mile steps without relabeling that older evidence as a newly
executed end-to-end run.
