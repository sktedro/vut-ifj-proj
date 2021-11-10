CC=gcc
CFLAGS=-Wall -Wextra -g


MAIN=main.c
LIBS=scanner.h token.h buffer.h symbol_stack.h precedence_analysis.h

TESTSCRIPT=./test.sh

OUTPUTDIR=./build



.PHONY: all
all:
	mkdir -p $(OUTPUTDIR)
	$(CC) $(MAIN) $(LIBS) -o $(OUTPUTDIR)/main $(CFLAGS)


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


clean:
	rm -rf build logs
