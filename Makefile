CC=gcc
CFLAGS=-Wall -Wextra -g


MAIN=main.c
LIBS=scanner.h token.h buffer.h

TESTSCRIPT=./tests/test.sh

OUTPUTDIR=./build



.PHONY: all
all:
	mkdir -p $(OUTPUTDIR)
	$(CC) $(MAIN) $(LIBS) -o $(OUTPUTDIR)/main $(CFLAGS)


.PHONY: run
run: all
	$(OUTPUTDIR)/main


# Run all tests (for individual tests please run test.sh individually..)
.PHONY: test
test:
	$(TESTSCRIPT) -a


clean:
	rm -rf build logs
