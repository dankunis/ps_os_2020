#!/bin/bash

# check parameters

USAGE="$0 <arg#1>"

if [ "$#" -ne 1 ]
then
  echo "Illegal number of parameters. See usage:"
  echo "$USAGE"
  exit
fi

# check if OUTDIR exists and create it otherwise

OUTDIR=$1

if ! mkdir -p $OUTDIR
then
  echo "mkdir failed. terminating execution"
  exit
fi

# iterate over current directory

for ENTRY in *
do
  # check if ENTRY is a regular file
  if [ ! -f $ENTRY ]
  then
    continue
  fi

  # check if current version of file is in OUTDIR

  FILEBACKUP="$OUTDIR/$ENTRY"

  # && [ -f $FILEBACKUP ] && [ ! "$ENTRY" -nt "$FILEBACKUP" ]
  if [ -f $FILEBACKUP ] && [ ! "$ENTRY" -nt "$FILEBACKUP" ]
  then
    continue
  fi

  # save the file

  echo "saving $ENTRY"
  echo "$ENTRY" > "$FILEBACKUP"
done
