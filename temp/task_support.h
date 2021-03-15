/* task_support.h
 * Casey Hird
 * crhird
 * ECE 2230 Spring 2019
 * MP3
 *
 * Propose: definitions of public functions in task_support.c
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignmnet.  
 *
 *              You may change this file if needed for your design.
 *
 * Bugs: no known bugs.
 *
 */

#define MAXLINE 120

/* new functions for MP3 sortings */
void task_list_add_tail_mp3(list_t *, int priority);
void task_list_sort(list_t **, int sort_type, int sort_order);
/* prototype function definitions */

/* function to compare task records */
int task_compare(const task_t *rec_a, const task_t *rec_b);

/* function to free memory in an task record */
void task_rec_cleanup(task_t *rec);

/* functions to create and cleanup a task record list */
list_t * task_list_create(void);
void task_list_cleanup(list_t *);

/* Functions to get and print task information */
void task_list_print(list_t * list_ptr, char *); /* print list of records */
void task_list_print_head(list_t *list_ptr);     /* print record at head */
void task_list_stats(list_t *, list_t *, list_t *);        /* prints size of each list */


/* functions for task list that has a maximum size for the list */
void task_list_add(list_t *, int max_list_size);
void task_list_lookup_id(list_t * , int id);
void task_list_remove_id(list_t * , int id);

/* MP1 ops */
void task_list_update(list_t *, int id, int state); 
void task_list_schedule(list_t *, int id, int priority);
void task_list_determine(list_t *); 
void task_list_remove_finished(list_t *); 

/* New in MP2 */
void task_list_reverse(list_t *); 



/* functions for unsorted FIFO list that has no limit of the size of the list,
 * inserts at the tail, removes at the head, and does not check for duplicate
 * elements in the list */
void task_list_add_tail(list_t *);
void task_list_remove_head(list_t * );

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
