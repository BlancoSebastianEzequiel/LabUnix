#!/bin/bash

ROOT=./
DIR=functions/
string1='functions'
string2='functions'
ARCHC=$DIR/functions.c
ARCHH=$DIR/functions.h

for case in $(ls $DIR); do
    filename=$(basename ${case%%.*})

    if [[ $string1 = $filename ]]; then
    	  continue
    fi
    printf "Executing $filename...";
    gcc -std=c99 -Wall -Werror $ARCHC $ARCHH $DIR/$case -o $ROOT/$filename
    printf "OK\n";
done
