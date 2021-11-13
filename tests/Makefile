CC=gcc
CFLAGS=-g #-Wall -Wextra

LIBS=\
../charBuffer.h \
../intBuffer.h \
../misc.h \
../precedence_analysis.h \
../scanner.h \
../symbol_stack.h \
../symtable_tree.h \
../symtable.h \
../symtable_stack.h \
../token.h \


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

.PHONY: symtable_stack
symtable_stack:
	@echo Compiling token symtable_stack:
	@mkdir -p ../build/tests/symtable_stack
	$(CC) symtable_stack/symtable_stack_tests.c $(LIBS) -o ../build/tests/symtable_stack/symtable_stack_tests $(CFLAGS)

.PHONY: symtable_tree
symtable_tree:
	@echo Compiling token symtable_tree:
	@mkdir -p ../build/tests/symtable_tree
	$(CC) symtable_tree/symtable_tree_tests.c $(LIBS) -o ../build/tests/symtable_tree/symtable_tree_tests $(CFLAGS)
