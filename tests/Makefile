CC=gcc
CFLAGS=-g -Wno-unused-result 


LIBS=\
../linked_list.c \
../built_in_functions.c \
../dynamic_char_array.c \
../generator.c \
../dynamic_int_array.c \
../misc.c \
../parser.c \
../precedence_analysis.c \
../scanner.c \
../dynamic_string_array.c \
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

