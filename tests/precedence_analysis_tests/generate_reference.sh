#!/bin/bash

# COLORS
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BLUE=`tput setaf 4`
NC=`tput sgr0` # No color

inputsFolder=./inputs
outputsFolder=./outputs
referenceFolder=./reference

for input in ./inputs/*; do
  filename=$(echo "$input" | cut -d '/' -f 3)
  if [ ! -f "$referenceFolder"/"$filename" ]; then
    input=$(cat "$inputsFolder"/"$filename" | head -n 1)
    output=$(cat "$outputsFolder"/"$filename")

    printf ${BLUE}"\n==================================================\n\n"${NC}
    printf "Test: "${GREEN}"$filename""\n\n"${NC}
    printf ${RED}"Input: "${NC}"\n"
    printf "$input""\n\n"
    printf ${RED}"Output:\n"${NC}
    cat "$outputsFolder"/"$filename"
    printf "\n"
    printf ${RED}"Is the output right? [y to confirm] "${NC}
    read confirmation
    if [ "$confirmation" = "y" ]; then
      echo ${GREEN}"Copying the output to the reference folder\n"${NC}
      cp "$outputsFolder"/"$filename" "$referenceFolder"
    fi
  else
    echo "Reference output for ""$input"" exists"
  fi
done
