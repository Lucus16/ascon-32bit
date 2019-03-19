.PHONY: all
all: test.elf

PREFIX        ?= riscv64-unknown-elf

AR             = $(PREFIX)-ar
CC             = $(PREFIX)-gcc
GDB            = $(PREFIX)-gdb

ASCON         ?= ascon128
OPTIMIZED     ?= ref

include sdk/libwrap/libwrap.mk

TESTCASE_DIR   = ../testcases
include $(TESTCASE_DIR)/Makefile

REF_DIR        = ../crypto_aead/$(ASCON)v12/$(OPTIMIZED)

LDSCRIPT      := sdk/flash.lds
INCLUDES      := -Isdk/include -I$(TESTCASE_DIR)/$(ASCON) -I$(REF_DIR)
ARCH_FLAGS    := -march=rv32imac -mabi=ilp32 -mcmodel=medany
CFLAGS        += -O3 -fno-builtin-printf \
				 -Wall -Wextra -Wredundant-decls \
				 -Wundef -Wshadow \
				 -fno-common $(ARCH_FLAGS)
LDFLAGS       += -static -T$(LDSCRIPT) -nostartfiles \
				 -Lsdk -Wl,--gc-sections

test.c.o: ../testcases/$(ASCON)/tests.h

test.elf: test.c.o $(REF_DIR)/ascon.c.o

%.elf: $(LDSCRIPT) $(LIBWRAP) sdk/entry.S.o sdk/start.S.o sdk/init.c.o
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out $(LIBWRAP) $(LDSCRIPT), $^) \
		-o $@ $(LDFLAGS)

%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.s.o: %.s
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.S.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(LIBWRAP_OBJS) $(LIBWRAP)
	find -name '*.elf' -o -name '*.o' | xargs rm -f
