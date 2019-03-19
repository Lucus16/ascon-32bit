all: ascon128a.h ascon128.h

TESTCASE_DIR ?= .

%.h: %.txt
	python3 $(TESTCASE_DIR)/generate.py $< $* > $@
