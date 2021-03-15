/* list.c
 * Casey Hird
 * crhird
 * ECE 2230 Spring 2019
 * MP3
 *
 * Propose: Implements the functions defined in list.h to create a
 * doubly linked list.
 *
 * Assumptions: All functions are used as defined in list.h.
 *
 * Bugs: No known bugs.
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
#define LIST_SORTED_DESCENDING  -61801
#define LIST_UNSORTED           -72467

/* prototypes for private functions used in list.c only */
void list_debug_validate(list_t *L);
void list_bubble_sort(list_t** L, int sort_order);
void list_insertion_sort(list_t** L, int sort_order);
void list_recursive_selection_sort(list_t** L, int sort_order);
void list_selection_sort(list_t** L, int sort_order);
void list_merge_sort(list_t** L, int sort_order);

/* sorting helper functions) */
void swap(list_node_t *A, list_node_t *B);
void SelectionSort(list_t** L, list_node_t* start, list_node_t* end, int sort_order);
list_node_t* find_max(list_t* L, list_node_t* start, list_node_t* end);
list_node_t* find_min(list_t* L, list_node_t* start, list_node_t* end);
void halve_list(list_t **L, list_t** left, list_t** right);
void merge_lists(list_t **L, list_t **left, list_t **right, int sort_order);

/* Sorts a list used a specific sorting algorithm and direction.
 *
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * sort_type: sorting algorithm used during sorting: 1) Bubble Sort;
 *            2)Insertion Sort; 3) Recursive Selection Sort;
 *            4) Iterative Selection Sort; and 5) Merge Sort
 *
 * sort_order: if 1 then the items are to be sorted in descending order.
 *             if 2 then the items are to be sorted in ascending order.
 *
 * return value: None.
 */
void list_sort(list_t **list_ptr, int sort_type, int sort_order)
{
	// Assert that neither the pointer to the list pointer
	// nor the list pointer itself is null.
	assert(list_ptr != NULL && *list_ptr != NULL);

	// If the list has more than 1 element, sort it
	if ((*list_ptr)->current_list_size > 1)
	{
		// Select the appropriate sort function
		switch (sort_type)
		{
			case 1:
				list_bubble_sort(list_ptr, sort_order);
				break;
			case 2:
				list_insertion_sort(list_ptr, sort_order);
				break;
			case 3:
				list_recursive_selection_sort(list_ptr, sort_order);
				break;
			case 4:
				list_selection_sort(list_ptr, sort_order);
				break;
			case 5:
				list_merge_sort(list_ptr, sort_order);
				break;
		}

	}

	// Set the given list order
	if (sort_order == 1)
		(*list_ptr)->list_sorted_state = LIST_SORTED_DESCENDING;
	else
		(*list_ptr)->list_sorted_state = LIST_SORTED_ASCENDING;
    //list_debug_validate(*list_ptr);
}

/* Sorting the list via the insertion sort algorithm
 *
 * L: pointer to list-of-interest.
 *
 * Precondition: (*L)->current_list_size > 1
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 */
void list_bubble_sort(list_t** L, int sort_order)
{
	// assert the precondition
	assert((*L)->current_list_size > 1);
	// Define a comparison based on the given sort_order that will be used
	// to compare elements.
	int compare;
	if (sort_order == 1)
		compare = 1;
	else
		compare = -1;
	int length = (*L)->current_list_size;
	int i, j;
	// Start with head node
	list_node_t *node = (*L)->head;
	for (i = 0; i < length-1; i++)
	{
		for (j = 0; j < length-i-1; j++)
		{
			// comp_proc returns 1 if the elements are already in ascending order
			// and -1 if they are in descending order
			if ((*L)->comp_proc(node->data_ptr,node->next->data_ptr) == compare)
				// Swap node and node->next
				swap(node,node->next);
			// Move to the next node
			node = node->next;
		}
		// Start over at head node
		node = (*L)->head;
	}
}

