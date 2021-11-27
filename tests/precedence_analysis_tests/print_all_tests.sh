#!/bin/sh

cat ./inputs/* | grep -v "end" | grep -v "^$" | cat -n
