#!/bin/bash

PROG=./original.out
COUNT=`ls -1q in_* | wc -l`
A=1

for IN_FILE in in_* ; do
	REF_FILE=`echo -n $IN_FILE | sed -e 's/in/out/'`
	$PROG < $IN_FILE > $REF_FILE;
	echo "[$A/$COUNT] Generated $REF_FILE from $IN_FILE";
	A=$((A+1));
done 