/*
	Swap 2 nodes in a given list.

	A: pointer to the front node

	B: pointer to the rear node

	Preconditions: nodes A and B are in the same list

	Postcondition: nodes A and B have switched positions
*/
void swap(list_node_t *A, list_node_t *B)
{
	// Swap the data pointers between the two nodes
	data_t *temp = A->data_ptr;
	A->data_ptr = B->data_ptr;
	B->data_ptr = temp;

	// Avoid dangling pointer
	temp = NULL;
}


/* Sorting the list via the insertion sort algorithm
 *
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 */
void list_insertion_sort(list_t** L, int sort_order)
{
	// assert the precondition
	assert((*L)->current_list_size > 1);

	// Create new list
	list_t *new_list = list_construct((*L)->comp_proc, (*L)->data_clean);
	// Set new_list order
	if (sort_order == 1)
		new_list->list_sorted_state = LIST_SORTED_DESCENDING;
	else
		new_list->list_sorted_state = LIST_SORTED_ASCENDING;
	// Create pointer to hold data
	int length = (*L)->current_list_size;
	int i;
	for (i = 0; i < length; i++)
	{
		// Remove the head of the old list (which will be a new node each time)
		// and insert into the new sorted list.
		list_insert_sorted(new_list, list_remove(*L,0));
	}
	// Destruct old list and point to new list
	list_destruct(*L);
	*L = new_list;
}

/* Sorting the list via the recursive selection sort algorithm
 *
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 */
void list_recursive_selection_sort(list_t** L, int sort_order)
{
	// assert the precondition
	assert((*L)->current_list_size > 1);
	// Check that the list ponter is not null
	assert(L != NULL && (*L) != NULL);

	SelectionSort(L, (*L)->head, (*L)->tail, sort_order);
}

/*
	Sorts the list with a recursive selection sort algorithm.

	L: pointer to the list to be sorted

	start: the first node in the list being sorted

	end: the last node in the list being sorted

    sort_order: 1 sort list in descending order 2 sort in ascending order	

	precondition: start is not later in the list than end
*/
void SelectionSort(list_t** L, list_node_t* start, list_node_t* end, int sort_order)
{
	list_node_t* swap_node;

	// If there is >1 node in the list, find the node to be swapped
	if (start != end)
	{
		if (sort_order == 1)
			swap_node = find_max(*L, start, end);
		else
			swap_node = find_min(*L, start, end);
	
		// Swap the node at the beginning and the node found
		swap(start, swap_node);

		// Call this function on the smaller list
		SelectionSort(L, start->next, end, sort_order);
	}
	swap_node = NULL;
}

/*
	Finds the max node on a range in a given list.
	Returns a pointer to the max node.

	L: the list to be searched

	start: the first position to be searched

	end: the last position to be searched
*/
list_node_t* find_max(list_t* L, list_node_t* start, list_node_t* end)
{
	// Set max equal to start node
	list_node_t* max = start;

	// Find the max node
	list_node_t* search_node = max->next;
	while (search_node != NULL)
	{
		// Set max to node if node data > max data
		if (L->comp_proc(max->data_ptr, search_node->data_ptr) == 1)
			max = search_node;
		search_node = search_node->next;
	}
	// Should have reached the end of the list
	assert(search_node == NULL);
	return max;
}

/*
	Finds the min node on a range in a given list.
	Returns a pointer to the min node.

	L: the list to be searched

	start: the first position to be searched

	end: the last position to be searched
*/
list_node_t* find_min(list_t* L, list_node_t* start, list_node_t* end)
{
	// Set max equal to the node at position start
	list_node_t* min = start;

	// Find the max node
	list_node_t* search_node = min->next;
	while (search_node != NULL)
	{
		// Set min to node if node data < min data
		if (L->comp_proc(min->data_ptr, search_node->data_ptr) == -1)
			min = search_node;
		search_node = search_node->next;
	}
	// Should have reached the end of the list
	assert(search_node == NULL);
	return min;
}

