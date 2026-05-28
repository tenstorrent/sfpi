#! /usr/bin/env python3

# Tests for local-xfails.py
# Run from anywhere:
#   python3 scripts/test_local_xfails.py

import io
import sys
import os
import importlib.util
import unittest

# local-xfails.py has a hyphen in its name, which is not a valid Python
# identifier; use importlib to load it by file path.
_here = os.path.dirname(os.path.abspath(__file__))
_spec = importlib.util.spec_from_file_location(
    "local_xfails", os.path.join(_here, "local-xfails.py")
)
_mod = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_mod)
readXfails = _mod.readXfails


class TestReadXfailsWhitespaceSeparators(unittest.TestCase):
    """readXfails must accept both space and tab as WS per the documented grammar:
       WS: ' ' | '\\t'
    """

    def _parse(self, text):
        return readXfails("<test>", io.StringIO(text))

    # Baseline: space-separated (must keep working)

    def test_space_multilibs_single(self):
        text = "multilibs: riscv-sim/*\nFAIL: some/test.c -O2 execution test\n"
        result = self._parse(text)
        self.assertIn("riscv-sim/*", result)
        self.assertEqual(result["riscv-sim/*"], ["some/test.c -O2 execution test"])

    def test_space_multilibs_multiple_specs(self):
        text = "multilibs: riscv-sim/cpu=gs riscv-sim/cpu=wh\nFAIL: foo.c -O0 execution test\n"
        result = self._parse(text)
        self.assertIn("riscv-sim/cpu=gs", result)
        self.assertIn("riscv-sim/cpu=wh", result)

    # Bug cases: tab as WS separator

    def test_tab_after_multilibs_keyword_does_not_crash(self):
        """Before the fix, 'multilibs:\\t...' caused an IndexError crash because
        line.split(' ', 1) failed to split at the tab, leaving tv with only one
        element and making tv[1] raise IndexError."""
        text = "multilibs:\triscv-sim/*\nFAIL: some/test.c -O2 execution test\n"
        result = self._parse(text)
        self.assertIn("riscv-sim/*", result,
                      "Tab-separated multilibs: spec not parsed correctly")

    def test_tab_separated_multilibs_specs(self):
        """If specs themselves are tab-separated, all globs must be recognised
        individually. Before the fix, split(' ') left them fused."""
        text = "multilibs:\triscv-sim/cpu=gs\triscv-sim/cpu=wh\nFAIL: bar.c -O1 execution test\n"
        result = self._parse(text)
        self.assertIn("riscv-sim/cpu=gs", result,
                      "First tab-separated spec not parsed as its own key")
        self.assertIn("riscv-sim/cpu=wh", result,
                      "Second tab-separated spec not parsed as its own key")

    def test_tab_after_fail_keyword_parses_test_name(self):
        """'FAIL:\\ttest-name' must parse the full test name, not trigger the
        'Unknown line type' path (which happened because split(' ',1) treated
        'FAIL:\\ttest-name' as a single token)."""
        text = "multilibs: *\nFAIL:\tsome/test.c -O2 execution test\n"
        result = self._parse(text)
        self.assertIn("*", result)
        self.assertEqual(result["*"], ["some/test.c -O2 execution test"])

    # Edge cases

    def test_blank_line_does_not_crash(self):
        text = "multilibs: *\n\nFAIL: a.c -O0 execution test\n"
        result = self._parse(text)
        self.assertIn("*", result)

    def test_comment_line_ignored(self):
        text = "# this is a comment\nmultilibs: *\nFAIL: a.c -O0 execution test\n"
        result = self._parse(text)
        self.assertIn("*", result)
        self.assertEqual(result["*"], ["a.c -O0 execution test"])

    def test_default_multilib_glob(self):
        """Without any multilibs: line the default glob '*' is used."""
        text = "FAIL: default.c -O0 execution test\n"
        result = self._parse(text)
        self.assertIn("*", result)


if __name__ == "__main__":
    unittest.main()
