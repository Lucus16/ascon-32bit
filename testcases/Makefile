TESTCASE_DIR ?= .

all_tests: ascon128/tests.h ascon128a/tests.h

%/tests.h: $(TESTCASE_DIR)/generate.py %.txt
	mkdir -p $*
	python3 $< $*.txt > $@
