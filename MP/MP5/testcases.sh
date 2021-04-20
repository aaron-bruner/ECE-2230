#!/bin/bash

# Script to run test cases

# Set values to be tested for initial shape, levels in the tree,
# and sort policy
shapes='o r p'
levels='5 10 15 20'
trials='1000 10000 100000 1000000'
policies='bst avl'

# Tests with 0 accesses that print out the resulting tree
./lab5 -o -w 5 -t 0 -v
./lab5 -r -w 5 -t 0 -v -s 1
./lab5 -p -w 5 -t 0 -v -s 2

# Test with 1000000 accesses
for s in $shapes
do
	for l in $levels
	do
		for t in $trials
		do
			for p in $policies
			do
				echo "Output for shape=$s, levels=$l, trials=$t, policy=$p"
				./lab5 -$s -w $l -t $t -f $p
			done
		done
	done
done