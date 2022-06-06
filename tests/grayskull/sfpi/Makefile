SFPI_ROOT ?= ../../..
CC_PATH ?= $(SFPI_ROOT)/compiler

INCLUDES = $(SFPI_ROOT)/include/sfpi.h $(SFPI_ROOT)/include/sfpi_fp16.h $(SFPI_ROOT)/include/grayskull/sfpi_hw.h $(SFPI_ROOT)/include/grayskull/sfpi_imp.h $(SFPI_ROOT)/include/grayskull/sfpi_lib.h $(SFPI_ROOT)/include/grayskull/ckernel_ops.h

CFLAGS = -I$(SFPI_ROOT)/include -I../../common/include -I$(SFPI_ROOT)/include/grayskull -DARCH_GRAYSKULL -O3 -std=c++17 -Wall
# Note: Production uses -g which adds GIMPLE_DEBUG gimple statements that can affect optimization
# Enabling -g here results in unreadable asm, but testing should occur both ways...

X86_INCLUDES = $(INCLUDES) sfpu.h
X86_CC = g++
X86_CFLAGS = -DCOMPILE_FOR_EMULE $(CFLAGS)

RV32_CC ?= $(CC_PATH)/bin/riscv32-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -mgrayskull -march=rv32iy -mabi=ilp32 -fno-exceptions

SRCS = kernels.cc coverage.cc live.cc

ASMS = $(addprefix rv32/,$(SRCS:.cc=.S)) rv32/combine.S rv32/pushpop.S rv32/regmov.S
OTHR = rv32/warn.txt rv32/warn_push.txt rv32/warn_pop.txt rv32/warn_spill.txt
EXES = $(addprefix rv32/,$(SRCS:.cc=))
LTOS = $(addprefix rv32lto/,$(SRCS:.cc=))
SIMS = $(addprefix x86/,$(SRCS:.cc=))

ifneq ("$(wildcard ckernel.cc)", "")
ASMS += rv32/ckernel.S
endif

all: $(ASMS) $(EXES) $(LTOS) $(SIMS) $(OTHR)

x86:
	mkdir x86
rv32:
	mkdir rv32
rv32lto:
	mkdir rv32lto
results:
	mkdir results

x86/sfpu.o: sfpu.cc sfpu.h $(INCLUDES)

x86/%.o: %.cc $(X86_INCLUDES) | x86
	$(X86_CC) $(X86_CFLAGS) -c -o $@ $<

x86/%: x86/%.o x86/sfpu.o | x86
	$(X86_CC) $(X86_CFLAGS) x86/sfpu.o -o $@ $<

rv32/warn.txt: warn.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -S -o rv32/warn.S $< 2> $@ || true
rv32/warn_push.txt: warn.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -DPUSH_POP -S -o rv32/warn.S $< 2> $@ || true
rv32/warn_pop.txt: warn.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -DPOP_PUSH -S -o rv32/warn.S $< 2> $@ || true
rv32/warn_spill.txt: warn.cc $(INCLUDES) | rv32
	$(RV32_CC) $(RV32_CFLAGS) -DSPILL -S -o rv32/warn.S $< 2> $@ || true

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

rv32lto/%.elf: lto/%.o $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -Wl,--verbose -o $@ $<

resultsx86/%: x86/kernels | results
	x86/kernels 14 b > results/kernels_int
	x86/kernels 14 > results/kernels_float

test: resultsx86/kernels_int resultsx86/kernels/float $(ASMS) $(OTHR)
	diff results/kernels_float gold/kernels_float
	diff results/kernels_int gold/kernels_int
	cmp rv32/combine.S gold/combine.S
	cmp rv32/pushpop.S gold/pushpop.S
	cmp rv32/coverage.S gold/coverage.S
	cmp rv32/live.S gold/live.S
	cmp rv32/regmov.S gold/regmov.S
	cmp rv32/warn.txt gold/warn.txt
	cmp rv32/warn_push.txt gold/warn_push.txt
	cmp rv32/warn_pop.txt gold/warn_pop.txt
	cmp rv32/warn_spill.txt gold/warn_spill.txt
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
	cp rv32/warn.txt gold/warn.txt
	cp rv32/warn_push.txt gold/warn_push.txt
	cp rv32/warn_pop.txt gold/warn_pop.txt
	cp rv32/warn_spill.txt gold/warn_spill.txt
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
