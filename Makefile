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

TESTSCRIPT=./test.sh

OUTPUTDIR=./build



.PHONY: all
all:
	mkdir -p $(OUTPUTDIR)
	$(CC) $(MAIN) $(LIBS) -o $(OUTPUTDIR)/main $(CFLAGS)
	# $(CC) $(MAIN) $(LIBS) -o main $(CFLAGS)


.PHONY: run
run: all
	$(OUTPUTDIR)/main


# Run tests for a target given by an argument (eg. make test target=scanner)
# TODO
.PHONY: test
test:
	@echo To run tests from the base directory, run eg. \'make test target=scanner\'
	@cd tests && \
	$(TESTSCRIPT) $(target)

pack: clean
	mkdir -p pack
	cp *.c *.h Makefile pack/
	cd pack && zip xskalo01.zip ./*
	mv pack/xskalo01.zip ./
	rm -rf pack

clean:
	rm -rf build logs pack
