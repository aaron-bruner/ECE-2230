# The makefile for MP1.
CC     = gcc
CFLAGS = -Wall -g

lab1: atom_list.o

lab1.o: atom_list.h

atom_list.o: atom_list.h

.PHONY: test1
test1: lab1
	./lab1 10 < input1.txt > myoutput1.txt
	diff myoutput1.txt output1.txt

.PHONY: test2
test2: lab1
	./lab1 10 < input2.txt > myoutput2.txt
	diff myoutput2.txt output2.txt

.PHONY: test3
test3: lab1
	./lab1 10 < input3.txt > myoutput3.txt
	diff myoutput3.txt output3.txt

.PHONY: clean
clean:
	rm -f atom_list.o lab1.o lab1

.PHONY: valgrind1
valgrind1:
	valgrind --leak-check=yes ./lab1 10 < input1.txt

.PHONY: valgrind2
valgrind2:
	valgrind --leak-check=yes ./lab1 10 < input2.txt

.PHONY: valgrind3
valgrind3:
	valgrind --leak-check=yes ./lab1 10 < input3.txt
