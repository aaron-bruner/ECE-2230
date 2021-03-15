/* task_support.c
 * Casey Hird
 * crhird
 * ECE 2230 Spring 2018
 * MP3
 *
 * Propose: Implements the doubly linked list defined in list.h and list.c
 * create unsorted and sorted list of type data_t.
 *
 * Assumptions: All functions in list.c are implemented correctly.
 *
 * Bugs: No known bugs.
 *
 * You can modify the interface for any of the task_ functions if you like
 * But you must clearly document your changes.
 *
 * (You CANNOT modify any of the details of the list.h interface, or use any
 *  of the private variables outside of list.c.)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

#include "datatypes.h"
#include "list.h"
#include "task_support.h"

#define FALSE 0
#define TRUE  1

// private functions for use in task_support.c only
void fill_task_record(task_t *new);          // collect input
int* getDetermineArgs(int* num_args);        // get input for "DETERMINE" cmd
char* state2str(enum state state);           // Converts enum to ASCII
void print_task_rec_short(task_t *rec);      // print one record
void print_task_rec_long(task_t *rec) ;      // print one record

// My helper functions
task_t* find_id(list_t* list_ptr, int id, int *index);

/*
	Creates and adds a task with only a priority to the given list.
	The priority of this task is equal to the value provided and
	all other values are equal to zero.

	list_ptr: the list to be added to

	priority: the priority of the task being created and added

	postcondition: a task with priority = priority and all other
	values = 0 is added to the tail of list_ptr
*/
void task_list_add_tail_mp3(list_t * list_ptr, int priority)
{
	// Allocate memory with all vallues intialized to zero
	task_t *rec_ptr = (task_t*) calloc(1, sizeof(task_t));
	// Set priority
	rec_ptr->priority = priority;
	// Add to tail
	list_insert(list_ptr, rec_ptr, list_size(list_ptr));
}

/* Function to sort a list using a given method and direction
 *   TODO: recomment
 */
void task_list_sort(list_t ** list_ptr, int sort_type, int sort_order)
{
    /*TODO: sort the list */
    
    clock_t start, end;
    double elapse_time;  // time in milliseconds
    int initial_size = list_size(*list_ptr);
    start = clock();
    
    list_sort(list_ptr, sort_type, sort_order);

    
    end = clock();
    elapse_time =  1000.0 * ((double) (end - start)) / CLOCKS_PER_SEC;
    assert(list_size(*list_ptr) == initial_size);
    printf("Sorting: n = %d time = %f type = %d order = %d\n", initial_size, elapse_time, sort_type, sort_order);
    

}


/* task_compare is required by the list ADT for sorted lists.
 *
 * This function returns
 *     1 if rec_a should be closer to the head than rec_b,
 *    -1 if rec_b is to be considered closer to the head, and
 *     0 if the records are equal.
 *
 * For the task data we want to sort from lowest task ID up, so
 * closer to the front means a smaller task ID.
 *
 * The function expects pointers to two record structures, and it is an error
 * if either is NULL
 *
 * THIS FUNCTION SHOULD NOT BE CHANGED
 */
int task_compare(const task_t *record_a, const task_t *record_b)
{
    assert(record_a != NULL && record_b !=NULL);
    if (record_a->priority < record_b->priority)
        return 1;
    else if (record_a->priority > record_b->priority)
        return -1;
    else
        return 0;

}

/* Function to pass into List ADT during construction to free the memory
 * associate with a task record.  This hides the details of how the record
 * is defined from the List ADT.
 */
void task_rec_cleanup(task_t *rec)
{
    if (rec != NULL) {
        free(rec->args);
        free(rec);
    }
}



/* This creates a list and it can be either a sorted or unsorted list
 *
 * This function is provided to you as an example of how to use
 * function pointers.
 */
list_t * task_list_create(void)
{
    return list_construct(task_compare, task_rec_cleanup);
}

/* this function frees the memory for either a sorted or unsorted list.
 */
void task_list_cleanup(list_t * list_ptr)
{
    list_destruct(list_ptr);
}


/* print one of the task record lists
 *
 * inputs:
 *    list_ptr: a list_t * pointer to either sorted or unsorted list
 *
 *    type_of_list: a charter string that must be either "List" or "Queue"
 *
 * output: prints the list in the format required for grading.
 *         Do not change any of these lines (i.e. printfs && print_task_rec_short()
 */
