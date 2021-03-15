/* list.c
 * Aaron Bruner
 * ajbrune
 * ECE 2230 Spring 2021
 * MP3
 *
 * Propose: The purpose of list.c is to continue our developing idea of a two way linked list. We're now wanting
 *          to evaluate different sorting methods of data in different sizes. We will be comparing 5 different sorting
 *          algorithms; bubble sort, insertion sort, recursive selection sort, iterative selection sort and merge sort.
 *
 * Assumptions: We must assume that the user operating the code understands how to properly run the program. Also
 *              we must assume that the user only provides valid input which will not break the lab3.c file.
 *
 * Bugs: None
 *
 * The interface definition for the two-way linked list ADT is based on one
 * defined by OPNET, Inc. http://www.opnet.com/
 */

#include <stdlib.h>
#include <assert.h>

#include "datatypes.h"   /* defines data_t */
#include "list.h"        /* defines public functions for list ADT */

/* definitions for private constants used in list.c only */

#define LIST_SORTED_ASCENDING   -29634
#define LIST_SORTED_DESCENDING  -29631
#define LIST_UNSORTED           -29630
//int VALIDATE_STATUS = 1;

/* prototypes for private functions used in list.c only */
void list_debug_validate(list_t *L);
void list_bubble_sort(list_t** L, int sort_order);
void list_insertion_sort(list_t** L, int sort_order);
void list_recursive_selection_sort(list_t** L, int sort_order);
void list_selection_sort(list_t** L, int sort_order);
void list_merge_sort(list_t** L, int sort_order);

/* sorting helper functions) */
void swap(list_node_t *xp, list_node_t *yp);
void SelectionSort(list_t** L, list_node_t* m, list_node_t* n, int sort_order);
list_node_t* FindMax(list_t* L, list_node_t* m, list_node_t* n);
list_node_t* FindMin(list_t* L, list_node_t* m, list_node_t* n);
void cut(list_t **L, list_t** left, list_t** right);
void combine(list_t **L, list_t **left, list_t **right, int sort_order);

/* The function list_sort sorts a list used a specific sorting algorithm and direction.
 *
 * Inputs:
 *  list_t ** list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *                      obtained from list_construct.
 *  int sort_type: sorting algorithm used during sorting:
 *                  1) Bubble Sort
 *                  2)Insertion Sort
 *                  3) Recursive Selection Sort
 *                  4) Iterative Selection Sort
 *                  5) Merge Sort
 *  int sort_order:
 *                  1) Items are to be sorted in descending order
 *                  2) Items are to be sorted in ascending order
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t ** list_ptr: The list that is being pointed to will be sorted based on the sort_type and order
 */
void list_sort(list_t **list_ptr, int sort_type, int sort_order)
{
    if (list_ptr == NULL || *list_ptr == NULL) exit(5); // Corner Cases: Bad list passed in

    if ((*list_ptr)->current_list_size > 1) { // Only sort if there are more than 1 element
        switch (sort_type) {
            case 1:
                list_bubble_sort(list_ptr, sort_order);                 // Bubble Sort
                break;
            case 2:
                list_insertion_sort(list_ptr, sort_order);              // Insertion Sort
                break;
            case 3:
                list_recursive_selection_sort(list_ptr, sort_order);    // Recursive Selection Sort
                break;
            case 4:
                list_selection_sort(list_ptr, sort_order);              // Iterative Selection Sort
                break;
            case 5:
                list_merge_sort(list_ptr, sort_order);                  // Merge Sort
                break;
            default:
                assert("Invalid sort_type...");                         // Invalid Input
        }
    }

    /* Sorting order 1 is descending while 2 is ascending */
    sort_order == 1 ? ((*list_ptr)->list_sorted_state = LIST_SORTED_DESCENDING) :
                        ((*list_ptr)->list_sorted_state = LIST_SORTED_ASCENDING);
    list_debug_validate(*list_ptr);
}

