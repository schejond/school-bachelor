#!/bin/bash

PROG=./a.out

for IN_FILE in in_* ; do
	REF_FILE=`echo -n $IN_FILE | sed -e 's/in/out/'`
	$PROG < $IN_FILE > my_out.txt
	if ! diff $REF_FILE my_out.txt ; then 
		echo "Fail: $IN_FILE";
	else
		echo "OK: $IN_FILE";
	fi
done 
