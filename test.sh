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
  echo "  -cd CODE      testing with IFJ21 teal code"
  echo

}

All() {
  echo "$TOKEN"
  echo "ALL"
}

Basic() {
  echo "$TOKEN"
  echo "ALL"
}

Code() {
  echo "CODE"
}

Custom() {
  echo "CUSTOM"
}

EXEC=""

while [ "$#" -gt 0 ]; do
  case $1 in
    -h | --help)
      help
      exit 0
      ;;
    TOKEN | token)
      TOKEN="true"
      shift
      ;;
    -a | all | ALL)
      All
      exit 0
      ;;
    -b | basic | BASIC)
      Basic
      exit 0
      ;;
    -c | comment | COMMENT)
      EXEC="${EXEC} ${1}"
      echo "$EXEC"
      shift
      ;;
    -s | string | STRING)
        EXEC="${EXEC} ${1}"
        echo "$EXEC"
        shift
        ;;
    -cd | code | CODE)
        Code
        shift
        ;;
    *)
      echo "ERROR"
      exit 1;
  esac
done
Custom

