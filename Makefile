CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

MAIN=main.c
LIBS=\
char_buffer.c \
int_buffer.c \
misc.c \
precedence_analysis.c \
scanner.c \
symbol_stack.c \
generator.c \
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
tests:
	@echo Running all unit tests
	cd $(TESTSDIR) && $(TESTSSCRIPT) all

.PHONY: test
test:
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
