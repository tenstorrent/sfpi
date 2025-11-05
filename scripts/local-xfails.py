#! /usr/bin/env python3

# post process dejagnu *.sum files to filter more xfails (caused by local spurious effects)
#   process-tests [--xfails=DIR] FILE.sum ...
# xfail files are named FILE.xfail and have the following syntax:

# FILE: LINE*
# LINE: COMMENT | BLANK | MULTILIBS | FAIL
# COMMENT: '#' text '\n'
# BLANK: WS* '\n'
# MULTILIBS: 'multilibs:' WS SPEC+ '\n'
# FAIL: 'FAIL:' WS TEST '\n'
# WS: ' ' | '\t'
# SPEC: GLOB
# TEST: LITERAL

# Each fail is keyed by the most recently preceeding multilib spec
# (defaults to ['*']).  the .sum file is read and every PASS|FAIL test
# line is matched against the set of expected FAILS and turned into an
# XPASS or XFAIL if there is a match. The file is written out into the
# current directory. Its Summary counts are adjusted.

# dejagnu .sum files have the following form (pseudo templatey language)

# intro-lines
# {repeat for each multilib
# Running target $MULTILIBSPEC
# run-lines
# PASS/FAIL: test-name
# === $SUITE Summary for $MULTILIBSPEC ===
# blank
# # of expected passes   $N
# # of unexpected failures $N
# # of unexpected successes $N
# # of expected failures $N
# # of unsupported tests $N
# }
# === $SUITE Summary ===
# # of expected passes   $N
# # of unexpected failures $N
# # of unexpected successes $N
# # of expected failures $N
# # of unsupported tests $N
# outro-lines

import sys
import os
import argparse
import re
import fnmatch
from enum import Enum, auto

debug = False
# debug = True

# returns a dictionary of string vectors
# keyed by multilib glob, contains test names
def readXfails(file, fd):
  res = {}
  multilibs = ['*']
  lineno = 0
  while True:
    lineno += 1
    line = fd.readline()
    if not line:
      break
    line = line.strip()
    tv = line.split(' ', 1)
    token=tv[0]
    if token == '':
      # blank line
      pass
    elif token == '#':
      # comment
      pass
    elif token == 'multilibs:':
      # multilib glob pattern
      multilibs = tv[1].split(' ')
    elif token == 'FAIL:':
      # expected fail
      for lib in multilibs:
        if debug:
          print(f"{lib} failing {tv[1]}")
        res.setdefault(lib, []).append(tv[1])
    else:
      print(f"{file}:{lineno}: Unknown line type {token}")

  return res