/* The function list_bubble_sort is sorting the list via the insertion sort algorithm.
 *
 * Inputs:
 *  list_t ** L: Pointer to list-of-interest.
 *  int sort_order:
 *          1) Sort list in descending order
 *          2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t ** L: The pointer to the list is being sorted.
 *
 * The algorithm being used here is based off of this page: https://www.geeksforgeeks.org/bubble-sort/
 */
void list_bubble_sort(list_t** L, int sort_order)
{
    if ((*L)->current_list_size <= 1) assert("List is not of the correct size"); // Corner Case: List of size 1 or less

    int comp_logic, n = (*L)->current_list_size;

    // Depending on the sort order we want to ensure our comp_proc function
    // is either comparing to 1 if they're in ascending order or -1 if they're in descending order
    sort_order == 1 ? (comp_logic = 1) : (comp_logic = -1);

    list_node_t *rover = (*L)->head;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if ((*L)->comp_proc(rover->data_ptr,rover->next->data_ptr) == comp_logic) swap(rover,rover->next);
            rover = rover->next;
        }
        rover = (*L)->head; // Return back to start for next pass
    }
}

/* The function swap will take two nodes and swap their data.
 *
 * Input:
 *  list_node_t *xp: Pointer to a node
 *  list_node_t *yp: Pointer to a node
 *
 * Return: N/A
 *
 * Structures Modified: The two pointers will have their data interchanged.
 */
void swap(list_node_t *xp, list_node_t *yp)
{
    data_t *temp = xp->data_ptr;
    xp->data_ptr = yp->data_ptr;
    yp->data_ptr = temp;

    temp = NULL;
}

/* The function list_insertion_sort is sorting the list via the insertion sort algorithm.
 *
 * Inputs:
 *  list_t ** L: Pointer to list-of-interest.
 *  int sort_order:
 *      1) Sort list in descending order
 *      2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be sorted in either ascending or descending order.
 */
void list_insertion_sort(list_t** L, int sort_order)
{
    if ((*L)->current_list_size <= 1) assert("List is not of the correct size"); // Corner Case: List of size 1 or less

    list_t *new_list = list_construct((*L)->comp_proc, (*L)->data_clean);

    /* Sorting order 1 is descending while 2 is ascending */
    sort_order == 1 ? (new_list->list_sorted_state = LIST_SORTED_DESCENDING) :
                        (new_list->list_sorted_state = LIST_SORTED_ASCENDING);

    int n = (*L)->current_list_size;
    for (int i = 0; i < n; i++) {
        list_insert_sorted(new_list, list_remove(*L, 0)); // Remove head, insert to second list
    }

    list_destruct(*L); // Clean up old list
    *L = new_list;
}

/* The function list_recursive_selection_sort is sorting the list via the recursive selection sort algorithm.
 *
 * Inputs:
 *  list_t ** L: pointer to list-of-interest.
 *  int sort_order:
 *      1) Sort list in descending order
 *      2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be sorted in either ascending or descending order.
 */
void list_recursive_selection_sort(list_t** L, int sort_order)
{
    if ((*L)->current_list_size <= 1) assert("List is not of the correct size"); // Corner Case: List of size 1 or less
    if ((*L) == NULL || L == NULL) assert("List is not valid..."); // Corner Case: List passed in is invalid.

    SelectionSort(L, (*L)->head, (*L)->tail, sort_order);
}

/* The function SelectionSort is sourced from 5.19 on page 152 by Standish. We have modified it so
 * that it can handle our two way linked list.
 *
 * Inputs:
 *  list_t ** L: Pointer to the list-of-interest
 *  list_node_t * m: Pointer to the first node to be sorted
 *  list_node_t * n: Pointer to the last node to be sorted
 *  int sort_order:
 *      1) Sort list in descending order
 *      2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be sorted in either ascending or descending order.
 */
