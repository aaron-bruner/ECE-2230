# The makefile for MP1.
CC     = gcc
CFLAGS = -Wall -g

lab1: atom_list.o

lab1.o: atom_list.h

atom_list.o: atom_list.h

.PHONY: test1
test1: lab1
	./lab1 10 < input1.txt > myoutput1.txt
	diff myoutput1.txt input_output/output1.txt

.PHONY: test2
test2: lab1
	./lab1 10 < input2.txt > myoutput2.txt
	diff myoutput2.txt input_output/output2.txt

.PHONY: test3
test3: lab1
	./lab1 10 < input3.txt > myoutput3.txt
	diff myoutput3.txt input_output/output3.txt

.PHONY: clean
clean:
	rm -f atom_list.o lab1.o lab1 myoutput1.txt myoutput2.txt myoutput3.txt

.PHONY: valgrind1
valgrind1:
	valgrind --leak-check=yes ./lab1 10 < input_output/input1.txt

.PHONY: valgrind2
valgrind2:
	valgrind --leak-check=yes ./lab1 10 < input_output/input2.txt

.PHONY: valgrind3
valgrind3:
	valgrind --leak-check=yes ./lab1 10 < input_output/input3.txt

.PHONY: valgrind4
valgrind4:
	valgrind --leak-check=yes ./lab1 10 < input_output/input4.txt