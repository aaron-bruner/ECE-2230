/* atom_support.c         <<-- A template
 * Prof. Calhoun          <<-- many updates required
 * jonccal
 * ECE 2230 Spring 2021
 * MP2
 *
 * Propose: A template for MP2
 *
 * Assumptions:
 *
 * Bugs:
 *
 * You can modify the interface for any of the atom_ functions if you like
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

#include "datatypes.h"
#include "list.h"
#include "atom_support.h"

#define FALSE 0
#define TRUE  1

// private functions for use in atom_support.c only

void fill_atom_record(atom_t *new);          // collect input
void print_atom_rec_short(atom_t *rec);      // print one record
void print_atom_rec_long(atom_t *rec) ;      // print one record
void get_bounding_box(float* minX, float* maxX, float* minY, float* maxY);
int  determine_inside_box(data_t* atom_ptr, float x_min,
                          float x_max, float y_min, float y_max);

/* atom_compare is required by the list ADT for sorted lists.
 *
 * This function returns
 *     1 if rec_a should be closer to the head than rec_b,
 *    -1 if rec_b is to be considered closer to the head, and
 *     0 if the records are equal.
 *
 * For the atom data we want to sort from lowest atom ID up, so
 * closer to the front means a smaller atom ID.
 *
 * The function expects pointers to two record structures, and it is an error
 * if either is NULL
 *
 * THIS FUNCTION SHOULD NOT BE CHANGED
 */
int atom_compare(const atom_t *record_a, const atom_t *record_b)
{
    assert(record_a != NULL && record_b !=NULL);
    if (record_a->atom_id < record_b->atom_id)
        return 1;
    else if (record_a->atom_id > record_b->atom_id)
        return -1;
    else
        return 0;

}

/* Function to pass into List ADT during construction to free the memory
 * associate with a atom record.  This hides the details of how the record
 * is defined from the List ADT.
 */
void atom_rec_cleanup(atom_t *rec)
{
    if (rec != NULL) {
        free(rec);
    }
}

/* This creates a list and it can be either a sorted or unsorted list
 *
 * This function is provided to you as an example of how to use
 * function pointers.
 */
list_t * atom_list_create(void)
{
    return list_construct(atom_compare, atom_rec_cleanup);
}


/* this function frees the memory for either a sorted or unsorted list.
 */
void atom_list_cleanup(list_t * list_ptr)
{
    list_destruct(list_ptr);
}

/* print one of the atom record lists
 *
 * inputs:
 *    list_ptr: a list_t * pointer to either sorted or unsorted list
 *
 *    type_of_list: a charter string that must be either "List" or "Queue"
 *
 * output: prints the list in the format required for grading.
 *         Do not change any of these lines (i.e. printfs && print_atom_rec_short()
 */
void atom_list_print(list_t * list_ptr, char *list_type)
{
    assert(strcmp(list_type, "List")==0 || strcmp(list_type, "Queue")==0);

    int i, num_in_list;

    if (list_ptr != NULL) {
        num_in_list = list_size(list_ptr);
    } else num_in_list = 0;
    atom_t *rec_ptr = NULL;

    if (num_in_list == 0) {
        printf("%s empty\n", list_type);
    } else {
        printf("%s has %d record%s\n", list_type, num_in_list,
               num_in_list==1 ? "." : "s");

        for (i = 0; i < num_in_list; i++) {
            printf("%4d: ", i);
            rec_ptr = list_access(list_ptr, i);
            print_atom_rec_short(rec_ptr);
        }
    }
    rec_ptr=NULL;
}

/* the function takes a pointer to each list and prints the
 * number of items in each list
 */
void atom_list_stats(list_t * sorted, list_t * unsorted)
{
    int num_in_sorted_list = list_size(sorted);
    int num_in_unsorted_list = list_size(unsorted);
    int order_of_list = list_order(sorted);

    printf("Number records: %d, Order: %s\n", num_in_sorted_list,
           order_of_list == 1 ? "Ascending" : "Descending");
    printf("Number records in queue: %d\n", num_in_unsorted_list);
}