def processFails(inname, tool, fails, fin, fout):
  class State(Enum):
    NONE = 0 #->TESTS

    #^Running target $MULTILIB
    TESTS = auto() #->SUMMARY | FINAL

    #=== $TOOL Summary for $MULTILIB === 
    #=== $TOOL Summary ===
    SUMMARY = auto() #->TESTS | SUMMARY
  class Expected(Enum):
    # This is ordered according to how dejagnu emit summary lines
    NONE = 0
    PASS = auto() # expected passes
    FAIL = auto() # unexpected failures
    XPASS = auto() # unexpected passes
    XFAIL = auto() # expected failures
    OTHER = auto()
  lineno = 0
  state = State.NONE
  expected = Expected.NONE
  xfailed = 0 # tests we xfailed
  xpassed = 0 # tests we xpassed
  totalXfailed = 0 # accumulation across all mlibs
  totalXpassed = 0 # likewise
  multilib = '' # current multilib
  failingTests = {} # dictionary of xfailed tests
  multiplePasses = False

  # regexes 
  runPattern = re.compile('Running target ([-a-zA-Z0-9_=/]+)\n')
  mlibSumPattern = re.compile(f'	+=== {re.escape(tool)} Summary for ([-a-zA-Z0-9_=/]+) ===\n')
  finalSumPattern = re.compile(f'	+=== {re.escape(tool)} Summary ===\n')
  testPattern = re.compile('([A-Z]+): (.+)\n')
  expectedPattern = re.compile('# of ([a-z]+) ([a-z]+)	+([0-9]+)\n')
  
  def summaryLine(line):
    nonlocal state
    if state != State.SUMMARY:
      return

    # adjust {,un}expected failures
    # adjust unexpected successes
    # 'of unexpected failures' -= xfailed
    # 'of expected failures' += xfailed
    # 'of unexpected passes' += xpassed
    # 'of expected passes' -= xpassed

    nonlocal expectedPattern
    match = expectedPattern.fullmatch(line) if line else None
    nonlocal expected
    toExpect = expected if line == '\n' else Expected.OTHER
    value = 0
    if match:
      isExpected = match[1] == "expected"
      if isExpected or match[1] == "unexpected":
        if match[2] == "passes" or match[2] == "successes":
          toExpect = Expected.PASS if isExpected else Expected.XPASS
        elif match[2] == "failures":
          toExpect = Expected.XFAIL if isExpected else Expected.FAIL
      if toExpect != Expected.OTHER:
        value = int(match[3])
    nonlocal fout
    if expected.value > toExpect.value:
      print(f"reversed summary order: {expected} > {toExpect}")
    while expected.value < toExpect.value:
      if expected == Expected.XPASS and xpassed != 0:
        fout.write(f"# of unexpected passes		{xpassed}\n")
      elif expected == Expected.XFAIL and xfailed != 0:
        fout.write(f"# of expected failures		{xfailed}\n")
      expected = Expected(expected.value + 1)
    if expected != Expected.OTHER and expected != Expected.NONE:
      if expected == Expected.PASS:
        value -= xpassed
      elif expected == Expected.FAIL:
        value -= xfailed
      elif expected == Expected.XPASS:
        value += xpassed
      elif expected == Expected.XFAIL:
        value += xfailed
      if value == 0:
        line = None
      else:
        line = line[:match.start(3)] + str(value) + line[match.end(3):]
      expected = Expected(expected.value + 1)
    return line

  while True:
    lineno += 1
    line = fin.readline()
    if not line:
      break
    match = runPattern.fullmatch(line)
    if match:
      # new multilib pass
      summaryLine(None)
      if state != State.NONE and state != State.SUMMARY:
        print(f"{inname}:{lineno}: Unexpected transition from {state} to {State.TESTS}")
      multilib = match[1]
      # compute the failedTest names
      failingTests = {}
      for glob, names in fails.items():
        if fnmatch.fnmatchcase(multilib, glob):
          for name in names:
            failingTests[name] = True
      xfailed = 0
      xpassed = 0
      # now expecting test results
      state = State.TESTS
      if debug:
        print(f"{inname}:{lineno}: Transition to {state}")
    else:
      match = mlibSumPattern.fullmatch(line)
      if match:
        # summary of one multilib
        summaryLine(None)
        if state != State.TESTS:
          print(f"{inname}:{lineno}: Unexpected transition from {state} to {State.SUMMARY}")
        elif multilib != match[1]:
          print(f"{inname}:{lineno}: Multilib does not match {multilib}")
        state = State.SUMMARY
        expected = Expected.NONE
        multiplePasses = True
        totalXfailed += xfailed
        totalXpassed += xpassed
        if debug:
          print(f"{inname}:{lineno}: Transition to {state}")
      else:
        match = finalSumPattern.fullmatch(line)
        if match:
          # final summary
          summaryLine(None)
          if state != (State.SUMMARY if multiplePasses else State.TESTS):
            print(f"{inname}:{lineno}: Unexpected transition from {state} to {State.SUMMARY}")
          if multiplePasses:
            xfailed = totalXfailed
            xpassed = totalXpassed
          state = State.SUMMARY
          expected = Expected.NONE
          if debug:
            print(f"{inname}:{lineno}: Transition to {state}")
        else:
          if state == State.TESTS:
            match = testPattern.fullmatch(line)
            if match and failingTests.get(match[2]):
              if match[1] == "FAIL":
                # expected fail
                xfailed += 1
                line = "X" + line
                if debug:
                  print(f"xfailing {line[:-1]}")
              elif match[1] == "PASS":
                # unexpected pass
                xpassed += 1
                line = "X" + line
                if debug:
                  print(f"xpassing {line[:-1]}")
          elif state == State.SUMMARY:
            line = summaryLine(line)
    if line:
      fout.write(line)
  
  pass
  
parser = argparse.ArgumentParser()
parser.add_argument("--xfails", type=str, default='.')
parser.add_argument("--output", type=str, default='.')
parser.add_argument("summaries", type=str, nargs='*')
args = parser.parse_args()

for summary in args.summaries:
  outname = os.path.basename(summary)
  tool = os.path.splitext(outname)[0]
  fails = {}
  xfails = args.xfails + '/' + tool + '.xfail'
  try:
    with open(xfails) as fd:
      fails = readXfails(xfails, fd)
  except FileNotFoundError:
    print(f"{xfails}: No xfails present")
  with open(summary) as fin:
    outpath=args.output
    if os.path.isdir(outpath):
      outpath = outpath + '/' + outname
    with open(outpath, 'w') as fout:
      processFails(summary, tool, fails, fin, fout)
