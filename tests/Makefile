CC=gcc
CFLAGS=-g #-Wall -Wextra

LIBS=../binary_search_tree.h ../buffer.h ../misc.h ../scanner.h ../token.h

.PHONY: none
none: 
	@echo Please choose which tests to compile

.PHONY: scanner
scanner:
	@echo Compiling scanner tests:
	@mkdir -p ../build/tests/scanner
	$(CC) scanner/scanner_tests.c $(LIBS) -o ../build/tests/scanner/scanner_tests $(CFLAGS)

.PHONY: token
token:
	@echo Compiling token tests:
	@mkdir -p ../build/tests/token
	$(CC) token/token_tests.c $(LIBS) -o ../build/tests/token/token_tests $(CFLAGS)
