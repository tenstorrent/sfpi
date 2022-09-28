SFPI_ROOT ?= ../..
CC_PATH ?= $(SFPI_ROOT)/compiler

INCLUDES =

CFLAGS = -ffast-math -O3 -std=c++17 -Wall
# Note: Production uses -g which adds GIMPLE_DEBUG gimple statements that can affect optimization
# Enabling -g here results in unreadable asm, but testing should occur both ways...

RV32_CC ?= $(CC_PATH)/bin/riscv32-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -mgrayskull -march=rv32i -mtune=rvtt-b1 -mabi=ilp32 -fno-exceptions -Wall -std=c++17 -Wno-unknown-pragmas -fno-use-cxa-atexit -Wno-error=multistatement-macros -Wno-error=parentheses -Wno-error=unused-but-set-variable -Wno-unused-variable -fno-exceptions

SRCS = rmext.cc load.cc latency.cc gs_arbiter_war.cc

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

rv32lto/%.S: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -S -o $@ $<

rv32lto/%.o: %.cc $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -c -o $@ $<

rv32lto/%.elf: lto/%.o $(INCLUDES) | rv32lto
	$(RV32_CC) $(RV32_CFLAGS) -flto -Wl,--verbose -o $@ $<

test: $(ASMS) $(OTHR)
	cmp rv32/rmext.S gold/rmext.S
	cmp rv32/load.S gold/load.S
	cmp rv32/latency.S gold/latency.S
	cmp rv32/gs_arbiter_war.S gold/gs_arbiter_war.S

promote:
	cp rv32/rmext.S gold/rmext.S
	cp rv32/load.S gold/load.S
	cp rv32/latency.S gold/latency.S
	cp rv32/gs_arbiter_war.S gold/gs_arbiter_war.S

clean:
	@rm -rf rv32
	@rm -rf rv32lto
	@rm -rf results
	@rm -rf core.*
	@rm -f *~*

.PHONY: all clean