/* This function adds an atom to the list.
 *
 * Otherwise, if the list is full the atom is rejected.
 */
void atom_list_add(list_t * list_ptr, int max_list_size)
{
    atom_t *rec_ptr = (atom_t *)malloc(sizeof(atom_t));
    fill_atom_record(rec_ptr);

    int added_return;

    if (list_order(list_ptr) == 0) {
        added_return = 1;
        list_insert(list_ptr, rec_ptr, max_list_size);
    } else {
        list_insert_sorted(list_ptr, rec_ptr);
        added_return = 1;
    }

    //if (list_size(list_ptr) >= max_list_size) added_return = -1;

    /*if ( list_size(list_ptr) < max_list_size) {
        list_insert_sorted(list_ptr, rec_ptr);
        added_return = 1;
    } else {
        added_return = -1;
    } */

    if (added_return == 1) {
        printf("Inserted: %d\n", rec_ptr->atomic_num);
    } else if (added_return == -1) {
        printf("Rejected: %d\n", rec_ptr->atomic_num);
    } else {
        printf("Error with return value! Fix your code.\n");
    }
    rec_ptr = NULL;
}

/* This function prints the first atom with the matching potential energy in
 * the list.
*/
void atom_list_lookup_potential_energy(list_t * list_ptr, float e_pot)
{
    int index = -1;
    atom_t *rec_ptr = NULL;

    for (int i = 0; i < list_size(list_ptr); i++) {
        rec_ptr = list_access(list_ptr, i);
        if (rec_ptr->potential_energy == e_pot){
            index = i;
            break;
        }
    }

    if (rec_ptr == NULL) {
        printf("Did not find: %e\n", e_pot);
    } else {
        printf("Found: %e at index %d\n", e_pot, index);
        print_atom_rec_long(rec_ptr);
    }
    rec_ptr = NULL;
}

/* This function removes the first atom from the list with the matching
 * potential energy
 */
void atom_list_remove_potential_energy(list_t * list_ptr, float e_pot)
{
    int index = -1;
    atom_t *rec_ptr = NULL;

    for (int i = 0; i < list_size(list_ptr); i++) {
        rec_ptr = list_access(list_ptr, i);
        if (rec_ptr->potential_energy == e_pot){
            index = i;
            break;
        }
        else {
            rec_ptr = NULL;
        }
    }

    if (rec_ptr == NULL) {
        printf("Did not remove: %e\n", e_pot);
    } else {
        rec_ptr = list_remove(list_ptr, index);
        printf("Removed: %e\n", e_pot);
        print_atom_rec_long(rec_ptr);
        free(rec_ptr);
    }
    rec_ptr = NULL;
}

