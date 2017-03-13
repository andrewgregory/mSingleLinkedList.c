# the tap.c submodule must be checked out to build and run tests

CFLAGS += -Wall -Wextra -Wpedantic -g

override CPPFLAGS += -I..

TESTS = 01-sanity.t 10-msort.t

01-sanity.t: CFLAGS += -std=c99 -pedantic -Werror

%.t: %.c ../mlinkedlist.c ../mlinkedlist.h tap.c/tap.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

check: tests
	prove .

Weverything: CC = clang
Weverything: CFLAGS += -Weverything -Wno-padded
Weverything: check

gcov: CC = gcc
gcov: CFLAGS += -fprofile-arcs -ftest-coverage
gcov: check
	gcov $(TESTS)

gprof: CC = gcc
gprof: CFLAGS += -pg
gprof: check

tests: $(TESTS)

all: tests

clean:
	$(RM) $(TESTS)
	$(RM) *.gcov *.gcda *.gcno gmon.out

.PHONY: all clean check gcov gprof tests Weverything
