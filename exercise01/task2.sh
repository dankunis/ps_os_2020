#!/bin/bash
INFILE=$1
OUTFILE=$2

USAGE="$0 <arg#1> <arg#2>"

if [ "$#" -ne 2 ]
then
  echo "Illegal number of parameters. See usage:"
  echo "$USAGE"
  exit
fi

if [ -e "$INFILE" ]
then
  if [ -w "$OUTFILE" ]
  then
    cat "$INFILE" >> "$OUTFILE"
  else
    echo "File is not writable" > error.log
  fi
fi
