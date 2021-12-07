CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g -Wno-unused-parameter

MAIN=main.c
LIBS=\
assignment.c \
built_in_functions.c \
char_buffer.c \
generator.c \
int_buffer.c \
misc.c \
precedence_analysis.c \
scanner.c \
string_buffer.c \
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
	cd pack && zip xskalo01.zip ./*
	mv pack/xskalo01.zip ./
	rm -rf pack

clean:
	rm -rf build logs pack
