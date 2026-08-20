# Pin review records

One file per gcc submodule pin: `PIN_REVIEW-<first-12-hex-of-gcc-sha>.md`,
committed IN THE SAME COMMIT as the gitlink bump (enforced by
`scripts/pin-review-lint.sh`, run by the `pin-review-lint` CI workflow and
runnable locally: `bash scripts/pin-review-lint.sh <range>`).

A record MUST quote the full 40-hex gcc sha and SHOULD state:

- what the pin contains (lanes/branches merged, one line each);
- who/what reviewed it (independent review artifacts, verdict entries);
- the gate evidence: flags-off byte-identity count, DejaGnu totals with
  the frozen unexpected-FAIL set comparison, CRAQ results, fire witnesses
  on the INSTALLED binary (witness_preflight), evidence dir path;
- known regressions/withheld rows shipped with the pin, by name.

This mirrors the tt-metal sweep-side REVIEW_RECORD-<cc1plus-sha12>.md
discipline (corpus/REVIEW_RECORD_TEMPLATE.md); the two records answer
different questions (source-pin review here, installed-binary review
there) and both must exist for a measured pin.

A RETROACTIVE record (created after its bump landed, to close a bare-bump
hole) must say so in its first line.
