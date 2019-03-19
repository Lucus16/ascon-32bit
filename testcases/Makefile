TESTCASE_DIR ?= .

%/tests.h: $(TESTCASE_DIR)/generate.py %.txt
	mkdir -p $*
	python3 $< $*.txt $* > $@