void SelectionSort(list_t** L, list_node_t* m, list_node_t* n, int sort_order)
{
    list_node_t* temp;

    if (m != n) { // As long as we're not looking for the same thing attempt to sort.
        sort_order == 1 ? (temp = FindMax(*L, m, n)) : (temp = FindMin(*L, m, n));
        swap(m, temp); // Swap the first node with the one we found
        SelectionSort(L, m->next, n, sort_order); // Reduce domain and enter recursion
    }
    temp = NULL;
}

/* The function FindMax is sourced from 5.19 on page 152 by Standish. We have modified it so
 * that it can handle our two way linked list.
 *
 * Inputs:
 *  list_t * L: Pointer to list-of-interest.
 *  list_node_t * m: Pointer to the first node to be sorted
 *  list_node_t * n: Pointer to the last node to be sorted
 *
 * Return:
 *  list_node_t * max: Pointer to the maximum node
 *
 *  Structures Modified: N/A
 */
list_node_t* FindMax(list_t* L, list_node_t* m, list_node_t* n)
{
    list_node_t* max = m;
    list_node_t* rover = max->next;

    do {
        if (L->comp_proc(max->data_ptr, rover->data_ptr) == 1) max = rover;
        rover = rover->next; // Keep moving down the list
    } while (rover != NULL);

    return max;
}

/* The function FindMin is a variation of FindMax, sourced from 5.19 on page 152
 * by Standish. We have modified it so that it can handle our two way linked list.
 *
 * Inputs:
 *  list_t * L: Pointer to list-of-interest.
 *  list_node_t * m: Pointer to the first node to be sorted
 *  list_node_t * n: Pointer to the last node to be sorted
 *
 * Return:
 *  list_node_t * min: Pointer to the minimum node
 *
 *  Structures Modified: N/A
 */
list_node_t* FindMin(list_t* L, list_node_t* m, list_node_t* n)
{
    list_node_t* min = m;
    list_node_t* rover = min->next;

    do {
        if (L->comp_proc(min->data_ptr, rover->data_ptr) == -1) min = rover;
        rover = rover->next; // Keep moving down the list
    } while (rover != NULL);

    return min;
}

/* The function list_selection_sort is sorting the list via the selection sort algorithm. The code here is a
 * modified version of 5.35 on page 171 by Standish so that it works with our two way linked list.
 *
 * Inputs:
 *  list_t ** L: pointer to list-of-interest.
 *  int sort_order:
 *      1) Sort list in descending order
 *      2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be sorted in either ascending or descending order.
 */
void list_selection_sort(list_t** L, int sort_order)
{
    if ((*L)->current_list_size <= 1) assert("List is not of the correct size"); // Corner Case: List of size 1 or less
    if ((*L) == NULL || L == NULL) assert("List is not valid..."); // Corner Case: List passed in is invalid.

    int comp_logic;
    // Depending on the sort order we want to ensure our comp_proc function
    // is either comparing to 1 if they're in ascending order or -1 if they're in descending order
    sort_order == 1 ? (comp_logic = 1) : (comp_logic = -1);

    list_node_t *start, *temp, *rover;
    int i, m = 0, n = (*L)->current_list_size - 1;

    while (m < n) {
        start = (*L)->head;
        for (i = 0; i < m; i++) start = start->next;

        i++;
        temp = start;
        rover = temp->next;
        while (rover) { // Look for max/min of list
            if ((*L)->comp_proc(temp->data_ptr, rover->data_ptr) == comp_logic)
                temp = rover;
            rover = rover->next; // Move the rover to the next value
        }
        swap(start, temp);
        m++;
    }
}

/* The function list_merge_sort is sorting the list via the merge sort algorithm. The code here is a
 * modified version of 6.19 on page 237 by Standish so that it works with our two way linked list.
 *
 * Inputs:
 *  list_t ** L: pointer to list-of-interest.
 *  int sort_order:
 *      1) Sort list in descending order
 *      2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be sorted in either ascending or descending order.
 */
