#!/usr/bin/env bash

make --silent --no-print-directory -C .. all

for (( i=0; i<=100; i++ ))
do
	if ! [ -f "inputs/input$i" ]; then
		continue
	fi
	../hw2.exe <"inputs/input$i" >'test.out'
	diff 'test.out' "outputs/output$i" >/dev/null
	if [ $? -ne 0 ]; then
		echo "input$i failed"
	fi
done

make --silent --no-print-directory -C .. clean
