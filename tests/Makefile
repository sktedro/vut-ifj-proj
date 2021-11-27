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
../generator.c \
../symtable_stack.c \
../symtable_tree.c \
../token.c


.PHONY: none
none: 
	@echo Please choose which tests to compile

%:
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@_tests
	$(CC) $@_tests/$@_tests.c $(LIBS) -o ../build/tests/$@_tests/$@_tests $(CFLAGS)
	# ./test.sh $@
