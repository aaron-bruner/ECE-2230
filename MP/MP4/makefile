# makefile for MP4
#
# -lm is used to link in the math library

CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lm

lab4 : lab4.o mem.o

lab4.o : lab4.c mem.h

mem.o : mem.c mem.h

.PHONY: clean
clean :
	rm -f mem.o lab4.o lab4 testcases/combined_with_and_without_coalescing.txt testcases/with_coalescing/combined_with_coalescing.txt testcases/without_coalescing/combined_without_coalescing.txt

.PHONY: test
test :
	./testcase.sh

.PHONY: combine
combine :
	cat testcases/with_coalescing/output* > testcases/with_coalescing/combined_with_coalescing.txt
	cat testcases/without_coalescing/output* > testcases/without_coalescing/combined_without_coalescing.txt
	cat testcases/with_coalescing/combined_with_coalescing.txt testcases/without_coalescing/combined_without_coalescing.txt > testcases/combined_with_and_without_coalescing.txt
