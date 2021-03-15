
# The makefile for MP2.
# Type:
#   make         -- to build program lab3
#   make testlist -- to compile testing program
#   make clean   -- to delete object files, executable, and core
#   make design  -- check for simple design errors (incomplete)
#   make list.o  -- to compile only list.o (or: use lab3.o, task_support.o)
#
# You should not need to change this file.  
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line


CC     = gcc
CFLAGS = -O

lab3 : list.o task_support.o lab3.o
	$(CC) $(CFLAGS) list.o task_support.o lab3.o -o lab3

list.o : datatypes.h list.h
	$(CC) $(CFLAGS) -c list.c

task_support.o : task_support.h datatypes.h list.h
	$(CC) $(CFLAGS) -c task_support.c

lab3.o : datatypes.h list.h task_support.h
	$(CC) $(CFLAGS) -c lab3.c

geninput : geninput.o
	$(CC) $(CFLAGS) geninput.o -o geninput 

geninput.o : geninput.c
	$(CC) $(CFLAGS) -c geninput.c

.PHONY : design
design :
	@grep -e "-> *head" lab3.c task_support.c ||:
	@grep -e "-> *tail" lab3.c task_support.c ||:
	@grep -e "-> *current_list_size" lab3.c task_support.c ||:
	@grep -e "-> *list_sorted_state" lab3.c task_support.c ||:
	@grep -e "-> *next" lab3.c task_support.c ||:
	@grep -e "-> *prev" lab3.c task_support.c ||:
	@grep -e "-> *data_ptr" lab3.c task_support.c ||:
	@grep "comp_proc" lab3.c task_support.c ||:
	@grep "data_clean" lab3.c task_support.c ||:
	@grep "list_node_t" lab3.c task_support.c ||:
	@grep "task_id" list.c ||:
	@grep "priority" list.c ||:
	@grep -e "-> *state" list.c ||:
	@grep "wallclocktime" list.c ||:
	@grep "nargs" list.c ||:
	@grep "args" list.c ||:
	@grep -e "-> *output" list.c ||:
	@grep "task_" list.c ||:

.PHONY : clean
clean :
	rm -f list.o task_support.o lab3.o lab3 testlist.o testlist
