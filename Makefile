CC=gcc
CFLAGS=-Wall -Wextra -g


MAIN=main.c
LIBS=scanner.h token.h buffer.h

OUTPUTDIR=./build
OUTPUT=./build/main

all:
	mkdir -p $(OUTPUTDIR)
	$(CC) $(MAIN) $(LIBS) -o $(OUTPUT) $(CFLAGS)

run: all
	$(OUTPUT)

tests:
	$(CC) test.c $(LIBS) -o ./build/test $(CFLAGS)

test: tests
	./build/test

