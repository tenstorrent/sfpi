BUDA_ROOT ?= ../../../ll-sw
SFPI_ROOT ?= ../..
CC_PATH ?= $(SFPI_ROOT)/compiler

INCLUDES = $(SFPI_ROOT)/include/sfpi.h $(SFPI_ROOT)/include/sfpi_fp16.h $(SFPI_ROOT)/include/sfpi_internal.h ../inc/ckernel_ops.h
CFLAGS = -I../inc -I$(SFPI_ROOT)/include -DARCH_GRAYSKULL -O3 -std=c++17 -Wall
# Note: Production uses -g which adds GIMPLE_DEBUG gimple statements that can affect optimization
# Enabling -g here results in unreadable asm, but testing should occur both ways...

X86_INCLUDES = $(INCLUDES) sfpu.h
X86_CC = g++
X86_CFLAGS = -DCOMPILE_FOR_EMULE $(CFLAGS)

RV32_CC ?= $(CC_PATH)/bin/riscv64-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -msfpu -march=rv32iy -mabi=ilp32 -fno-exceptions

SRCS = kernels.cc coverage.cc live.cc fp16c.cc

ASMS = $(addprefix rv32/,$(SRCS:.cc=.S)) rv32/combine.S rv32/pushpop.S rv32/regmov.S
EXES = $(addprefix rv32/,$(SRCS:.cc=))
LTOS = $(addprefix rv32lto/,$(SRCS:.cc=))
SIMS = $(addprefix x86/,$(SRCS:.cc=))

ifneq ("$(wildcard ckernel.cc)", "")
ASMS += rv32/ckernel.S
endif

all: $(ASMS) $(EXES) $(LTOS) $(SIMS)

x86:
	mkdir x86
rv32:
	mkdir rv32
rv32lto:
	mkdir rv32lto
results:
	mkdir results

x86/sfpu.o: sfpu.cc sfpu.h $(SFPI_ROOT)/include/sfpi_internal.h

x86/%.o: %.cc $(X86_INCLUDES) | x86
	$(X86_CC) $(X86_CFLAGS) -c -o $@ $<

x86/%: x86/%.o x86/sfpu.o | x86
	$(X86_CC) $(X86_CFLAGS) x86/sfpu.o -o $@ $<

rv32/%.S: %.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -S -o $@ $<

rv32/%: %.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -o $@ $<

rv32lto/%: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -o $@ $<

rv32lto/%.S: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -S -o $@ $<

rv32lto/%.o: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -c -o $@ $<

rv32ltoo/%.elf: lto/%.o $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -Wl,--verbose -o $@ $<

resultsx86/%: x86/kernels | results
	x86/kernels 14 b > results/kernels_int
	x86/kernels 14 > results/kernels_float

test: resultsx86/kernels_int resultsx86/kernels/float $(ASMS)
	diff results/kernels_float gold/kernels_float
	diff results/kernels_int gold/kernels_int
	cmp rv32/combine.S gold/combine.S
	cmp rv32/pushpop.S gold/pushpop.S
	cmp rv32/coverage.S gold/coverage.S
	cmp rv32/live.S gold/live.S
	cmp rv32/regmov.S gold/regmov.S
ifneq ("$(wildcard ckernel.cc)", "")
	cmp rv32/ckernel.S gold/ckernel.S
endif

promote:
	cp results/kernels_int gold/kernels_int
	cp results/kernels_float gold/kernels_float
	cp rv32/combine.S gold/combine.S
	cp rv32/pushpop.S gold/pushpop.S
	cp rv32/coverage.S gold/coverage.S
	cp rv32/live.S gold/live.S
	cp rv32/regmov.S gold/regmov.S
ifneq ("$(wildcard ckernel.cc)", "")
	cp rv32/ckernel.S gold/ckernel.S
endif

clean:
	@rm -rf x86
	@rm -rf rv32
	@rm -rf rv32lto
	@rm -rf results
	@rm -rf core.*
	@rm -f *~*

.PHONY: all clean
