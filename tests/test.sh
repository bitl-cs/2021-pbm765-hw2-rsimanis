#!/usr/bin/env bash

make --silent --no-print-directory -C .. all

for (( i=0; i<=1; i++ ))
do
	../hw2.exe <'inputs/input'"$i" >'test.out'
	diff 'test.out' 'outputs/output'"$i" >/dev/null
	if [ $? -ne 0 ]; then
		echo "input$i failed"
	fi
done

make --silent --no-print-directory -C .. clean