void list_merge_sort(list_t** L, int sort_order)
{
    int length = (*L)->current_list_size;
    if (length > 1) {

        list_t *left = list_construct((*L)->comp_proc, (*L)->data_clean);
        list_t *right = list_construct((*L)->comp_proc, (*L)->data_clean);

        cut(L, &left, &right); // Split into two half-lists

        list_merge_sort(&left, sort_order); // Sort
        list_merge_sort(&right, sort_order);

        combine(L, &left, &right, sort_order); // Combine
    }
}

/* The function cut is splitting a list into two separate pieces.
 *
 * Inputs:
 *  list_t ** L: Pointer to list-of-interest.
 *  list_t ** left: Pointer to left side list
 *  list_t ** right: Pointer to right side list
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be split into a left side and a right side. The memory will be freed.
 */
void cut(list_t **L, list_t** left, list_t** right)
{

    int half_length = (*L)->current_list_size / 2; // Length of half of the list
    list_node_t *middle = (*L)->head;

    for (int i = 1; i < half_length; i++) middle = middle->next;

    (*left)->head = (*L)->head; // Create left list
    (*left)->tail = middle;
    (*left)->current_list_size = half_length;

    (*right)->head = middle->next; // Create right list
    (*right)->tail = (*L)->tail;

    (*left)->tail->next = NULL; // Split each list by setting ends to NULL
    (*right)->head->prev = NULL;

    /* We need to handle the case where there is an odd number of elements in the list. If there are an
    * odd number of atoms then we will add one more to the right side of our list. Otherwise, it'll be
    *set to the half_length. */
    (*L)->current_list_size % 2 == 0 ? ((*right)->current_list_size = half_length) :
                                            ((*right)->current_list_size = half_length+1);

    free(*L); // Since we've removed the elements from L we can free it
    *L = NULL;
}

/* The function combine is designed to bring two lists together and make them one.
 *
 * Inputs:
 *  list_t ** L: Pointer to list where we'll combine the left and right into.
 *  list_t ** left: Left side.
 *  list_t ** right: Right side.
 *  int sort_order:
 *      1) Sort list in descending order
 *      2) Sort in ascending order
 *
 * Return: N/A
 *
 * Structures Modified: The data in list L will be filled with left and right.
 */
void combine(list_t **L, list_t **left, list_t **right, int sort_order)
{
    int comp_logic;
    // Depending on the sort order we want to ensure our comp_proc function
    // is either comparing to 1 if they're in ascending order or -1 if they're in descending order
    sort_order == 1 ? (comp_logic = 1) : (comp_logic = -1);

    *L = list_construct((*left)->comp_proc, (*left)->data_clean);
    int n = (*left)->current_list_size + (*right)->current_list_size; // # of Left atoms + # of Right atoms
    data_t* data;

    for (int i = 0; i < n; i++) {
        if ((*left)->current_list_size == 0) {              // Left is empty, go to Right
            data = list_remove(*right, 0);
            list_insert(*L, data, LISTPOS_TAIL);
        }
        else if ((*right)->current_list_size == 0) {        // Right is empty, go to Left
            data = list_remove(*left, 0);
            list_insert(*L, data, LISTPOS_TAIL);
        }
        else {                                              // Neither list is empty
            (*L)->comp_proc((*left)->head->data_ptr, (*right)->head->data_ptr) == comp_logic ?
                (data = list_remove(*left, 0), list_insert(*L, data, LISTPOS_TAIL)) :
                (data = list_remove(*right, 0), list_insert(*L, data, LISTPOS_TAIL));
        }
    }
    list_destruct(*left); // Now that the left and right have been added back to L we can free them.
    list_destruct(*right);
    *left = NULL;
    *right = NULL;
}

/* ----- below are the functions  ----- */

