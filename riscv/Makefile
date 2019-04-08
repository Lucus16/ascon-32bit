PREFIX        ?= riscv64-unknown-elf

AR             = $(PREFIX)-ar
CC             = $(PREFIX)-gcc
GDB            = $(PREFIX)-gdb

ASCON         ?= ascon128
IMPL          ?= ref

CONFIG         = $(ASCON)-$(IMPL)

.PHONY: all
all: build/$(CONFIG).elf

include sdk/libwrap/libwrap.mk

TESTCASE_DIR   = ../testcases
include $(TESTCASE_DIR)/Makefile

LDSCRIPT      := sdk/flash.lds
INCLUDES      := -Isdk/include -I$(TESTCASE_DIR)/$(ASCON) -I$(CONFIG)
ARCH_FLAGS    := -march=rv32imac -mabi=ilp32 -mcmodel=medany
CFLAGS        += -O3 -fno-builtin-printf \
				 -Wall -Wextra -Wredundant-decls \
				 -Wundef -Wshadow \
				 -fno-common $(ARCH_FLAGS)
LDFLAGS       += -static -T$(LDSCRIPT) -nostartfiles \
				 -Lsdk -Wl,--gc-sections

SDK_OBJS       = build/entry.o build/start.o build/init.o

build/$(ASCON)-test.o: test.c ../testcases/$(ASCON)/tests.h
build/$(ASCON)-asm.elf: build/$(ASCON)-test.o build/$(CONFIG).o build/cycles.o build/permutation.o
build/$(CONFIG).elf: build/$(ASCON)-test.o build/$(CONFIG).o build/cycles.o

build/cycles.o: cycles.s
build/permutation.o: permutation.s
build/$(CONFIG).o: $(CONFIG)/ascon.c
build/entry.o: sdk/entry.S
build/start.o: sdk/start.S
build/init.o: sdk/init.c

build/%.o:
	mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

build/%.elf: $(LDSCRIPT) $(LIBWRAP) $(SDK_OBJS)
	mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out $(LIBWRAP) $(LDSCRIPT), $^) \
		-o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf $(LIBWRAP_OBJS) $(LIBWRAP) build/