void task_list_print(list_t * list_ptr, char *list_type)
{
    assert(strcmp(list_type, "List")==0 || strcmp(list_type, "Queue")==0);

    int i, num_in_list;

    num_in_list = list_ptr->current_list_size; // fix
    task_t *rec_ptr = NULL;  // fix


    if (num_in_list == 0) {
        printf("%s empty\n", list_type);
    } else {
        printf("%s has %d record%s\n", list_type, num_in_list,
                num_in_list==1 ? "." : "s");

        for (i = 0; i < num_in_list; i++) {
            printf("%4d: ", i);
            rec_ptr = list_access(list_ptr, i);   // fix to grab record at pos i!
            print_task_rec_short(rec_ptr);
        }
    }
    rec_ptr = NULL;
}


/* the function takes a pointer to each list and prints the
 * number of items in each list
 */
void task_list_stats(list_t * sorted, list_t * unsorted, list_t* mp3)
{
    // TODO: get the number in list and size of the list
    int num_in_sorted_list = 0;
    int num_in_unsorted_list = 0;
    int order_of_list = 0; // fix
    int num_in_mp3_sorted_list = -1;
    char* sort_order_mp3;

    switch (list_order(mp3)) {
        case 1:
            sort_order_mp3 = "Ascending";
            break;
        case -1:
            sort_order_mp3 = "Descending";
            break;
        default:
            sort_order_mp3 = "Unsorted";
    }
    printf("[MP3] Number records: %d, Order: %s\n", num_in_mp3_sorted_list, sort_order_mp3);

    printf("Number records: %d, Order: %s\n", num_in_sorted_list,
            order_of_list == 1 ? "Ascending" : "Descending");
    printf("Number records in queue: %d\n", num_in_unsorted_list);
}








/* This function adds an task to the list.
 *
 * Otherwise, if the list is full the task is rejected.
 */
void task_list_add(list_t * list_ptr, int max_list_size)
{
    task_t *rec_ptr = (task_t*) malloc(sizeof(task_t));  // fix
    fill_task_record(rec_ptr);

    // determine the correct return value
    int added_return = -2;

    // Check that the list has not already reached max size
  	if (list_size(list_ptr) >= max_list_size){
  		added_return = -1;
  	}
  	// Otherwise, add the element to the list
  	else{
  		// If the list is unsorted, add to the end
  		if (list_order(list_ptr) == 0){
  			added_return = 1;
  			list_insert(list_ptr, rec_ptr, max_list_size);
  		}
  		// Otherwise add using the sorted function
  		else{
  			list_insert_sorted(list_ptr, rec_ptr);
  			added_return = 1;
  		}
  	}

    if (added_return == 1) {
        printf("Inserted: %d\n", rec_ptr->task_id);
    } else if (added_return == -1) {
        printf("Rejected: %d\n", rec_ptr->task_id);
    } else {
        printf("Error with return value! Fix your code.\n");
    }

    //TODO: clean up
    rec_ptr = NULL;
}



/* This function prints the first task with the matching id in the
 * list.
*/
void task_list_lookup_id(list_t * list_ptr, int id)
{
    int index = -1;
    task_t *rec_ptr = NULL;   // fix to use

    // Find the first task with the given id
  	rec_ptr = find_id(list_ptr, id, &index);

    if (rec_ptr == NULL) {
        printf("Did not find: %d\n", id);
    } else {
        printf("Found: %d at index %d\n", id, index);
        print_task_rec_long(rec_ptr);
    }
}



/* This function removes the first task from the list with the matching
 * task id
 */
void task_list_remove_id(list_t * list_ptr, int id)
{
    int index = -1;
    task_t *rec_ptr = NULL;   // TODO: fix

    // Find the first task with the given id
  	rec_ptr = find_id(list_ptr, id, &index);

    if (rec_ptr == NULL) {
        printf("Did not remove: %d\n", id);
    } else {
      // Remove the found task
  list_remove(list_ptr, index);
        printf("Removed: %d\n", id);
        print_task_rec_long(rec_ptr);
        // Cleanup
		task_rec_cleanup(rec_ptr);
    }
    rec_ptr = NULL;
}


