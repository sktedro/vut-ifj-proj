CC=gcc
CFLAGS=-g #-Wall -Wextra

LIBS=\
../char_buffer.h \
../int_buffer.h \
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
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@
	$(CC) $@/$@_tests.c $(LIBS) -o ../build/tests/$@/$@_tests $(CFLAGS)

.PHONY: token
token:
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@
	$(CC) $@/$@_tests.c $(LIBS) -o ../build/tests/$@/$@_tests $(CFLAGS)

.PHONY: symtable_tree
symtable_tree:
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@
	$(CC) $@/$@_tests.c $(LIBS) -o ../build/tests/$@/$@_tests $(CFLAGS)

.PHONY: symtable_stack
symtable_stack:
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@
	$(CC) $@/$@_tests.c $(LIBS) -o ../build/tests/$@/$@_tests $(CFLAGS)

