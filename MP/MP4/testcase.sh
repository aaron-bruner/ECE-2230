#! /bin/sh

# Run each case without coalescing
for u in 0 1 2 3 4 5 6
do
	for s in best first worst
	do
		./lab4 -s -f $s -u $u > testcases/without_coalescing/output$u$s.txt
	done
done


# Run each case with coalescing
for u in 0 1 2 3 4 5 6
do
	for s in best first worst
	do
		./lab4 -s -f $s -c -u $u > testcases/with_coalescing/output$u$s.txt
	done
done