/* The function list_access obtains a pointer to an element
 * stored in the specified list, at the specified list position.
 *
 * Inputs:
 *  list_t * list_ptr: pointer to list-of-interest. A pointer to an empty list is
 *                     obtained from list_construct.
 *  int pos_index: position of the element to be accessed.  Index starts at 0 at
 *                 head of the list, and incremented by one until the tail is
 *                 reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * Return:
 *  data_t * L->data_ptr: A pointer to element accessed within the specified list.  A
 *                        value NULL is returned if the pos_index does not correspond
 *                        to an element in the list.  For example, if the list is
 *                        empty, NULL is returned.
 *
 *
 * Structures Modified: N/A
 *
 * Code copied from MP2.
 */
data_t * list_access(list_t *list_ptr, int pos_index)
{
    list_node_t *L;

    assert(list_ptr != NULL);

    /* debugging function to verify that the structure of the list is valid */
    list_debug_validate(list_ptr);

    if ( list_ptr->current_list_size == 0 || list_ptr->head == NULL || list_ptr == NULL) { // List is empty
        return NULL;
    }
    else if ( pos_index == 0 || pos_index == LISTPOS_HEAD) { // Head
        L = list_ptr->head;
    }
    else if ( pos_index == (list_ptr->current_list_size)-1 || pos_index == LISTPOS_TAIL) { // Tail
        L = list_ptr->tail;
    }
    else if ( pos_index >= list_ptr->current_list_size || (pos_index < 0 && pos_index != LISTPOS_HEAD
                                                           && pos_index != LISTPOS_TAIL) ) {
        return NULL;
    }
    else { // we need to find the index value
        L = list_ptr->head;
        for (int i = 0; i < pos_index; i++) {
            L = L->next;
        }
    }

    assert(L != NULL);
    return L->data_ptr;
}

/* The function list_construct allocates a new, empty list.
 *
 * Inputs:
 *  int (*fcomp)(const data_t *, const data_t *): Pointer to the comparison function
 *  void (*dataclean)(data_t *)): Pointer to the data clean function
 *
 * Return:
 *  list_t * L: A pointer to the newly created, empty list.
 *
 * Structures Modified: L is created and all values are set to 0, NULL, pointers to functions, or ascending.
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values LIST_SORTED_ASCENDING LIST_SORTED_DESCENDING or
 * LIST_UNSORTED
 *
 * Use list_free to remove and deallocate all elements on a list (retaining the
 * list itself).
 *
 * Use the standard function free() to deallocate a list which is no longer
 * useful (after freeing its elements).
 */
list_t * list_construct(int (*fcomp)(const data_t *, const data_t *),
        void (*dataclean)(data_t *))
{
    list_t *L;
    L = (list_t *) malloc(sizeof(list_t));
    L->head = NULL;
    L->tail = NULL;
    L->current_list_size = 0;
    L->list_sorted_state = LIST_SORTED_ASCENDING;
    L->comp_proc = fcomp;
    L->data_clean = dataclean;

    /* the last line of this function must call validate */
    list_debug_validate(L);
    return L;
}

/* The function list_elem_find finds an element in a list and returns a pointer to it.
 *
 * Inputs:
 *  list_t * list_ptr: pointer to list-of-interest.
 *  data_t * elem_ptr: element against which other elements in the list are compared.
 *                     Note: use the comp_proc function pointer found in the list_t
 *                     header block.
 *  int * pos_index: used as a return value for the position index of matching element
 *
 * Return:
 *  data_t * returnValue: A pointer to the matching element with lowest index if
 *                        a match if found. If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 * lowest index.  If a matching element is not found, the position index that
 * is returned should be -1.
 *
 * Code copied from MP2.
 */
data_t * list_elem_find(list_t *list_ptr, data_t *elem_ptr, int *pos_index)
{
    list_debug_validate(list_ptr);

    *pos_index = -1;
    list_node_t *list = list_ptr->head; // initialize our list to the start of the data
    data_t *returnValue = NULL;

    if (list_access(list_ptr, *pos_index) == NULL) return NULL; // check for invalid index

    for (int i = 0; i < list_ptr->current_list_size; i++) {
        if ( list_ptr->comp_proc(list->data_ptr, elem_ptr) == 0) { // if the i value we're at matches elem_ptr then
            *pos_index = i;                                        // pos_index = i and break out of the loop
            break;
        } else {
            list = list->next; // If it isn't a match then go to the next one
        }
    }

    returnValue = list->data_ptr; // Just setting the return value to the data pointer

    return returnValue;
}

