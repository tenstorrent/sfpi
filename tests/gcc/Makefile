SFPI_ROOT ?= ../..
CC_PATH ?= $(SFPI_ROOT)/compiler

INCLUDES = $(SFPI_ROOT)/include/ckernel_ops.h
CFLAGS = -I$(SFPI_ROOT)/include -DARCH_GRAYSKULL -O3 -std=c++17 -Wall

RV32_CC ?= $(CC_PATH)/bin/riscv32-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -mgrayskull -march=rv32iy -mabi=ilp32 -fno-exceptions

SRCS = smoke.cc limits.cc nonimm.cc rv32_and_sfpu.cc sfplut.cc unnecessarymov.cc

ASMS = $(addprefix rv32/,$(SRCS:.cc=.S)) rv32/limits_low.S rv32/limits_high.S
EXES = $(addprefix rv32/,$(SRCS:.cc=))
LTOS = $(addprefix rv32lto/,$(SRCS:.cc=)) rv32lto/limits_low rv32lto/limits_high

all: $(ASMS) $(EXES) $(LTOS)

rv32:
	mkdir rv32
rv32lto:
	mkdir rv32lto

rv32/%.S: %.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -S -o $@ $<

rv32/limits_low.S: limits.cc | rv32
	$(RV32_CC) $(RV32_CFLAGS) -DLIMIT_OFFSET12=-1 -S -o $@ $<

rv32/limits_high.S: limits.cc | rv32
	$(RV32_CC) $(RV32_CFLAGS) -DLIMIT_OFFSET12=1 -S -o $@ $<

rv32/%: %.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -o $@ $<

rv32lto/%: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -o $@ $<

rv32lto/limits_low: limits.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -DLIMIT_OFFSET12=-1 -flto -o $@ $<

rv32lto/limits_high: limits.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -DLIMIT_OFFSET12=1 -flto -o $@ $<

test: rv32/smoke.S rv32/limits.S rv32/limits_low.S rv32/limits_high.S rv32/nonimm.S rv32/unnecessarymov.S
	cmp rv32/smoke.S gold/smoke.S
	cmp rv32/limits.S gold/limits.S
	cmp rv32/limits_low.S gold/limits_low.S
	cmp rv32/limits_high.S gold/limits_high.S
	cmp rv32/nonimm.S gold/nonimm.S
	cmp rv32/unnecessarymov.S gold/unnecessarymov.S

promote:
	cp rv32/smoke.S gold/smoke.S
	cp rv32/limits.S gold/limits.S
	cp rv32/limits_low.S gold/limits_low.S
	cp rv32/limits_high.S gold/limits_high.S
	cp rv32/nonimm.S gold/nonimm.S
	cp rv32/unnecessarymov.S gold/unnecessarymov.S

clean:
	@rm -rf x86
	@rm -rf rv32
	@rm -rf rv32lto
	@rm -rf results
	@rm -f *~*

.PHONY: all clean
