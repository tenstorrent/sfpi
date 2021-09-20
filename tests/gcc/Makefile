CC_PATH ?= $(SFPI_ROOT)/../install

INCLUDES = ../../sfpi/sfpi.h ../../sfpi/sfpi_internal.h ../inc/ckernel_ops.h
CFLAGS = -I../inc -I../../sfpi -DARCH_GRAYSKULL -O3 -std=c++17 -Wall

RV32_CC ?= $(CC_PATH)/bin/riscv64-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -msfpu -march=rv32imay -mabi=ilp32 -fno-exceptions

SRCS = smoke.cc limits.cc immediate.cc rv32_and_sfpu.cc sfplut.cc

ASMS = $(addprefix rv32/,$(SRCS:.cc=.S))

all: $(ASMS)

rv32:
	mkdir rv32

rv32/%.S: %.cc rv32 $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -S -o $@ $<

rv32/%: %.cc | rv32 $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -o $@ $<

clean:
	@rm -rf x86
	@rm -rf rv32
	@rm -f *~*

.PHONY: all clean
