CC=gcc
CFLAGS=-g -Wno-unused-result 


LIBS=\
../assignment.c \
../built_in_functions.c \
../char_buffer.c \
../generator.c \
../int_buffer.c \
../misc.c \
../parser.c \
../precedence_analysis.c \
../scanner.c \
../string_buffer.c \
../symbol_stack.c \
../symtable.c \
../symtable_stack.c \
../symtable_tree.c \
../token.c


.PHONY: none
none: 
	@echo Please choose which tests to compile

all:
	make token
	make scanner
	make symtable_tree
	make symtable_stack
	make symtable
	make precedence_analysis

%:
	@echo Compiling token $@:
	@mkdir -p ../build/tests/$@_tests
	$(CC) $@_tests/$@_tests.c $(LIBS) -o ../build/tests/$@_tests/$@_tests $(CFLAGS)
	./test.sh $@

