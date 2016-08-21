#!/bin/bash
FILES=*.mtx
rm grammar_log
for f in $FILES
do
	echo "Processing file $f" >> grammar_log
	./main -f $f -B >> grammar_log
done