/* The function list_destruct deallocates the contents of
 * the specified list, releasing associated memory resources for other purposes.
 *
 * Inputs:
 *  list_t * list_ptr: A pointer to the list that needs to be deallocated.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: The entire list will be removed.
 *
 * Code copied from MP2.
 */
void list_destruct(list_t *list_ptr)
{
    /* the first line must validate the list */
    list_debug_validate(list_ptr);

    assert(list_ptr != NULL);

    if ( list_ptr->current_list_size == 0 ) {
        free(list_ptr); // List is empty
    }

    else { // If the list is not empty then free everything from the last element back to the first
        for (int i = (list_ptr->current_list_size)-1; i >= 0; i--) {
            list_node_t *list = list_ptr->tail;
            free(list_ptr->tail->data_ptr);         // Free the data at the tail pointer
            list_ptr->tail = list_ptr->tail->prev;  // Shift the pointer to the previous element
            free(list);
        }
        free(list_ptr);
    }
}

/* The function list_insert inserts the specified data
 * element into the specified list at the specified position.
 *
 * Inputs:
 *  list_t * list_ptr: pointer to list-of-interest.
 *  data_t * elem_ptr: pointer to the element to be inserted into list.
 *  int pos_index: numeric position index of the element to be inserted into the
 *                 list.  Index starts at 0 at head of the list, and incremented by
 *                 one until the tail is reached.  The index can also be equal
 *                 to LISTPOS_HEAD or LISTPOS_TAIL (these are special negative
 *                 values use to provide a short cut for adding to the head
 *                 or tail of the list).
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: elem_ptr will be added to the list_ptr at pos_index, assuming it exists.
 *
 * If pos_index is greater than the number of elements currently in the list,
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->list_sorted_state must be equal
 * to LIST_UNSORTED.
 *
 * Code copied from MP2.
 */
void list_insert(list_t *list_ptr, data_t *elem_ptr, int pos_index)
{
    assert(list_ptr != NULL);

    list_node_t *list = (list_node_t *)malloc(sizeof(list_node_t)); if (list == NULL) exit(1);
    list->data_ptr = elem_ptr;

    if (list_ptr->current_list_size == 0 /*|| (list_ptr->head == NULL && list_ptr->tail == NULL)*/) { // Empty List
        list->prev = NULL;
        list->next = NULL;
        list_ptr->head = list;
        list_ptr->tail = list;
    }
    else if (pos_index == 0 || pos_index == LISTPOS_HEAD) { // HEAD
        list->prev = NULL;
        list_ptr->head->prev = list;
        list->next = list_ptr->head;
        list_ptr->head = list;
    }
    else if (pos_index >= list_ptr->current_list_size || pos_index == LISTPOS_TAIL) { // TAIL
        list_ptr->tail->next = list;
        list->prev = list_ptr->tail;
        list->next = NULL;
        list_ptr->tail = list;
    }
    else {
        list_node_t *start = list_ptr->head;
        list_node_t *end;
        for (int i = 0; i < pos_index; i++) start = start->next; // Get to the pos_index position

        end = start->prev;
        list->prev = end;
        end->next = list;
        list->next = start;
        start->prev = list;
    }

    list_ptr->current_list_size += 1;

    /* the last two lines of this function must be the following */
    if (list_ptr->list_sorted_state != LIST_UNSORTED)
        list_ptr->list_sorted_state = LIST_UNSORTED;
    list_debug_validate(list_ptr);
}