/* This function updates the state of the task identified by id if possible */
void task_list_update(list_t * list_ptr, int id, int state)
{
	int index = -1;
    task_t* rec_ptr = find_id(list_ptr, id, &index);
	enum state old_state;

    if (rec_ptr != NULL){
		old_state = rec_ptr->state;
		// If task is finished do nothing
		if (old_state == FINISHED){
			assert(old_state == FINISHED);
		}
		// If task is queued and the given state is running, set it to running
		else if (old_state == QUEUED && state == RUNNING){
			rec_ptr->state = RUNNING;
		}
		// If task is running
		else if (old_state == RUNNING){
			// If given state is finished, set it
			if (state == FINISHED){
				rec_ptr->state = FINISHED;
			}
			// If given state is blocked, set it
			else if (state == BLOCKED){
				rec_ptr->state = BLOCKED;
			}
		}
		// If task is blocked and given state is running, set it to running
    	else if (state == BLOCKED && state == RUNNING){
			rec_ptr->state = RUNNING;
		}
        // displays the current state for the task even if not changed
        printf("Task %d has state of %s\n", id, state2str(rec_ptr->state));
    }
    rec_ptr = NULL;

}


/* This function schedules based on id and priority */
void task_list_schedule(list_t * list_ptr, int id, int priority)
{
    task_t* rec_ptr = NULL;
    int i = 0;

	// Search for task with matching id to schedule
	int length = list_size(list_ptr);
	int found = 0;
	while (found == 0 && i < length){
		rec_ptr = list_access(list_ptr, i);
		if (rec_ptr->task_id == id){
			found = 1;
		}
		else{
			rec_ptr = NULL;
			i++;
		}
	}
	// If id is not found, schedule task with given priority
	if (rec_ptr == NULL){
		found = 0;
		i = 0;
		while (found == 0 && i < length){
			rec_ptr = list_access(list_ptr, i);
			if (rec_ptr->priority == priority){
				found = 1;
			}
			else{
				rec_ptr = NULL;
				i++;
			}
		}
	}

    if (rec_ptr != NULL)
    {
        printf("Now running task:\n");
		// Schedule the task to run
		enum state new_state = RUNNING;
		task_list_update(list_ptr, id, new_state);
        print_task_rec_short(rec_ptr);

    }
    else {
        printf("Did not find task to schedule...\n");
    }

}

/* This function determins if a task is runable */
void task_list_determine(list_t * list_ptr)
{
    int num_args;
    int idx = -1;
    int* args = getDetermineArgs(&num_args);

    int i, j;
    task_t* task = NULL;
	int length = list_size(list_ptr);
	i = 0;
	// Find first runnable task
	while (i < length && idx == -1){
		task = list_access(list_ptr, i);
		j = 0;
		while (j < num_args && task != NULL){
			if (task->args[j] == args[j]){
				j++;
			}
			else{
				task = NULL;
			}
		}
		if (j == num_args){
			idx = i;
		}
		else{
			i++;
		}
	}

    if (idx != -1) {
        task = list_access(list_ptr, idx);
        printf("Task %d is runable.\n", task->task_id);
    }
    else {
        printf("No runable tasks.\n");
    }

    free(args);
}

void task_list_remove_finished(list_t * list_ptr)
{
    task_t* task = NULL;
    list_t* rm = task_list_create();

    int i;
	// Find all finished tasks then remove them from list_ptr
	// and add them to list rm
	for (i = 0; i < list_size(list_ptr); i++){
		if (list_access(list_ptr, i)->state == FINISHED){
			list_insert(rm, list_remove(list_ptr, i), i);
		}
	}

    int num_in_list = list_size(rm);
    printf("Removing %d finshed tasks\n", num_in_list);
    for (i = 0; i<num_in_list; i++) {
		task = list_access(rm, i);
        print_task_rec_short(task);
    }

    //TODO: cleanup
	task = NULL;
	list_destruct(rm);
}

/* This function reverses the order of the items in the list */
void task_list_reverse(list_t * list_ptr)
{
    list_reverse(list_ptr);
    int order_of_list = list_order(list_ptr);
    printf("List reversed, new order: %s\n",
            order_of_list == 1 ? "Ascending" : "Descending");
}





/****************************************************************************/
/****************************************************************************/

/*       Queue Functions                                                    */

/****************************************************************************/
/****************************************************************************/


/* This functions adds an task record to the tail of a list.  The list is
 * unsorted, and it does not have any limits on the maximum size of the list.
 */
void task_list_add_tail(list_t * list_ptr)
{
    task_t *rec_ptr = (task_t*) malloc(sizeof(task_t));
    fill_task_record(rec_ptr);          // collect input
    // Add to tail
	list_insert(list_ptr, rec_ptr, list_size(list_ptr));
    printf("Appended %d onto queue\n", rec_ptr->task_id);
}

/* This function removes the task record at the head of the list.  The
 * list is unsorted, and there is no limit on the maximum size of the list.
 *
 * The task should be recycled so there are no memory leaks.
 */
