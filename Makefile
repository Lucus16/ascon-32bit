.PHONY: all
all: asmwrapper.elf cyclecount.elf test.elf

PREFIX        ?= riscv64-unknown-elf

AR             = $(PREFIX)-ar
CC             = $(PREFIX)-gcc
GDB            = $(PREFIX)-gdb

include sdk/libwrap/libwrap.mk

LDSCRIPT      := sdk/flash.lds
INCLUDES      := -Isdk/include
ARCH_FLAGS    := -march=rv32imac -mabi=ilp32 -mcmodel=medany
CFLAGS        += -O3 -fno-builtin-printf \
				 -Wall -Wextra -Wredundant-decls \
				 -Wundef -Wshadow \
				 -fno-common $(ARCH_FLAGS)
LDFLAGS       += -static -T$(LDSCRIPT) -nostartfiles \
				 -Lsdk -Wl,--gc-sections

asmwrapper.elf: asmfunction.S.o asmwrapper.c.o

cyclecount.elf: getcycles.S.o cyclecount.c.o

test.elf: test.c.o ref/ascon.c.o

%.elf: $(LDSCRIPT) $(LIBWRAP) sdk/entry.S.o sdk/start.S.o sdk/init.c.o
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out $(LIBWRAP) $(LDSCRIPT), $^) \
		-o $@ $(LDFLAGS)

%.o: %
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(LIBWRAP_OBJS) $(LIBWRAP)
	find -name '*.elf' -o -name '*.o' | xargs rm -f
