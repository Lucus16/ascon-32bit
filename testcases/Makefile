all: ascon128a.h ascon128.h

%.h: ../testcases/generate.py %.txt
	python3 $< $*.txt $* > $@