/* creates and displays the list of atoms to migrate */
void atom_list_migrate(list_t* list_ptr)
{
    int i, count;
    float minX, maxX, minY, maxY = 0.0;
    list_t* migrate_list = atom_list_create();
    data_t* rec_ptr = NULL;

    get_bounding_box(&minX, &maxX, &minY, &maxY);

    int insideBox = 0;

    for (int index = 0; index < list_size(list_ptr); index++) {
        rec_ptr = list_access(list_ptr, index);
        insideBox = determine_inside_box(rec_ptr, minX, maxX, minY, maxY);
        if (insideBox == 0) {
            if (list_order(list_ptr) == 0) {
                list_insert(migrate_list, rec_ptr, list_size(list_ptr));
            } else {
                list_insert_sorted(migrate_list, rec_ptr);
            }
            list_remove(list_ptr, index);
        }
        insideBox = 0;
        rec_ptr = NULL;
    }

    count = list_size(migrate_list);

    if (count == 0) {
        printf("Did not find atoms to migrate in : %e %e %e %e\n", minX, maxX, minY, maxY);
    } else {
        /* print items in structure */
        printf("Found atoms to migrate:\n");

        for (i=0; i < count; i++) {
            rec_ptr = list_access(migrate_list, i);
            if (rec_ptr != NULL) {
                print_atom_rec_long(rec_ptr);
            }
            else {
                printf("Error in migrate list: NULL value\n");
                break;
            }
        }

    }
    atom_list_cleanup(migrate_list);
    rec_ptr = NULL;
    migrate_list = NULL;


    /*
    int i, count, temp;
    float minX, maxX, minY, maxY = 0.0;
    list_t* migrate_list = (list_t*)malloc(sizeof(list_t));
    data_t* rec_ptr = NULL;

    get_bounding_box(&minX, &maxX, &minY, &maxY);

    for (int j = 0; j < list_size(list_ptr); j++) {
        rec_ptr = list_access(list_ptr, j);

        temp = determine_inside_box(rec_ptr, minX, maxX, minY, maxY);
        if (temp == 0) {
            list_insert_sorted(migrate_list, rec_ptr);
            rec_ptr = list_remove(list_ptr, j);
            free(rec_ptr);
        }
        temp = 0;
    }
    count = list_size(migrate_list);

    if (count == 0) {
        printf("Did not find atoms to migrate in : %e %e %e %e\n", minX, maxX, minY, maxY);
    } else {
        atom_list_print(migrate_list, "List");
        printf("Found atoms to migrate:\n");

        for (i=0; i < count; i++) {
            rec_ptr = list_access(migrate_list, i);
            if (rec_ptr != NULL) {
                print_atom_rec_long(rec_ptr);
            }
            else {
                printf("Error in migrate list: NULL value\n");
                break;
            }
        }
        free(rec_ptr);
    }
    atom_list_cleanup(migrate_list);
    rec_ptr = NULL;
    migrate_list = NULL;*/
}

/* simulates the computation in a single time-step for a MD simulation */
void atom_list_update(list_t* list_ptr, float dt)
{
    data_t *rec_ptr = NULL;

    for (int i = 0; i < list_size(list_ptr); i++) {

        rec_ptr = list_access(list_ptr, i);

        for (int j = 0; j < 2; j++) {
            rec_ptr->force[j] = drand48();//advance forces
            rec_ptr->momenta[j] += dt * rec_ptr->force[j];//advance momenta
            rec_ptr->position[j] += (dt*rec_ptr->momenta[j])/(rec_ptr->mass);//advance position
        }
        rec_ptr->potential_energy = drand48();
    }
}
int determine_inside_box(data_t* rec_ptr, float x_min,
                         float x_max, float y_min, float y_max)
{
    assert(rec_ptr != NULL && "ATOM NULL IN BOX CHECK\n");

    int returnValue = FALSE;

    if (rec_ptr->position[0] >= x_min && rec_ptr->position[0] <= x_max && rec_ptr->position[1] >= y_min &&
        rec_ptr->position[1] <= y_max) {
        returnValue = TRUE;
    }

    return returnValue;
}

/* This function reverses the order of the items in the list */
void atom_list_reverse(list_t * list_ptr)
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

/* This functions adds an atom record to the tail of a list.  The list is
 * unsorted, and it does not have any limits on the maximum size of the list.
 */
void atom_list_add_tail(list_t * list_ptr)
{
    atom_t *rec_ptr = (atom_t *)calloc(1, sizeof(atom_t));
    fill_atom_record(rec_ptr);

    list_insert(list_ptr, rec_ptr, list_size(list_ptr));
    printf("Appended %d onto queue\n", rec_ptr->atomic_num);
}

/* This function removes the atom record at the head of the list.  The
 * list is unsorted, and there is no limit on the maximum size of the list.
 *
 * The atom should be recycled so there are no memory leaks.
 */
