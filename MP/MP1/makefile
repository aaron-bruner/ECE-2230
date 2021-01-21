# The makefile for MP1.
CC     = gcc
CFLAGS = -Wall -g

lab1: atom_list.o

lab1.o: atom_list.h

atom_list.o: atom_list.h

.PHONY: test
test: lab1
	./lab1 10 < input1.txt

.PHONY: clean
clean:
	rm -f atom_list.o lab1.o lab1
