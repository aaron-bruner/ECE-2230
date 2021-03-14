/* 

   Writing and debugging MP2 is challenging, and it is particularly hard if you
   try and write all the code before you do any testing. It is much easier to
   break the problem into small pieces and test each piece. 

For MP2, one approach is to write list_insert first, since it might be the
easiest algorithm and it depends only on list_construct.

   For testing, we use a simple main function in this file.  This file is for
   testing only, and you will NOT submit it as it is for your personal testing
   during development.

Below are a few simple examples that allocates a few structures and inserts
them into a list.

With this file containing a temporary main section, you can compile and run
list.c on its own using 
   gcc -Wall -g testlist.c list.c atom_list_support.c -o testlist

The compling command is also found in the provided makefile.  Do
   make testlist

Do this testing before you make ANY changes to atom_list_support.c.  The only 
reason we need to compile atom_list_support.c is for the atom_list_compare function.
The linker needs to find the comp_proc function and it is found in the template
   atom_list_support.c file (with no changes) 

*/

#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"
#include "list.h"
#include "atom_support.h"

/* prototype for list debug print */
void list_debug_print(list_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    list_t *Lptr = NULL;

    Lptr = list_construct(atom_compare, atom_rec_cleanup);

    // create one item to test list_insert
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->atom_id = 55;
    list_insert(Lptr, datap, LISTPOS_HEAD);
    datap = NULL;

    // test list_access with one item in list
    datap = list_access(Lptr, LISTPOS_HEAD);
    printf("Should find 55 and found (%d)\n\n", datap->atom_id);
    datap = NULL;

    // add a second item to tail of the list
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->atom_id = 44;
    list_insert(Lptr, datap, LISTPOS_TAIL);
    datap = NULL;

    // add a third item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->atom_id = 33;
    list_insert(Lptr, datap, LISTPOS_TAIL);
    datap = NULL;


    // find all three and print 
    datap = list_access(Lptr, 0); 
    printf("Second test\nPosition 0 should find 55 and found (%d)\n", datap->atom_id);
    datap = list_access(Lptr, 1);
    printf("Position 1 should find 44 and found (%d)\n", datap->atom_id);
    datap = list_access(Lptr, 2);
    printf("Position 2 should find 33 and found (%d)\n", datap->atom_id);

    //Next try to use list_debug_print 
    printf("\nTest of list print\n\n");
    list_debug_print(Lptr);


    // Uncomment this section to test list_elem_find 
    /*********************************************************************
    */data_t template;
    int my_index = -999;
    template.atom_id = 44;
    data_t *foundp = list_elem_find(Lptr, &template, &my_index);
    printf("\nTest of list elem find\n");
    if (foundp != NULL)
        printf("looked for %d and found %d at index %d\n",
            template.atom_id, foundp->atom_id, my_index);
    else
        printf("looked for %d and did not find \n", template.atom_id);
    foundp = NULL;
/*
    */

    list_destruct(Lptr);
 

    return 0;
}

/*

Next you will want to write your own list_debug_print function to print a
list. Then you can do "before and after" testing. That is, print the list
before a change, and print the list after the change to verify that the
change worked.

Something like

*/

void list_debug_print(list_t *list_ptr) {
    list_node_t *L;
    int count;

    printf("List size: %d\n", list_ptr->current_list_size);
    L = list_ptr->head;
    count = 0;
    while(L != NULL) {
        printf("Pos: %d contains atom id: %d\n", count, L->data_ptr->atom_id);
        L = L->next;
        count++;
    }
    if (count != list_ptr->current_list_size)
        printf("Hey!! the list size is wrong! count is %d\n", count);
}

