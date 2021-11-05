CC=gcc
CFLAGS=-g #-Wall -Wextra

LIBS=../scanner.h ../token.h ../buffer.h

.PHONY: none
none: 
	@echo Please choose which tests to compile

.PHONY: scanner
scanner:
	@echo Compiling scanner tests:
	@ mkdir -p ../build/tests/scanner
	$(CC) scanner/scanner_tests.c $(LIBS) -o ../build/tests/scanner/scanner_tests $(CFLAGS)

