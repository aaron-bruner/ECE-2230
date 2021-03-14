/* atom_support.h       <<-- A template
 * Prof. Calhoun        <<-- some changes needed
 * jonccal
 * ECE 2230 Spring 2021
 * MP3
 *
 * Propose: A template for MP3
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignment.  
 *
 *              You may change this file if needed for your design.
 *
 * Bugs:
 *
 */

#define MAXLINE 120

/* new functions for MP3 sortings */
void atom_list_add_tail_mp3(list_t *, int atom_id);
void atom_list_sort(list_t **, int sort_type, int sort_order);


/* prototype function definitions */

/* function to compare atom records */
int atom_compare(const atom_t *rec_a, const atom_t *rec_b);

/* function to free memory in an atom record */
void atom_rec_cleanup(atom_t *rec);

/* functions to create and cleanup a atom record list */
list_t * atom_list_create(void);
void atom_list_cleanup(list_t *);

/* Functions to get and print atom information */
void atom_list_print(list_t * list_ptr, char *); /* print list of records */
void atom_list_print_head(list_t *list_ptr);     /* print record at head */
void atom_list_stats(list_t *, list_t *, list_t *);        /* prints size of each list */


/* functions for atom list that has a maximum
 * size for the list */
void atom_list_add(list_t *, int max_list_size);
void atom_list_lookup_potential_energy(list_t * , float potential_energy);
void atom_list_remove_potential_energy(list_t * , float potential_energy);

/* MP1 ops */
void atom_list_migrate(list_t*);
void atom_list_update(list_t*, float dt);

/* New in MP2 */
 void atom_list_reverse(list_t *);

/* functions for unsorted FIFO list that has no limit of the size of the list,
 * inserts at the tail, removes at the head, and does not check for duplicate
 * elements in the list */
void atom_list_add_tail(list_t *);
void atom_list_remove_head(list_t * );

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
