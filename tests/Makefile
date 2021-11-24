CC=gcc
CFLAGS=-g #-Wall -Wextra


LIBS=\
../char_buffer.c \
../int_buffer.c \
../misc.c \
../parser.c \
../precedence_analysis.c \
../scanner.c \
../symbol_stack.c \
../symtable.c \
../symtable_stack.c \
../symtable_tree.c \
../token.c


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

.PHONY: symtable
symtable:
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@
	$(CC) $@/$@_tests.c $(LIBS) -o ../build/tests/$@/$@_tests $(CFLAGS)
