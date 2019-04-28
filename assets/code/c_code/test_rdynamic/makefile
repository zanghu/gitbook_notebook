.PHONY: all clean test

LDEXTRAFLAGS ?=

all: prog

bar.o: bar.c
	cc -c -Wall -fpic -o $@ $<

libbar.so: bar.o
	cc -shared -o $@ $<

main.o: main.c
	cc -c -Wall -o $@ $<

prog: main.o | libbar.so
	cc $(LDEXTRAFLAGS) -rdynamic -o $@ $< -L. -lbar -ldl

clean:
	rm -f *.o *.so prog

test: prog
	./$<