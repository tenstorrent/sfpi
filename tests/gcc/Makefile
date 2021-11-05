SFPI_ROOT ?= ../..
CC_PATH ?= $(SFPI_ROOT)/compiler

INCLUDES = ../inc/ckernel_ops.h
CFLAGS = -I../inc -I$(SFPI_ROOT)/include -DARCH_GRAYSKULL -O3 -std=c++17 -Wall

RV32_CC ?= $(CC_PATH)/bin/riscv64-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -msfpu -march=rv32iy -mabi=ilp32 -fno-exceptions

SRCS = smoke.cc limits.cc nonimm.cc rv32_and_sfpu.cc sfplut.cc

ASMS = $(addprefix rv32/,$(SRCS:.cc=.S))
EXES = $(addprefix rv32/,$(SRCS:.cc=))
LTOS = $(addprefix rv32lto/,$(SRCS:.cc=))

all: $(ASMS) $(EXES) $(LTOS)

rv32:
	mkdir rv32

rv32lto:
	mkdir rv32lto

rv32/%.S: %.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -S -o $@ $<

rv32/%: %.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -o $@ $<

rv32lto/%: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -o $@ $<

clean:
	@rm -rf x86
	@rm -rf rv32
	@rm -rf rv32lto
	@rm -f *~*

.PHONY: all clean