/* Sorting the list via the selection sort algorithm
 *
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 */
void list_selection_sort(list_t** L, int sort_order)
{
	// assert the precondition
	assert((*L)->current_list_size > 1);
	// Set up for comparison direction
	int compare;
	if (sort_order == 1)
		compare = 1;
	else
		compare = -1;
	int i;
	int start = 0;
	int end = (*L)->current_list_size - 1;
	list_node_t *start_node, *swap_node, *search_node;
	while (start < end)
	{
		// Find start_node for this iteration
		start_node = (*L)->head;
		for (i = 0; i < start; i++)
			start_node = start_node->next;
		
		// Find the swap node (min or max) between start and end
		assert(i == start);
		i++;
		swap_node = start_node;
		search_node = swap_node->next;
		while (i <= end)
		{
			if ((*L)->comp_proc(swap_node->data_ptr, search_node->data_ptr) == compare)
				swap_node = search_node;
			search_node = search_node->next;
			i++;
		}

		// Swap swap_node and start_node;
		swap(start_node, swap_node);
	
		start++;
	}
}

/* Sorting the list via the merge sort algorithm
 *
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 */
void list_merge_sort(list_t** L, int sort_order)
{
	int length = (*L)->current_list_size;
	// Do nothing if there is one element in the list
	if (length > 1)
	{
		// Create two lists to hold the halves
		list_t *left = list_construct((*L)->comp_proc, (*L)->data_clean);
		list_t *right = list_construct((*L)->comp_proc, (*L)->data_clean);
		// Divide the list int two halves
		halve_list(L, &left, &right);
		// Sort both smaller lists
		list_merge_sort(&left, sort_order);
		list_merge_sort(&right, sort_order);
		// Merge lists left and right
		merge_lists(L, &left, &right, sort_order);
	}
}

/*
	Removes the elements from one list and places half of them
	into one list and the other half in another.

	L: the list which holds the elements to be removed

	left: the list which will hold the first half of L's elements

	right: the list which will hold the second half of L's elements
*/
void halve_list(list_t **L, list_t** left, list_t** right)
{
	// Find length of a half list
	int half_length = (*L)->current_list_size / 2;
	// Find the midpoint
	list_node_t *midpoint = (*L)->head;
	int i;
	for (i = 1; i < half_length; i++)
		midpoint = midpoint->next;
	// Assign left list
	(*left)->head = (*L)->head;
	(*left)->tail = midpoint;
	(*left)->current_list_size = half_length;
	// Assign right list
	(*right)->head = midpoint->next;
	(*right)->tail = (*L)->tail;
	// Break lists apart be setting ends to NULL
	(*left)->tail->next = NULL;
	(*right)->head->prev = NULL;
	// Make right an element shorter if L had an odd number of elements
	if ((*L)->current_list_size % 2 == 0)
		(*right)->current_list_size = half_length;
	else
		(*right)->current_list_size = half_length+1;
	// Deallocate the list L and remove the dangling pointer
	free(*L);
	*L = NULL;
}

