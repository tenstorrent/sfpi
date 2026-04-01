#! /usr/bin/env python3

# Generate C++ declarations of the sfp builtins directly from the compiler's rvtt-insn.def file.

import argparse
import os
import subprocess

targets={"":["wh", "bh"],
         "wh":["wh"],
         "bh":["bh"]}
num_archs = len (targets[""])

types={"USI":"unsigned",
       "SI":"signed",
       "VOID":"void",
       "XTT_IPTR":"volatile void *",
       "XTT_VEC":"__xtt_vector",
       "XTT_VEC2":"__xtt_vector2",
       "XTT_VEC4":"__xtt_vector4",
       "MAX":None}

builtins=[]

def extract_type (type):
  under = type.find ('_')
  if type[:under] == "XTT":
    under = type.find ('_', under + 1)
  return types[type[:under]], type[under+1:]

class Decl:
  def __init__ (self, type, arch):
    self.type = type
    self.archs = {*()}
    self.add_arch (arch)

  def add_arch (self, arch):
    for a in targets[arch]:
      self.archs.add (a)

  def print (self, name):
    rettype, type = extract_type (self.type + '_')
    if not rettype:
      return

    print(f"{rettype} __builtin_rvtt_{name} (", end="")

    type = type[type.find ('_') + 1:]
    comma = False
    while type:
      if comma:
        print(f", ", end="")
      arg, type = extract_type (type)
      print(f"{arg}", end="")
      comma = True
    print(f") noexcept;");

class Builtin:
  def __init__ (self, name):
    self.name = name
    self.decls = []

  def add_decl (self, type, arch):
    for d in self.decls:
      if (d.type == type):
        d.add_arch (arch)
        return
    self.decls.append (Decl (type, arch))

  @classmethod
  def begin (cls):
    return {*()}

  @classmethod
  def end (cls, state):
    if state:
      print (f"#endif")
    return {*()}

  def print (self, state):
    self.decls.sort (key=lambda s: len(s.archs))
    if len (self.decls) > 1 or not self.decls[0].archs == state:
      state = self.end (state)

    for decl in self.decls:
      if decl.archs == state:
        pass
      elif len (decl.archs) == num_archs:
        if state:
          print (f"#else")
      else:
        if state:
          print ("#elif ", end="")
        else:
          print ("#if ", end="")
        oring = False
        # iterate over targets[""], rather than decl.archs,
        # to maintain a stable ordering
        for arch in targets[""]:
          if arch in decl.archs and not arch in state:
            state.add (arch)
            if oring:
              print (f" || ", end="")
            oring = True
            print (f"__riscv_xtttensix{arch}", end="")
        print ("")
      decl.print (self.name)

    if len (builtin.decls) > 1:
      state = self.end (state)

    return state

def builtin_def (name, type, arch):
  builtins.append (Builtin (name))
  builtin_ovr (type, arch)

def builtin_ovr (type, arch):
  builtins[-1].add_decl (type, arch)

parser = argparse.ArgumentParser()
parser.add_argument("--gcc", type=str, default="gcc")
parser.add_argument("--version", type=str, default="")
parser.add_argument("defs", type=str, nargs=1)
args = parser.parse_args()

process = subprocess.Popen([args.gcc,
                          "-DRVTT_FN(name,arch,c,type,e,f)=builtin_def(#name,#type,#arch)",
                          "-DRVTT_OVR(a,arch,c,type,e,f)=builtin_ovr(#type,#arch)",
                            "-E", "-w", "-x", "c++", args.defs[0]],
                           stdout=subprocess.PIPE, text=True)
exec (process.communicate()[0])

print(f"// {args.version} \t-*- C++ -*-")
print("// Machine generated, do not edit")
print()
print("#pragma once")
print()
print("extern \"C\" {")

state = Builtin.begin ()
for builtin in builtins:
  state = builtin.print (state)
state = Builtin.end (state)

print("}")
