CC=gcc
CFLAGS=-Wall -Wextra -g


MAIN=main.c
LIBS=scanner.h token.h buffer.h

OUTPUTDIR=./build
OUTPUT=./build/main

.PHONY: all
all:
	mkdir -p $(OUTPUTDIR)
	$(CC) $(MAIN) $(LIBS) -o $(OUTPUT) $(CFLAGS)

.PHONY: run
run: all
	$(OUTPUT)

.PHONY: tests
tests:
	$(CC) test.c $(LIBS) -o ./build/test $(CFLAGS)

.PHONY: test
test: tests
	./build/test