void atom_list_remove_head(list_t * list_ptr)
{
    atom_t *atom_ptr = NULL;
    atom_ptr = list_remove(list_ptr, 0);

    if (atom_ptr != NULL)
        printf("Deleted head with atom id and atomic number: %d %d\n",
               atom_ptr->atom_id, atom_ptr->atomic_num);
    else
        printf("Queue empty, did not remove\n");
    atom_rec_cleanup(atom_ptr);
}

/* This function prints the atom at the head of the queue.
 * Print all fields of the atom record.
 */
void atom_list_print_head(list_t * list_ptr)
{
    atom_t *rec_ptr = NULL;
    rec_ptr = list_access(list_ptr, 0);

    if (rec_ptr == NULL) {
        printf("Queue is empty\n");
    } else {
        /* print items in structure */
        printf("Found at head of queue:\n");
        print_atom_rec_long(rec_ptr);
    }
}

/****************************************************************************/
/****************************************************************************/

/*       Private functions for the atom list you declare                    */

/****************************************************************************/
/****************************************************************************/







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

/* Prompts user for atom record input.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void fill_atom_record(atom_t *new)
{
    static int atom_id = 0;
    char line[MAXLINE];
    assert(new != NULL);

    new->atom_id = atom_id;
    atom_id++;

    printf("Atomic Number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->atomic_num);

    printf("Atomic Mass:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", &new->mass);

    //handle multiple dimensions
    memset(new->position, 0, 3*sizeof(float));
    printf("Atom X Position:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", &new->position[0]);
    if (NDIM >= 2) {
        printf("Atom Y Position:");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%e", &new->position[1]);
    }
    if (NDIM >= 3) {
        printf("Atom Z Position:");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%e", &new->position[2]);
    }

    //handle multiple dimensions
    memset(new->momenta, 0, 3*sizeof(float));
    printf("Atom X Momenta:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", &new->momenta[0]);
    if (NDIM >= 2) {
        printf("Atom Y Momenta:");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%e", &new->momenta[1]);
    }
    if (NDIM >= 3) {
        printf("Atom Z Momenta:");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%e", &new->momenta[2]);
    }

    //handle multiple dimensions
    memset(new->force, 0, 3*sizeof(float));
    printf("Atom X Force:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", &new->force[0]);
    if (NDIM >= 2) {
        printf("Atom Y Force:");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%e", &new->force[1]);
    }
    if (NDIM >= 3) {
        printf("Atom Z Force:");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%e", &new->force[2]);
    }


    printf("Potential Energy:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", &new->potential_energy);




    printf("\n");
}


/* Prompts user for x and y values to create a box in 2D space. The lower left
 * corner is defined as (x_min, y_min) and the upper right corner is defined as
 * (x_max, y_max).  The input is not checked for errors but will default to an
 * acceptable value if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void get_bounding_box(float* minX, float* maxX, float* minY, float* maxY)
{

    char line[MAXLINE];

    printf("Min X:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", minX);

    printf("Max X:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", maxX);


    printf("Min Y:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", minY);

    printf("Max Y:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%e", maxY);

}

/* print the information for a particular atom record in short form
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_atom_rec_short(atom_t *rec)
{
    assert(rec != NULL);
    printf("Atom ID: %d position = (%e, %e)\n", rec->atom_id, rec->position[0], rec->position[1]);
}

/* Long form to print a particular atom record
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_atom_rec_long(atom_t *rec)
{
    assert(rec != NULL);
    printf("Atom ID: %d\n", rec->atom_id);
    printf("     atomic # = %d\n", rec->atomic_num);
    printf("     mass     = %e\n", rec->mass);
    printf("     position = (%e, %e)\n", rec->position[0], rec->position[1]);
    printf("     momenta = (%e, %e)\n", rec->momenta[0], rec->momenta[1]);
    printf("     force = (%e, %e)\n", rec->force[0], rec->force[1]);
    printf("     potential energy = %e\n", rec->potential_energy);
    printf("\n");
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
