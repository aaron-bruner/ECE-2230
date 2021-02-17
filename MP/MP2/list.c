/* list.c
 * Aaron Bruner
 * ajbrune
 * ECE 2230 Spring 2021
 * MP2
 *
 * Propose: The purpose of MP 2 is to build an understanding of two-way linked lists. We based the idea of this
 *          machine problem mostly on MP 1, which used a dynamic array. Now, we're referencing our new list
 *          which should be more reusable for future assignments.
 *
 * Assumptions: We must assume that the user operating the code understands how to properly run the program. Also
 *              we must assume that the user only provides valid input which will not break the lab2.c file.
 *
 * Bugs: N/A
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

/* prototypes for private functions used in list.c only */
void list_debug_validate(list_t *L);


/* ----- below are the functions  ----- */

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.  For example, if the list is empty, NULL is returned.
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

/* Allocates a new, empty list 
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values LIST_SORTED_ASCENDING LIST_SORTED_DESCENDING or 
 * LIST_UNSORTED
 *
 * Use list_free to remove and deallocate all elements on a list (retaining the
 * list itself).
 *
 * comp_proc = pointer to comparison function
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

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 *           Note: use the comp_proc function pointer found in the list_t 
 *           header block. 
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1. 
 *
 * pos_index: used as a return value for the position index of matching element
 *
 *
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

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list, and the header block.  Use the data_clean
 * function point to free the data_t items in the list.
 */
void list_destruct(list_t *list_ptr)
{
    /* the first line must validate the list */
    list_debug_validate(list_ptr);

    assert(list_ptr != NULL);

    // Set up loop variables, one to lead and one to trail
    list_node_t *A, *B;
    // If list is empty, skip
    if (list_ptr->head == NULL && list_ptr->tail == NULL){
        assert(list_ptr->current_list_size == 0);
    }
        // If there is only 1 node
    else if (list_ptr->current_list_size == 1){
        free(list_ptr->head);
    }
        // Otherwise, there are multiple nodes
    else{
        // Initialize loop variables
        A = list_ptr->head;
        B = A->next;
        // For each node, free the data then the node itself
        while (B != NULL){
            list_ptr->data_clean(A->data_ptr);
            free(A);
            A = B;
            B = B->next;
        }
        list_ptr->data_clean(A->data_ptr);
        free(A);
    }
    // Free the last node and the header block
    free(list_ptr);

    /*
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
    } */

}

/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the 
 *            list.  Index starts at 0 at head of the list, and incremented by 
 *            one until the tail is reached.  The index can also be equal
 *            to LISTPOS_HEAD or LISTPOS_TAIL (these are special negative 
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list, 
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->list_sorted_state must be equal 
 * to LIST_UNSORTED.
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

/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the comp_proc function pointer found in the header block.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
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
 * list_ptr: pointer to list-of-interest.  
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
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

/* Reverse the order of the elements in the list.  Also change the 
 * list_sorted_state flag.  This function can only be called on a list
 * that is sorted.
 *
 * list_ptr: pointer to list-of-interest.
 */
void list_reverse(list_t *list_ptr)
{
    assert(list_order(list_ptr) != 0);

    //if (list_ptr == NULL || list_ptr->current_list_size == 1 || (list_ptr->head == NULL && list_ptr->tail == NULL));

    if (list_ptr->head != list_ptr->tail && list_ptr->current_list_size > 1) {

        list_node_t *prevNode, *curNode = list_ptr->head;
        list_ptr->tail = curNode;

        while ( curNode != NULL ) {
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

/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int list_size(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->current_list_size >= 0);
    return list_ptr->current_list_size;
}

/* Obtains the sort status and order of the specified list. 
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns 
 *    1: the list is sorted in ascending order
 *   -1: descending order
 *    0: the list is not sorted but a queue
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
    list_node_t *N;
    int count = 0;
    assert(L != NULL);
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
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

