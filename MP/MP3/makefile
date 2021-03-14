# The makefile for MP3.
# Type:
#   make          -- to build programs lab3 and geninput (also "make all")
#   make lab3     -- to build program lab3
#   make geninput -- to build the program to generate test input
#   make clean    -- to delete object files, executable, and core
#   make design   -- check for simple design errors (incomplete)
#   make list.o   -- to compile only list.o (or: use lab3.o, task_support.o)
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line

# CC is the compiler used to compile and link C programs
CC     = gcc
# CFLAGS are the compiler flags used when compiling C programs
CFLAGS = -Wall -g -DVALIDATE

.PHONY : all
all : lab3 geninput

lab3 : list.o atom_support.o lab3.o
	$(CC) -Wall -g list.o atom_support.o lab3.o -o lab3

list.o : list.c datatypes.h list.h
	$(CC) $(CFLAGS) -c list.c

atom_support.o : atom_support.c atom_support.h datatypes.h list.h
	$(CC) $(CFLAGS) -c atom_support.c

lab3.o : lab3.c datatypes.h list.h atom_support.h
	$(CC) $(CFLAGS) -c lab3.c

geninput : geninput.o
	$(CC) -Wall -g geninput.o -o geninput

geninput.o : geninput.c 
	$(CC) $(CFLAGS) -c geninput.c

.PHONY : design
design :
	@grep -e "-> *head" lab3.c atom_support.c ||:
	@grep -e "-> *tail" lab3.c atom_support.c ||:
	@grep -e "-> *current_list_size" lab3.c atom_support.c ||:
	@grep -e "-> *list_sorted_state" lab3.c atom_support.c ||:
	@grep -e "-> *next" lab3.c atom_support.c ||:
	@grep -e "-> *prev" lab3.c atom_support.c ||:
	@grep -e "-> *data_ptr" lab3.c atom_support.c ||:
	@grep "comp_proc" lab3.c atom_support.c ||:
	@grep "data_clean" lab3.c atom_support.c ||:
	@grep "list_node_t" lab3.c atom_support.c ||:
	@grep "atomic_num" list.c ||:
	@grep "mass" list.c ||:
	@grep "position *\[" list.c ||:
	@grep "momenta" list.c ||:
	@grep "force" list.c ||:
	@grep "potential_energy" list.c ||:
	@grep "atom_" list.c ||:

clean :
	rm -f *.o lab3 testlist geninput *.core a.out 

