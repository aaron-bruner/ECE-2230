/* atom_support.c
 * Aaron Bruner
 * ajbrune
 * ECE 2230 Spring 2021
 * MP3
 *
 * Propose: The purpose of atom_support.c is to provide many functions that can be used by our list.c function without
 *          explicitly providing the way in which our list is managed. Instead we want to use a wall of abstraction and
 *          have the user only provide what is necessary to complete the task provided.
 *
 * Assumptions: The assumptions are that the user running this file is aware of how to operate each command/function.
 *              We also must assume that for every function called the provided data is correct.
 *
 * Bugs: None
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

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

/* The function atom_list_add_tail_mp3 is designed to create a new atom using the list_insert() function.
 *
 * Inputs:
 *  list_t * list_ptr: A pointer to the list in which we're going to add our atom to.
 *  int atom_id: The atom id for our newly created atom.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: The atom we created (rec_ptr) will be appended to list_ptr.
*/
void atom_list_add_tail_mp3(list_t *list_ptr, int atom_id)
{
    atom_t *rec_ptr = (atom_t *)calloc(1, sizeof(atom_t)); // Create new atom using calloc()
    rec_ptr->atom_id = atom_id;                            // Assign atom ID to that which was passed in
    list_insert(list_ptr, rec_ptr, list_size(list_ptr));   // Insert using list_insert()
}

/* Function to sort a list using a given method and direction
 *
 * The function atom_list_sort is designed to sort our data and also calculate how long it took for us to sort that
 * data in milliseconds. Then, that data is printed so we can see how efficient our sorting algorithms are.
 *
 * Inputs:
 *  list_t ** list_ptr: A pointer to a pointer of our list.
 *  int sort_type: The sort type that we're intended to use for the sorting algorithm. The options are either:
 *      1. Bubble Sort
 *      2. Insertion Sort
 *      3. Recursive Selection Sort
 *      4. Iterative Selection Sort
 *      5. Merge Sort
 *  int sort_order: The sort order is one of two things, either sorting in ascending order or descending order.
 *      1. Descending Order
 *      2. Ascending Order
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t ** list_ptr: The structure that is being pointed to will be sorted as a result of calling this function.
 */
void atom_list_sort(list_t** list_ptr, int sort_type, int sort_order)
{
    clock_t start, end;
    double elapse_time;  // time in milliseconds
    int initial_size = list_size(*list_ptr);
    start = clock();

    list_sort(list_ptr, sort_type, sort_order);

    end = clock();
    elapse_time =  1000.0 * ((double) (end - start)) / CLOCKS_PER_SEC;
    assert(list_size(*list_ptr) == initial_size);
    printf("Sorting: %d %f %d %d\n", initial_size, elapse_time, sort_type, sort_order);
}

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

/* The function atom_rec_cleanup is designed to free the memory of our passed in rec pointer. It is also designed to
 * mask the details of how the record is defined from the List ADT.
 *
 * Input:
 *  atom_t * rec: A pointer to the atom in which we want to free.
 *
 * Return: N/A
 *
 * Structures Modified: N/A
 */
void atom_rec_cleanup(atom_t *rec)
{
    if (rec != NULL) {
        free(rec);
    }
}

/* The function atom_list_create is designed to create a list which can be either sorted or unsorted.
 *
 * Input: N/A
 *
 * Return: A pointer to our newly created list.
 *
 * Structures Modified: The newly created list that was returned from list_construct.
 */
list_t * atom_list_create(void)
{
    return list_construct(atom_compare, atom_rec_cleanup);
}

/* The function atom_list_cleanup is very simple. It will free the memory of the provided list using the list_destruct
 * function.
 *
 * Input:
 *  list_t * list_ptr: A pointer to the list in which we want to free.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: The structure is going to be free'd.
 */
void atom_list_cleanup(list_t * list_ptr)
{
    list_destruct(list_ptr);
}

/* The function atom_list_print will take the input of a list and a character string of "List" or "Queue."
 *
 * Input:
 *  list_t * list_ptr: A pointer to the list in which we want to print.
 *  char * list_type: A string of either "List" or "Queue."
 *
 * Return: N/A
 *
 * Structures Modified: N/A
 *
 * output: prints the list in the format required for grading.
 *         Do not change any of these lines (i.e. printfs && print_atom_rec_short()
 */
void atom_list_print(list_t * list_ptr, char *list_type)
{
    assert(strcmp(list_type, "List")==0 || strcmp(list_type, "Queue")==0);

    int i, num_in_list = list_size(list_ptr);
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
    rec_ptr = NULL;
}

/* The function atom_list_stats will take a pointer to multiple lists (sorted, unsorted, and mp3) and then print
 * the number of items in each list.
 *
 * Input:
 *  list_t * sorted: A pointer to the sorted list.
 *  list_t * unsorted: A pointer to the unsorted list.
 *  list_t * mp3: A pointer to our new MP3 list.
 *
 * Return: N/A
 *
 * Structures Modified: N/A
 */