/* The function list_insert_sorted inserts the element into the specified
 * sorted list at the proper position, as defined by the comp_proc function
 * pointer found in the header block.
 *
 * Inputs:
 *  list_t * list_ptr: Pointer to list-of-interest.
 *  data_t * elem_ptr: Pointer to the element to be inserted into list.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: The list provided will have the element elem_ptr added to it's list in either
 *  ascending order or descending order.
 *
 * If you use list_insert_sorted, the list preserves its sorted nature.
 *
 * If you use list_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call list_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 * This definition results in the list being in ascending order.  To insert
 * in descending order, change the sign of the value that is returned.
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 *
 * Code copied from MP2.
 */
void list_insert_sorted(list_t *list_ptr, data_t *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->list_sorted_state != LIST_UNSORTED);

    int position = 0, current_state = list_ptr->list_sorted_state;
    list_node_t *list = list_ptr->head;

    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING) { // Ascending
        while (list != NULL && list_ptr->comp_proc(list->data_ptr, elem_ptr) != -1){
            position += 1;
            list = list->next;
        }
    }
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING) { // Descending
        while (list != NULL && list_ptr->comp_proc(list->data_ptr, elem_ptr) != 1){
            position += 1;
            list = list->next;
        }
    }

    list_insert(list_ptr, elem_ptr, position);

    list_ptr->list_sorted_state = current_state;

    /* the last line of this function must be the following */
    list_debug_validate(list_ptr);
}

/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * Input:
 *  list_t * list_ptr: Pointer to list-of-interest.
 *  int pos_index: position of the element to be removed.  Index starts at 0 at
 *                 head of the list, and incremented by one until the tail is
 *                 reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * Return:
 *  data_t * temp_data: A pointer to the data that was removed. If it was not removed then
 *  a NULL pointer will be returned.
 *
 * Structures Modified:
 *  list_t * list_ptr: The list provided will have the pos_index value removed from the list, assuming it exists.
 *
 *  Code copied from MP2.
 */
data_t * list_remove(list_t *list_ptr, int pos_index)
{
    if (list_access(list_ptr, pos_index) == NULL) return NULL; // List doesn't exist

    // We're going to use temp and temp_data as a copy of our removed index value
    list_node_t *temp;
    data_t *temp_data;

    list_node_t *next, *prev;

    if ((pos_index == 0 || pos_index == LISTPOS_HEAD || pos_index == LISTPOS_TAIL) // list of size 1
        && list_ptr->current_list_size == 1) {
        temp = list_ptr->head; // Copy the head into temp & data into temp_data
        temp_data = temp->data_ptr;

        list_ptr->head = NULL; // Remove the data at head and tail
        list_ptr->tail = NULL;
    }
    else if (pos_index == 0 || pos_index == LISTPOS_HEAD) { // Remove Head

        temp = list_ptr->head;
        temp_data = temp->data_ptr;

        list_ptr->head = list_ptr->head->next;
        list_ptr->head->prev = NULL;
    }
    else if (pos_index == (list_ptr->current_list_size)-1 || pos_index == LISTPOS_TAIL) { // Remove Tail

        temp = list_ptr->tail;
        temp_data = temp->data_ptr;

        list_ptr->tail = list_ptr->tail->prev;
        list_ptr->tail->next = NULL;
    }
    else { // Remove pos_index node
        if (list_access(list_ptr, pos_index) == NULL) return NULL;

        temp = list_ptr->head;
        for ( int i = 0; i < pos_index; i++) temp = temp->next; // Get to the pos_index value
        temp_data = temp->data_ptr;

        next = temp->next;
        prev = temp->prev;

        prev->next = next;
        next->prev = prev;

    }
    list_ptr->current_list_size -= 1; // Decrement the list size
    temp->data_ptr = NULL;
    free(temp);

    list_debug_validate(list_ptr);

    return temp_data;
}

