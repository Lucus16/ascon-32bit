TESTCASE_DIR ?= .

ASCON ?= ascon128
REF    = ../crypto_aead/$(ASCON)v12/ref
CFLAGS = -O3 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable \
         -I$(REF)

testcases_all: ascon128/tests.h ascon128a/tests.h

%/tests.h: $(TESTCASE_DIR)/generate.py %.txt
	mkdir -p $*
	python3 $< $*.txt > $@

generator: generator.c $(REF)/ascon.c
	$(CC) $(CFLAGS) -o $@ $^