/*
	Merges two lists into a single sorted list.

	L: intially empty list that will finish will all elements from the
	other two lists sorted according to sort_order

	left: one sublist that will have its elements moved

	right: the other sublist that will have its elements moved

	sort_order: 1 sort list in descending order 2 sort in ascending order
*/
void merge_lists(list_t **L, list_t **left, list_t **right, int sort_order)
{
	// Determine how to compare elements based on the sort order
	int compare;
	if (sort_order == 1)
		compare = -1;
	else
		compare = 1;
	// Allocate the new merged list
	*L = list_construct((*left)->comp_proc, (*left)->data_clean);
	int num_elements =(*left)->current_list_size + (*right)->current_list_size;
	int i;
	data_t* data;
	// Do this while there are elements remaining in either half list
	//while (!((*left)->current_list_size == 0 && 
	//	(*right)->current_list_size == 0))
	for (i = 0; i < num_elements; i++)
	{
		// If the left list is empty, take from the right
		if ((*left)->current_list_size == 0)
		{
			data = list_remove(*right, 0);
			list_insert(*L, data, LISTPOS_TAIL);
		}

		// If the right list is empty, take from the left
		else if ((*right)->current_list_size == 0)
		{
			data = list_remove(*left, 0);
			list_insert(*L, data, LISTPOS_TAIL);
		}

		// If neither list is empty, compare the heads of the lists,
		// and take from the appropriate list
		else
		{
			// Compare the list heads
			if ((*L)->comp_proc((*left)->head->data_ptr,
				(*right)->head->data_ptr) == compare)
			{
				data = list_remove(*left, 0);
				list_insert(*L, data, LISTPOS_TAIL);
			}
			else
			{
				data = list_remove(*right, 0);
				list_insert(*L, data, LISTPOS_TAIL);
			}
		}
	}
	// Destroy the two smaller lists and remove dangling pointers
	list_destruct(*left);
	list_destruct(*right);
	*left = NULL;
	*right = NULL;
}

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

    /* TODO: handle four special cases.
     *   1.  The list is empty
     *   2.  Asking for the head
     *   3.  Asking for the tail
     *   4.  Asking for invalid pos
     */
     // Case 1
   	if (list_ptr->head == NULL){
   	 	return NULL;
   	}
   	// Case 4
   	else if (pos_index < 0 || pos_index >= list_ptr->current_list_size){
   		return NULL;
   	}
   	// Case 2
   	else if (pos_index == 0 || pos_index == LISTPOS_HEAD){
   		L = list_ptr->head;
   	}
   	// Case 3
   	else if (pos_index == LISTPOS_TAIL ||
   		pos_index == (list_ptr->current_list_size)-1){

   		L = list_ptr->tail;
   	}
    /* we now know pos_index is for an interal element */
    /* TODO: loop through the list until find correct position index */
    else{
      int i;
      L = list_ptr->head;
      for (i = 0; i < pos_index; i++){
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
	data_t* d = NULL;

    /*TODO: */
    /* fix the return value */
	// If list is empty or uninitialized return NULL and -1
	if (list_ptr == NULL || list_ptr->head == NULL){
		*pos_index = -1;
		return NULL;
	}
	// Search for matching element
	else{
		*pos_index = 0;
		list_node_t* N = list_ptr->head;
		while (*pos_index<(list_ptr->current_list_size)
			&& list_ptr->comp_proc(N->data_ptr, elem_ptr) == 0 ){

			N = N->next;
			(*pos_index)++;
		}
		// If we went past the end of the list, need to set pos_index
		if (*pos_index == list_ptr->current_list_size){
			*pos_index = -1;
		}
		// Otherwise, pos_index is at the correct value nad we need to set d
		else{
			d = N->data_ptr;
		}
	}

    return d;
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

	// Create and allocate memory for new list node
	list_node_t *new_node = (list_node_t*)malloc(sizeof(list_node_t));
	new_node->data_ptr = elem_ptr;
	// If pos_index is an invalid negative value, exit the function
	if (pos_index < 0 && pos_index != LISTPOS_HEAD && pos_index != LISTPOS_TAIL){
		return;
	}
	// Insert into empty list
	else if (list_ptr->head == NULL && list_ptr->tail == NULL){
		new_node->prev = NULL;
		new_node->next = NULL;
		list_ptr->head = new_node;
		list_ptr->tail = new_node;
	}
	// Insert element at head
	else if (pos_index == 0 || pos_index == LISTPOS_HEAD){
		new_node->prev = NULL;
		new_node->next = list_ptr->head;
		list_ptr->head->prev = new_node;
		list_ptr->head = new_node;
	}
	// Insert element at tail
	else if (pos_index >= list_ptr->current_list_size || pos_index == LISTPOS_TAIL){
		new_node->prev = list_ptr->tail;
		new_node->next = NULL;
		list_ptr->tail->next = new_node;
		list_ptr->tail = new_node;
	}
	// Insert element anywhere else in the list
	else{
		list_node_t *front = list_ptr->head;
		list_node_t *back;
		int i;
		// Get the node currently at pos_index
		for (i = 0; i < pos_index; i++){
			front = front->next;
		}
		back = front->prev;
		new_node->prev = back;
		back->next = new_node;
		new_node->next = front;
		front->prev = new_node;
	}
	// Add one to list size
	(list_ptr->current_list_size)++;

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
 * in decending order, change the sign of the value that is returned.
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 */
void list_insert_sorted(list_t *list_ptr, data_t *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->list_sorted_state != LIST_UNSORTED);
	int index;
	int current_state = list_ptr->list_sorted_state;

    /* insert your code here */
    /*TODO*/
	// Place data if list is empty
	if (list_ptr->head == NULL && list_ptr->tail == NULL){
		index = 0;
	}
	// Find the index at which to place the data
	index = 0;
	list_node_t *node = list_ptr->head;
	// Find index for ascending sort
	if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING){
		while (node != NULL && list_ptr->comp_proc(node->data_ptr,elem_ptr) != -1){
			index++;
			node = node->next;
		}
	}
	// Find index for descending sort
	else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING){
		while (node != NULL && list_ptr->comp_proc(node->data_ptr,elem_ptr) != 1){
			index++;
			node = node->next;
		}
	}
	// Use the list_insert() function to insert at the found index
	list_insert(list_ptr, elem_ptr, index);
	// Reset the list state
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
    list_debug_validate(list_ptr);
	// NOTE: we could write less code by using the list_access() function to
	// return the desired data, but I decided instead to repeat some of that
	// code here to avoid having to traverse the list a second time for removal.

	list_node_t *lead, *follow, *removed;
	data_t *data;

	// If the list does not exist, the list is empty, or the given index
	// does not exist, return NULL.
	if (list_ptr == NULL || (list_ptr->head == NULL && list_ptr->tail == NULL)
		|| (pos_index < 0 && pos_index != LISTPOS_HEAD && pos_index != LISTPOS_TAIL)
		|| pos_index >= list_ptr->current_list_size){

		return NULL;
	}
	// If there is only 1 node and the head or tail is chosen, remove it
	else if ((pos_index == 0 ||pos_index == LISTPOS_HEAD
	|| pos_index == LISTPOS_TAIL)
		&& list_ptr->current_list_size == 1){

		removed = list_ptr->head;
		data = removed->data_ptr;
		list_ptr->head = NULL;
		list_ptr->tail = NULL;
	}
	// Remove the list head if requested
	else if (pos_index == 0 || pos_index == LISTPOS_HEAD){
		follow = list_ptr->head;
		removed = follow;
		data = follow->data_ptr;
		lead = follow->next;
		lead->prev = NULL;
		list_ptr->head = lead;
	}
	// Remove the list tail if requested
	else if (pos_index == (list_ptr->current_list_size)-1
		|| pos_index == LISTPOS_TAIL){

		lead = list_ptr->tail;
		removed = lead;
		data = lead->data_ptr;
		follow = lead->prev;
		follow->next = NULL;
		list_ptr->tail = follow;
	}
	// Remove any other node requested
	else{
		removed = list_ptr->head;
		// Get to the index given
		int i;
		for (i = 0; i < pos_index; i++){
			removed = removed->next;
		}
		data = removed->data_ptr;
		follow = removed->prev;
		lead = removed->next;
		follow->next = lead;
		lead->prev = follow;
	}
	// Decrement the list size
	(list_ptr->current_list_size)--;
	// Free the removed node memory
	free(removed);
	// Prove that the data has been found if the function reached this point.
    assert(data != NULL);
	return data;
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
	// If the list does not exist, is empty, or only has 1 element, we don't
	// need to do anything
	if (list_ptr == NULL || (list_ptr->head == NULL && list_ptr->tail == NULL)
		|| list_ptr->head == list_ptr->tail){
	}
	// If the list has only 1 element, skip this and just change the state
	else if (list_ptr->head != list_ptr->tail){
		// Otherwise, swap the prev and next pointers at each node
		list_node_t *node, *temp;
		node = list_ptr->head;
		// Set the first node as the new tail
		list_ptr->tail = node;
		// Swap prev and next
		while (node != NULL){
			temp = node->prev;
			node->prev = node->next;
			node->next = temp;
			node = node->prev;
		}
		// Set the last node as the new head
		list_ptr->head = temp->prev;
	}
	// Reverse the list_sorted_state
	if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING){
		list_ptr->list_sorted_state = LIST_SORTED_DESCENDING;
	}
	else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING){
		list_ptr->list_sorted_state = LIST_SORTED_ASCENDING;
	}
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
#ifdef VALIDATE
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
#endif
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
