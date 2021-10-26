#!/bin/sh

Help()
{
  echo "Usage: test [-h|--help]"
  echo "       test [TARGET] [COMMAND]"
  echo
  echo "TARGET"
  echo "  TOKEN         token test"
  echo
  echo "COMMAND"
  echo "  -a ALL        all tests to selected target"
  echo
  echo "  -b BASIC      just testing with basic tests selected target"
  echo
  echo "  -e EXPRESSION testing how good scanner will work with EXPRESSION"
  echo "                THIS COMMAND IS AVAILABLE JUST FOR TOKEN AS TARGET"
  echo
  echo "  -c COMMENT    testing single-line and multi-line comments"
  echo "                THIS COMMAND IS AVAILABLE JUST FOR TOKEN AS TARGET"
  echo
  echo "  -s STRING     testing how scanner deals with strings"
  echo "                THIS COMMAND IS AVAILABLE JUST FOR TOKEN AS TARGET"
  echo

}