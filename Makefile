# VUT FIT - IFJ 2021
#
# Patrik Skaloš (xskalo01)
# Jana Kováčiková (xkovac59)
# Alexaner Okrucký (xokruc00)
# Jiřina Frýbortová (xfrybo01)

CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g -Wno-unused-parameter

MAIN=main.c
LIBS=\
linked_list.c \
built_in_functions.c \
dynamic_char_array.c \
generator.c \
dynamic_int_array.c \
misc.c \
precedence_analysis.c \
scanner.c \
dynamic_string_array.c \
symbol_stack.c \
symtable.c \
symtable_stack.c \
symtable_tree.c \
token.c
# why is parser not included as a lib, you might ask? Cause if you include it,
# the compiler won't compile but throw a thousand errors instead :)))

TESTSDIR=./tests
TESTSSCRIPT=./test.sh

TESTDIR=./ifjtest
TESTSCRIPT=./ifjtest

OUTPUTDIR=./build


.PHONY: all
all:
	mkdir -p $(OUTPUTDIR)
	$(CC) $(MAIN) $(LIBS) -o $(OUTPUTDIR)/main $(CFLAGS)
	# $(CC) $(MAIN) $(LIBS) -o main $(CFLAGS)

.PHONY: run
run: all
	$(OUTPUTDIR)/main

.PHONY: tests
tests: all
	@echo Running all unit tests
	cd $(TESTSDIR) && $(TESTSSCRIPT) all

.PHONY: test
test: all
	@echo Running tests by Ondroid
	cd $(TESTDIR) && $(TESTSCRIPT)
	

pack: clean
	mkdir -p pack
	cp *.c *.h Makefile pack/
	cp misc/rozdeleni pack/
	cp doc/doc.pdf pack/dokumentace.pdf
	cd pack && zip xskalo01.zip ./*
	mv pack/xskalo01.zip ./
	rm -rf pack

clean:
	rm -rf build logs pack main xskalo01.zip
