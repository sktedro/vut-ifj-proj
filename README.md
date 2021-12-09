# vut-ifj-proj

A basic compiler for teal-like "IFJ21" programming language written in C -
generating assembler-like "IFJcode21" code. 

# Authors

- Patrik Skaloš (xskalo01)
- Jana Kováčiková (xkovac59)
- Alexaner Okrucký (xokruc00)
- Jiřina Frýbortová (xfrybo01)

# Known bugs, issues and unfinished functionality

1. Built in functions are wrong (substr is definitely not working, chr is
   probably not working, the rest is not well tested but should work)
2. Multiassignment is not working properly in certain cases and throws an error
   without a reason
3. A number literal without a floating point part (eg. 15.e-1) is acceptable by
   the scanner, but should not be!
4. No runtime error detections implemented (eg. dividing by a variable which 
   represents a zero number) 
5. The compiler might return a wrong error code (syntax err) when function 
   parameter amount or types don't match

# Makefile options

## run

Runs the compiler

## tests

Runs all unit tests

## test

Runs compiler tests by Ondroid

## pack

Packs everything that is to be submitted to a zip file (xskalo01.zip)

## clean

Removes build/, logs/ and pack/ folders
