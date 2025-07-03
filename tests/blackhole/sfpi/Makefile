ifeq ($(SFPI_ROOT),)
SFPI_ROOT = ../../..
else ifeq ($(firstword $(shell ls -id $(SFPI_ROOT))),$(firstword $(shell ls -id ../../..)))
SFPI_ROOT = ../../..
endif
CC_PATH ?= $(SFPI_ROOT)/compiler

INCLUDES = $(SFPI_ROOT)/include/sfpi.h $(SFPI_ROOT)/include/sfpi_fp16.h $(SFPI_ROOT)/include/blackhole/sfpi_hw.h $(SFPI_ROOT)/include/blackhole/sfpi_imp.h $(SFPI_ROOT)/include/blackhole/sfpi_lib.h $(SFPI_ROOT)/include/blackhole/ckernel_ops.h

CFLAGS = -I$(SFPI_ROOT)/include -I../../common/include -I$(SFPI_ROOT)/include/blackhole -DARCH_BLACKHOLE -O3 -std=c++17 -Wall
# Note: Production uses -g which adds GIMPLE_DEBUG gimple statements that can affect optimization
# Enabling -g here results in unreadable asm, but testing should occur both ways...

RV32_CC ?= $(CC_PATH)/bin/riscv32-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -mcpu=tt-bh -fno-exceptions

SRCS = kernels.cc coverage.cc live.cc

ASMS = $(addprefix rv32/,$(SRCS:.cc=.S)) rv32/combine.S rv32/pushpop.S rv32/regmov.S rv32/vcond.S rv32/nonimm.S rv32/unnecessarymov.S rv32/replay.S rv32/schedule.S
OTHR = rv32/warn.txt rv32/warn_push.txt rv32/warn_pop.txt rv32/warn_spill.txt
EXES = $(addprefix rv32/,$(SRCS:.cc=))
LTOS = $(addprefix rv32lto/,$(SRCS:.cc=))

ifneq ("$(wildcard ckernel.cc)", "")
ASMS += rv32/ckernel.S
endif

all: $(ASMS) $(EXES) $(LTOS) $(OTHR)

rv32:
	mkdir rv32
rv32lto:
	mkdir rv32lto
results:
	mkdir results

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
	$(RV32_CC) $(RV32_CFLAGS) -flto=auto -o $@ $<

rv32lto/%.S: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto=auto -S -o $@ $<

rv32lto/%.o: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto=auto -c -o $@ $<

rv32lto/%.elf: lto/%.o $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto=auto -Wl,--verbose -o $@ $<

test: $(ASMS) $(OTHR)
	diff results/kernels_float gold/kernels_float
	diff results/kernels_int gold/kernels_int
	cmp rv32/combine.S gold/combine.S
	cmp rv32/pushpop.S gold/pushpop.S
	cmp rv32/coverage.S gold/coverage.S
	cmp rv32/live.S gold/live.S
	cmp rv32/regmov.S gold/regmov.S
	cmp rv32/vcond.S gold/vcond.S
	cmp rv32/nonimm.S gold/nonimm.S
	cmp rv32/replay.S gold/replay.S
	cmp rv32/unnecessarymov.S gold/unnecessarymov.S
	cmp rv32/schedule.S gold/schedule.S
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
	cp rv32/vcond.S gold/vcond.S
	cp rv32/nonimm.S gold/nonimm.S
	cp rv32/replay.S gold/replay.S
	cp rv32/unnecessarymov.S gold/unnecessarymov.S
	cp rv32/schedule.S gold/schedule.S
	cp rv32/warn.txt gold/warn.txt
	cp rv32/warn_push.txt gold/warn_push.txt
	cp rv32/warn_pop.txt gold/warn_pop.txt
	cp rv32/warn_spill.txt gold/warn_spill.txt
ifneq ("$(wildcard ckernel.cc)", "")
	cp rv32/ckernel.S gold/ckernel.S
endif

clean:
	@rm -rf rv32
	@rm -rf rv32lto
	@rm -rf results
	@rm -rf core.*
	@rm -f *~*

.PHONY: all clean
