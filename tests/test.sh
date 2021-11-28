#!/bin/bash

# COLORS
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BLUE=`tput setaf 4`
NC=`tput sgr0` # No color

help() {
  echo "Usage: test [-h|--help]"
  echo "       test [TARGET]"
  echo
  echo "TARGET"
  echo "  ALL           run all tests"
  echo "  TOKEN         token tests"
  echo "  SCANNER       scanner tests"
  echo "  ..."
  echo
}

runTest() {
  # This is the test case name and at the same time, folder in which the tests are
  echo ${BLUE}========================================
  echo Testing $(echo "$testTarget" | tr a-z A-Z) "(compile and run tests)": 
  echo ========================================${NC}

  # Just save the folder names
  inputsFolder="$testTarget"/inputs
  referenceFolder="$testTarget"/reference
  outputsFolder="$testTarget"/outputs
  logFolder=../logs/tests/"$testTarget"

  # Create the outputs folder if it does not exist
  mkdir -p $outputsFolder
  # Create the logs folder if it does not exists
  mkdir -p $logFolder

  # Compile the tests
  make "$testTarget"
  testFile=../build/tests/"$testTarget"/"$testTarget"


  echo ${BLUE}========================================${NC}

  # Check if a test case exists
  if [ ! -n "$(ls -A "$inputsFolder" 2>/dev/null)" ]; then
    echo ${RED}There are no inputs for test target \'"$testTarget"\'
    exit 1
  fi

  # Run tests for each file in $inputsFolder
  for input in "$inputsFolder"/*; do

    # Get the input file name (without the path)
    inputName=$(echo $input | rev | cut -d '/' -f 1 | rev)

    # Check if a reference file to the input exists
    if [ ! -f "$referenceFolder"/"$inputName" ]; then
      echo ${RED}Reference output to test "$inputName" for "$testTarget" does not exist!
      echo Would you like to run the test anyway? [y/n]${NC}
      read run
      if [ "$run" != "y" ] && [ "$run" != "Y" ]; then
        echo ${RED}Tests cancelled
        exit 1
      else
        run=y
      fi
    fi

    # Run the test (save stdout AND STDERR to the output folder)
    echo ${BLUE}Testing "$inputName" input ${NC}
    "$testFile" < "$input" > "$outputsFolder"/"$inputName" 2>&1

    # If there was no reference output, stop here
    if [ "$run" = "y" ]; then
      echo ${GREEN}Test output was written to outputs/ folder ${NC}
      continue
    fi

    # Compare the generated output with the reference output
    output=$(diff -s "$outputsFolder"/"$inputName" "$referenceFolder"/"$inputName")
    diffReturnCode="$?"

    # Diff returns 0 if files are the same
    if [ "$diffReturnCode" -eq 0 ]; then
      echo ${GREEN}Test passed${NC}
    else
      echo ${RED}"Test not passed. Diff:"${NC}
      echo diff is also saved to logs/tests/
      echo "$output"
      echo "$output" > "$logFolder"/"$inputName"_diff
      returnCode="$diffReturnCode"
    fi
  done

  echo ${BLUE}========================================
  echo Testing $(echo "$testTarget" | tr a-z A-Z) done
  echo ========================================
  echo ========================================${NC}

  if [ -n "$returnCode" ]; then
    exit 1
  fi
}


if [ -z "$1" ]; then
  echo ${RED}Please type targets as arguments ${NC}
  help
fi


case $1 in

  -h | --help)
    help
    exit 0
    ;;

  *)
    # Convert to lowercase
    testTarget=$(echo "$1""_tests" | tr A-Z a-z)

    if [ "$testTarget" = "all_tests" ]; then
      make all
      exit "$?"
    fi

    # If there is no directory containing tests for that target
    if [ ! -d ./"$testTarget" ]; then
      echo ${RED}Tests for target \'"$testTarget"\' do not exist!
    else
      runTest
      exit "$?"
    fi
    ;;

esac