/* Reverse the order of the elements in the list. Also change the list_sorted_state flag.
 * This function can only be called on a list that is sorted.
 *
 * Inputs:
 *  list_t * list_ptr: Pointer to list-of-interest.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  list_t * list_ptr: The elements of the list provided will be reversed.
 *
 * Algorithm based on this article: https://www.geeksforgeeks.org/c-program-for-reverse-a-linked-list/
 *
 * Code copied from MP2.
 */
void list_reverse(list_t *list_ptr)
{
    assert(list_order(list_ptr) != 0);

    if (list_ptr->head != list_ptr->tail && list_ptr->current_list_size > 1) {

        list_node_t *prevNode, *curNode = list_ptr->head;
        list_ptr->tail = curNode;

        while ( curNode != NULL ) { // Reverse algorithm
            prevNode = curNode->prev;
            curNode->prev = curNode->next;
            curNode->next = prevNode;
            curNode = curNode->prev;
        }
        list_ptr->head = prevNode->prev;
    }

    // Inverts whatever the sorted state is
    list_ptr->list_sorted_state == LIST_SORTED_ASCENDING ? (list_ptr->list_sorted_state = LIST_SORTED_DESCENDING) :
    (list_ptr->list_sorted_state = LIST_SORTED_ASCENDING);

    // after the list is reversed verify it is valid.
    list_debug_validate(list_ptr);
}

/* The function list_size will return the stored size of the list provided.
 *
 * Inputs:
 *  list_t * list_ptr: A pointer to the list-of-interest.
 *
 * Returns: An integer value of how many elements are in the list provided. ( Empty = 0 )
 *
 * Structures Modified: N/A
 */
int list_size(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->current_list_size >= 0);
    return list_ptr->current_list_size;
}

/* The function list_order will return an integer value based on the current sorted state.
 *
 * Input:
 *  list_t * list_ptr: A pointer to the list we want to check.
 *
 * Return: An integer value determining what sorted state the list is in. It can be only one of the following:
 *      -1: Descending
 *       0: Unsorted ( Not sorted but in queue )
 *       1: Ascending ( Sorted )
 *
 * Structures Modified: N/A
 */
int list_order(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING)
        return 1;
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING)
        return -1;
    else if (list_ptr->list_sorted_state == LIST_UNSORTED)
        return 0;
    else
        exit(5);  // this should not happen
}

/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void list_debug_validate(list_t *L)
{
#ifdef VALIDATE
    list_node_t *N;
    int count = 0;
    assert(L != NULL);
//    if (VALIDATE_STATUS != 1)
//        return;

    if (L->head == NULL)
        assert(L->tail == NULL && L->current_list_size == 0);
    if (L->tail == NULL)
        assert(L->head == NULL && L->current_list_size == 0);
    if (L->current_list_size == 0)
        assert(L->head == NULL && L->tail == NULL);
    assert(L->list_sorted_state == LIST_SORTED_ASCENDING
            || L->list_sorted_state == LIST_SORTED_DESCENDING
            || L->list_sorted_state == LIST_UNSORTED);

    if (L->current_list_size == 1) {
        assert(L->head == L->tail && L->head != NULL);
        assert(L->head->next == NULL && L->head->prev == NULL);
        assert(L->head->data_ptr != NULL);
    }
    if (L->head == L->tail && L->head != NULL)
        assert(L->current_list_size == 1);
    if (L->current_list_size > 1) {
        assert(L->head != L->tail && L->head != NULL && L->tail != NULL);
        N = L->head;
        assert(N->prev == NULL);
        while (N != NULL) {
            assert(N->data_ptr != NULL);
            if (N->next != NULL)
                assert(N->next->prev == N);
            else
                assert(N == L->tail);
            count++;
            N = N->next;
        }
        assert(count == L->current_list_size);
    }
    if (L->list_sorted_state != LIST_UNSORTED && L->head != NULL) {
        N = L->head;
        int comp_val = -1 * list_order(L);
        while (N->next != NULL) {
            assert((L->comp_proc)(N->data_ptr, N->next->data_ptr) != comp_val);
            N = N->next;
        }
    }
#endif
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */