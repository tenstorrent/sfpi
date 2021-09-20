CC_PATH ?= $(SFPI_ROOT)/../install

INCLUDES = ../../sfpi/sfpi.h ../../sfpi/sfpi_internal.h ../inc/ckernel_ops.h
CFLAGS = -I../inc -I../../sfpi -DARCH_GRAYSKULL -O3 -std=c++17 -Wall

X86_INCLUDES = $(INCLUDES) sfpu.h
X86_CC = g++
X86_CFLAGS = -DCOMPILE_FOR_EMULE $(CFLAGS)

RV32_CC ?= $(CC_PATH)/bin/riscv64-unknown-elf-g++
RV32_CFLAGS = $(CFLAGS) -msfpu -march=rv32imay -mabi=ilp32 -fno-exceptions

SRCS = kernels.cc coverage.cc

all: $(addprefix rv32/,$(SRCS:.cc=.S)) $(addprefix rv32lto/,$(SRCS:.cc=.S)) $(addprefix x86/,$(SRCS:.cc=))

x86:
	mkdir x86
rv32:
	mkdir rv32
rv32lto:
	mkdir rv32lto


x86/sfpu.o: sfpu.cc sfpu.h ../../sfpi/sfpi_internal.h

x86/%.o: %.cc $(X86_INCLUDES) | x86
	$(X86_CC) $(X86_CFLAGS) -c -o $@ $<

x86/%: x86/%.o x86/sfpu.o
	$(X86_CC) $(X86_CFLAGS) x86/sfpu.o -o $@ $<

x86/%.S: %.cc x86 $(X86_INCLUDES)
	$(X86_CC) $(X86_CFLAGS) -S -o $@ $<

rv32/%.S: %.cc rv32 $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -S -o $@ $<

rv32/%: %.cc | rv32 $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -o $@ $<

rv32lto/%: %.cc rv32lto $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -flto -o $@ $<

rv32lto/%.S: %.cc rv32lto $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -flto -S -o $@ $<

rv32lto/%.o: %.cc rv32lto $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -flto -c -o $@ $<

rv32ltoo/%.elf: lto/%.o rv32lto $(INCLUDES)
	$(RV32_CC) $(RV32_CFLAGS) -flto -Wl,--verbose -o $@ $<

clean:
	@rm -rf x86
	@rm -rf rv32
	@rm -rf rv32lto
	@rm -f *~*

.PHONY: all clean
