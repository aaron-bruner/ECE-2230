#!/bin/bash

# test_cases.sh
# Casey Hird
# crhird
# ECE 2230 Spring 2019
# MP3

# Script to run test cases


# set these lists with the parameter space's values
sizes4='3000 6000 9000 12000 15000'
sizesmerge='50000 250000 450000 650000 850000'
# descending,  ascending, random
orders='1 2 3'

# sorting algorithm IDs
sorts='1 2 3 4'

# sort output orders
outputs='1 2'

# First run each sort function with a memory check and save valgrind output
./geninput 20 3 1 2 | valgrind --leak-check=yes --log-file='valgrind_output1.txt' ./lab3 1
./geninput 20 3 2 2 | valgrind --leak-check=yes --log-file='valgrind_output2.txt' ./lab3 1
./geninput 20 3 3 2 | valgrind --leak-check=yes --log-file='valgrind_output3.txt' ./lab3 1
./geninput 20 3 4 2 | valgrind --leak-check=yes --log-file='valgrind_output4.txt' ./lab3 1
./geninput 20 3 5 2 | valgrind --leak-check=yes --log-file='valgrind_output5.txt' ./lab3 1

# Run test for first 4 functions
for b in $outputs
do
	for s in $sorts
	do
   		echo "Method: $s"

	    for o in $orders
  		do
        	echo "Data Order: $o"
        	for n in $sizes4
        	do
            	echo $n $o $s
            	./geninput $n $o $s $b | ./lab3 1 >> test_output.txt
			done
    	done
	done
done
# Run test for function 5 (merge sort)
for b in $outputs
do
	for o in $orders
	do
		for y in $sizesmerge
		do
			./geninput $y $o 5 $b | ./lab3 1 >> test_output.txt
		done
	done
done