void atom_list_stats(list_t * sorted, list_t * unsorted, list_t* mp3)
{

    int num_in_sorted_list = list_size(sorted);
    int num_in_mp3_sorted_list = list_size(mp3);
    int num_in_unsorted_list = list_size(unsorted);
    int order_of_list = list_order(sorted);
    char *sort_order_mp3;

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

/* The function atom_list_add will add the atom that is passed in depending on the sort order of the list. Assuming the
 * list is full then an atom will not be added to the list.
 *
 * Input:
 *  list_t *list_ptr: This is a pointer to the header block that the atom should be added to.
 *  int max_list_size: An integer of how large the list size is.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: The list list_ptr will have a new atom added based on what the user inputs to rec_ptr.
 */
void atom_list_add(list_t * list_ptr, int max_list_size)
{
    atom_t *rec_ptr = (atom_t *)malloc(sizeof(atom_t));
    fill_atom_record(rec_ptr);

    int added_return = -2;

    if (list_size(list_ptr) >= max_list_size) {
        added_return = -1; // Failure
    } else {
        if (list_order(list_ptr) == 0) {
            added_return = 1; // Success
            list_insert(list_ptr, rec_ptr, max_list_size);
        }
        else {
            list_insert_sorted(list_ptr, rec_ptr);
            added_return = 1; // Success
        }
    }

    if (added_return == 1) {
        printf("Inserted: %d\n", rec_ptr->atomic_num); // Successful Insertion
    } else if (added_return == -1) {
        printf("Rejected: %d\n", rec_ptr->atomic_num); // Failed Insertion
    } else {
        printf("Error with return value! Fix your code.\n");
    }

    rec_ptr = NULL;
}

/* The function atom_list_lookup_potential_energy will check the list that is provided and determine if an atom in
 * the list has the provided potential energy. If it is found, we will print out that atom using print_atom_rec_long.
 *
 * Input:
 *  list_t *list_ptr: The pointer to our list in which we want to search.
 *  float e_pot: The potential energy value we're looking for.
 *
 * Return: N/A
 *
 * Structures Modified: N/A
 */
void atom_list_lookup_potential_energy(list_t * list_ptr, float e_pot)
{
    int index = -1;
    atom_t *rec_ptr = NULL;

    for (int i = 0; i < list_size(list_ptr); i++) { // Check from index 0 to list_size - 1
        rec_ptr = list_access(list_ptr, i);
        if (rec_ptr->potential_energy == e_pot){ // If the potential_energy for the atom matches e_pot then we break
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
}

/* The function atom_list_remove_potential_energy will search over the provided list and compare every atom's
 * potential energy to the provided float value e_pot. If the potential energy of the atom matches e_pot then
 * it is removed from the list.
 *
 * Input:
 *  list_t * list_ptr: A pointer to the list in which we want to search and compare e_pot to.
 *  float e_pot: A float value in which we want to compare each atom's potential energy to.
 *
 * Structures Modified:
 *  list_t * list_ptr: There is a chance that an atom can be removed from this list.
 */
void atom_list_remove_potential_energy(list_t * list_ptr, float e_pot)
{
    int index = -1;
    atom_t *rec_ptr = NULL;

    for (int i = 0; i < list_size(list_ptr); i++) { // Find the index for our e_pot value
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
        rec_ptr = list_remove(list_ptr, index); // Remove index value where e_pot shows up
        printf("Removed: %e\n", e_pot);
        print_atom_rec_long(rec_ptr);
        free(rec_ptr);
    }
    rec_ptr = NULL;
}

/* The function atom_list_migrate looks at the list provided and checks if atoms are either within or outside
 * of the gathered coordinates. If the atom is within the specified coordinates then it remains inside the list.
 * If the atom is outside of the coordinates then it is added to a new list and removed from the original list.
 *
 * Input:
 *  list_t * list_ptr: Pointer to header block which contains our 'original' list.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: Atoms could potentially be removed from the list.
 *  list_t * migrate_list: List is initialized and atoms could be added to the list.
 */
void atom_list_migrate(list_t * list_ptr)
{
    int i, count;
    float minX, maxX, minY, maxY = 0.0;
    list_t* migrate_list = atom_list_create(); // Allocate memory for our new migrate list
    data_t* rec_ptr = NULL;

    get_bounding_box(&minX, &maxX, &minY, &maxY);

    int insideBox = 0; // This variable is used as a flag to check if determine_inside_box is true or false

    for (int index = 0; index < list_size(list_ptr); index++) { // This code was copied from MP1 and modified to work
        rec_ptr = list_access(list_ptr, index);                 // with our new list ADT
        insideBox = determine_inside_box(rec_ptr, minX, maxX, minY, maxY);
        if (insideBox == 0) {
            if (list_order(list_ptr) == 0) {
                list_insert(migrate_list, rec_ptr, list_size(list_ptr));
            } else {
                list_insert_sorted(migrate_list, rec_ptr);
            }
            rec_ptr = list_remove(list_ptr, index);
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
}

/* The function atom_list_update will simulate multiple computations. Specifically, one for our
 * force, momenta and position. In MP1 we had three separate functions to complete this, here
 * we can complete it in one function call.
 *
 * Input:
 *  list_t * list_ptr: A pointer to the list in which we want to apply our calculations to.
 *  float dt: A float value provided by the user in which we will use to apply to our calculations.
 *
 * Output: N/A
 *
 * Structures Modified:
 *  list_ptr: We will be modifying the force, momenta and position of both 0 and 1 for the array. The
 *            formulas used were provided in MP1 and have been reused in MP2 and now MP3.
 */
void atom_list_update(list_t* list_ptr, float dt)
{
    data_t *rec_ptr = NULL;

    for (int i = 0; i < list_size(list_ptr); i++) { // Code copied from MP1 and modified to work with our new list ADT

        rec_ptr = list_access(list_ptr, i); // Iterating through our list from the head to the tail

        for (int j = 0; j < 2; j++) {
            rec_ptr->force[j] = drand48();//advance forces
            rec_ptr->momenta[j] += dt * rec_ptr->force[j];//advance momenta
            rec_ptr->position[j] += (dt*rec_ptr->momenta[j])/(rec_ptr->mass);//advance position
        }
        rec_ptr->potential_energy = drand48();
    }
}

/* The function determine_inside_box determines if the provided rec_ptr
 * is within the boundaries provided to the function.
 *
 * Input:
 *  data_t * rec_ptr: A pointer to an atom element
 *  x_min: A float value for comparing the x minimum
 *  x_max: A float value for comparing the x maximum
 *  y_min: A float value for comparing the y minimum
 *  y_max: A float value for comparing the y maximum
 *
 * Output:
 *  An integer value of 1 or 0 (TRUE or FALSE) which will determine if the provided atom is
 *  within the provided boundaries.
 *
 *  Structures Modified: N/A
 */
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

/* The function atom_list_reverse is designed to take in an input list and reverse the elements of the list.
 *
 * Input:
 *  list_t * list_ptr: A pointer to our list in which we want to reverse.
 *
 * Output: N/A
 *
 * Structures Modified:
 *  list_ptr: We are reversing all of the atoms in the list.
 */
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

/* The function atom_list_add_tail is designed to remove the tail of our list. In other words
 * we are removing the last atom from the list. The list is unsorted, and it does not have any limits
 * on the maximum size of the list.
 *
 * Input:
 *  list_t * list_ptr: A pointer to the head of the list in which we want to remove the tail from.
 *
 * Output: N/A
 *
 * Structures Modified:
 *  list_ptr: Adding rec_ptr to the tail of list_ptr. The information for rec_ptr is gathered from the user.
 */
void atom_list_add_tail(list_t * list_ptr)
{
    atom_t *rec_ptr = (atom_t *) malloc(sizeof(atom_t)); // Allocate memory for node
    fill_atom_record(rec_ptr); // Collect input

    list_insert(list_ptr, rec_ptr, list_size(list_ptr)); // Insert into non-sorted list

    printf("Appended %d onto queue\n", rec_ptr->atomic_num);
}

/* The function atom_list_remove_head is designed to remove the atom record at the head of the list.
 * The list is unsorted, and there is no limit on the maximum size of the list.
 *
 * Input:
 *  list_t * list_ptr:
 *      A pointer to the head of the list of which we want to remove the head atom.
 *
 * Output: N/A
 *
 * Structures Modified:
 *  list_ptr: We are deleting the head node of the list.
 */
void atom_list_remove_head(list_t * list_ptr)
{
    atom_t *atom_ptr = list_remove(list_ptr, 0); // Remove the head node

    if (atom_ptr != NULL) // Ensure the atom was indeed removed
        printf("Deleted head with atom id and atomic number: %d %d\n",
                atom_ptr->atom_id, atom_ptr->atomic_num);
    else
        printf("Queue empty, did not remove\n");
    atom_rec_cleanup(atom_ptr); // Cleanup memory
}

/* The function atom_list_print_head prints the atom at the head of the queue. Then, it will
 * print all fields of the atom record using print_atom_rec_long.
 *
 * Input:
 *  list_t * list_ptr: A pointer to the head of our list of which we want to print
 *
 *  Return: N/A
 *
 *  Structures Modified: N/A
 */
void atom_list_print_head(list_t * list_ptr)
{
    atom_t *rec_ptr = list_access(list_ptr, 0); // Copy the head pointer (index 0)

    if (rec_ptr == NULL) {
        printf("Queue is empty\n");
    } else {
        printf("Found at head of queue:\n");
        print_atom_rec_long(rec_ptr); // Print items in structure
    }
}

/****************************************************************************/
/****************************************************************************/

/*       Private functions for the atom list you declare                    */

/****************************************************************************/
/****************************************************************************/



/****************************************************************************/
/****************************************************************************/

/*       Functions from here to end of file should not be changed           */

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