void task_list_remove_head(list_t * list_ptr)
{
    task_t *task_ptr = NULL;// fix
	// Remove and return the task at the head of the list
	task_ptr = list_remove(list_ptr, 0);

    if (task_ptr != NULL)
        printf("Deleted head with task id and priority: %d %d\n",
                task_ptr->task_id, task_ptr->priority);
    else
        printf("Queue empty, did not remove\n");
    task_rec_cleanup(task_ptr);
}

/* This function prints the task at the head of the queue.
 * Print all fields of the task record.
 */
void task_list_print_head(list_t * list_ptr)
{
    task_t *rec_ptr = NULL; //fix
    // Return task at the head of the list
  	rec_ptr = list_access(list_ptr, 0);

    if (rec_ptr == NULL) {
        printf("Queue is empty\n");
    } else {
        /* print items in structure */
        printf("Found at head of queue:\n");
        print_task_rec_long(rec_ptr);
    }
}




/****************************************************************************/
/****************************************************************************/

/*       Private functions for the task list you declare                    */

/****************************************************************************/
/****************************************************************************/

/*
	This function returns a pointer to the first task in a given list
	that has the given id. If no such task is found, return NULL.

	list_ptr: the list being search

	id: the id we are searching for

	index: used as a return value for the index of the task
*/
task_t* find_id(list_t* list_ptr, int id, int *index)
{
	int i, length;
	length = list_size(list_ptr);
	task_t* task;
	for (i = 0; i < length; i++){
		task = list_access(list_ptr, i);
		if (task->task_id == id){
			*index = i;
			return task;
		}
	}
	return NULL;
}





/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/*       Functions from here to end of file should not be changed           */

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/* If a string ends with an end-of-line \n, remove it. */
void chomp(char *str)
{
    int lastchar = strlen(str) - 1;
    if (lastchar >= 0 && str[lastchar] == '\n') {
        str[lastchar] = '\0';
    }
}

/* Prompts user for task record input.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void fill_task_record(task_t *new)
{
    static int task_id = 0;
    char line[MAXLINE];
    assert(new != NULL);

    new->task_id = task_id;
    task_id++;

    printf("Priority:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->priority);

    //printf("State:");
    //fgets(line, MAXLINE, stdin);
    //sscanf(line, "%u", &new->state);
    new->state = QUEUED;

    printf("Wallclocktime:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%le", &new->wallclocktime);


    printf("Number of Args:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->nargs);

    assert(new->nargs >= 0 && "# args negative\n");
    new->args = (int*) malloc(sizeof(int) * new->nargs);
    int i=0;
    for (i=0; i<new->nargs; i++) {
        printf("Arg %d:", i);
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &new->args[i]);
    }

    printf("\n");
}

/* Gets values (from user) to use as arguments to determine if a task is runnable
 *
 * Input/Output argument is the number of args the user provides
 * Returns pointer to array of args
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
int* getDetermineArgs(int* num_args)
{
    char line[MAXLINE];
    int i;

    printf("Number of Args:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", num_args);

    assert(*num_args >= 0 && "# args negative\n");
    int* args = (int*) malloc(sizeof(int) * (*num_args));
    for (i=0; i<*num_args; i++) {
        printf("Arg %d:", i);
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &args[i]);
    }

    return args;
}

/* Converts state enum value to a printable string
 *
 * Input state enum value
 *
 * DO NOT CHANGE THIS FUNCTION
 */
char* state2str(enum state state)
{
    if (state == QUEUED)
        return "QUEUED";
    else if (state == RUNNING)
        return "RUNNING";
    else if (state == BLOCKED)
        return "BLOCKED";
    else if (state == FINISHED)
        return "FINISHED";
    else
        return "UNKNOWN";
}




/* print the information for a particular task record in short form
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_task_rec_short(task_t *rec)
{
    assert(rec != NULL);
    printf("Task ID: %d priority = %d state = %s\n",
            rec->task_id, rec->priority, state2str(rec->state));
}

/* Long form to print a particular task record
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_task_rec_long(task_t *rec)
{
    assert(rec != NULL);
    int i;

    printf("Task ID: %d\n", rec->task_id);
    printf("     priority = %d\n", rec->priority);
    printf("     state     = %s\n", state2str(rec->state));
    printf("     time = %e\n", rec->wallclocktime);
    printf("     nargs = %d\n", rec->nargs);
    printf("     args = {");
    for (i=0; i<rec->nargs; i++)
        printf(" %d, ", rec->args[i]);
    printf("}\n");
}


/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
