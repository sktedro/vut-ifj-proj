cmake_minimum_required(VERSION 3.15)
project(vut-igj-proj)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(
        SOURCES
        main.c
)

set(
        HEADERS
        misc.h
        charBuffer.h
        intBuffer.h
        precedence_analysis.h
        scanner.h
        symtable_stack.h
        symbol_table_tree.h
        symtable.h
        symbol_stack.h
        token.h
)


add_executable(ifj21 ${SOURCES})

