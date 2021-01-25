// See ECE 2230 programming guide for required comments!
//
// You must have comments at the top of the file.
//
// EVERY function must have comments

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atom_list.h"

struct atom_list_t *atom_list_construct(int size)
{
    struct atom_list_t *atom_list_head;
    atom_list_head = NULL;
    atom_list_head = malloc(sizeof(struct atom_list_t));
    if (atom_list_head == NULL) {
        //fprintf(stderr, "Error allocating memory...");
        exit(1);
    }

    atom_list_head->atom_ptr = malloc(size * sizeof(struct atom_t));
    if (atom_list_head->atom_ptr == NULL) {
        //fprintf(stderr, "Error allocating memory...");
        exit(1);
    }

    for(int i = 0; i < size; i++) {
        atom_list_head->atom_ptr[i] = NULL;
    }

    atom_list_head->atom_count = 0;
    atom_list_head->atom_size = size;

    return atom_list_head;
}

void atom_list_destruct(struct atom_list_t *ptr)
{
    int tempVar = ptr->atom_size;
    for (int i = tempVar - 1; i >= 0; i--) // valgrind complains that 400 bytes in 1 blocks definitely lost
        free(ptr->atom_ptr[i]);
    free(ptr->atom_ptr); // All heap blocks freed
    free(ptr);
}

int atom_list_number_entries(struct atom_list_t *list_ptr)
{
    int list_size = -1;

    list_size = list_ptr->atom_count;

    return list_size;
}

int atom_list_add(struct atom_list_t *list_ptr, struct atom_t *rec_ptr)
{
    int returnVal;

    if (list_ptr->atom_count == list_ptr->atom_size-1)
        returnVal = -1;
    else {
        list_ptr->atom_ptr[list_ptr->atom_count] = rec_ptr;
        returnVal = 1;
        list_ptr->atom_count += 1;
    }

    return returnVal;
}

struct atom_t *atom_list_access(struct atom_list_t *list_ptr, int index)
{
    struct atom_t * atom_t_ptr = NULL;

    if (index >= list_ptr->atom_size)
        atom_t_ptr = NULL;
    else if (list_ptr->atom_ptr[index] != NULL)
            atom_t_ptr = list_ptr->atom_ptr[index];

    return atom_t_ptr;
}

struct atom_t *atom_list_remove(struct atom_list_t *list_ptr, int index)
{

    struct atom_t * atom = atom_list_access(list_ptr, index);

    if (atom == NULL) {
        atom = NULL;
    }

    for (int i = index; i < list_ptr->atom_count - 1; i++) {
        list_ptr->atom_ptr[i] = list_ptr->atom_ptr[i+1];
    }
    --list_ptr->atom_count;

    return atom;

}

/*//int * return_ptr;
struct atom_t * return_ptr;

if (list_ptr->atom_ptr == NULL) {
    return_ptr = NULL;
}

else if (list_ptr->atom_count == 1 && index == 0) {
    list_ptr->atom_ptr[index] = NULL;
    return_ptr = list_ptr->atom_ptr;
    list_ptr->atom_count -= 1;
}
else {
    list_ptr->atom_count -= 1;
    for (int i = index; i < list_ptr->atom_count; i++) {
        list_ptr->atom_ptr[i] = list_ptr->atom_ptr[i+1];
    }
    return_ptr = list_ptr->atom_ptr;
}

return return_ptr;*/

/*int * returnStructPtr;
returnStructPtr = malloc(list_ptr->atom_size * sizeof(struct atom_t));
if (returnStructPtr == NULL) {
    //fprintf(stderr, "Error allocating memory...");
    exit(1);
}

for(int i = 0; i < list_ptr->atom_size; i++) {
    returnStructPtr[i] = NULL;
}

for (int i = 0; i < index; i++) {
    memcpy(&returnStructPtr[i], list_ptr->atom_ptr[i], sizeof(struct atom_t));
}
for (int i = index+1; i < list_ptr->atom_count; i++) {
    memcpy(&returnStructPtr[i-1], list_ptr->atom_ptr[i], sizeof(struct atom_t));
}

list_ptr->atom_count -= 1;

for (int i = list_ptr->atom_count + 1; i < list_ptr->atom_size; i++){
    returnStructPtr[i] = NULL;
}

for (int i = 0; i < list_ptr->atom_size - 1; i++) {
    free(list_ptr->atom_ptr[i]);
}*/

/*if (list_ptr != NULL){ // If it's already empty then just ignore...
        if (list_ptr->atom_ptr[index] == NULL || index == 1) { // If the list has 1 atom
            free(list_ptr->atom_ptr[index]);
            returnVal = list_ptr->atom_ptr;
        } else { //If it's not empty and has more than 1 entry...

            free(list_ptr->atom_ptr[index]);

            while (list_ptr->atom_size !>= tempIndex)

                free(list_ptr->atom_ptr[index]);
                struct atom_t * temp = malloc (sizeof(struct atom_t));
                memcpy(temp, list_ptr->atom_ptr[index+1], sizeof(struct atom_t));
                memcpy(list_ptr->atom_ptr[index], temp, sizeof(struct atom_t));

                free(temp);
                tempIndex++;
            }
        }
}*/

void atom_list_advance_momenta(struct atom_list_t *list_ptr, float dt)
{
}

void atom_list_advance_position(struct atom_list_t *list_ptr, float dt)
{
}

void atom_list_compute_forces(struct atom_list_t *list_ptr)
{
}


int atom_list_lookup_max_potential_energy(struct atom_list_t *list_ptr,
        int potential_energy)
{
    return -1;
}

int atom_list_determine_inside_box(struct atom_t *atom_ptr, float x_min,
                        float x_max, float y_min, float y_max)
{
    return 1;
}

struct atom_list_t *atom_list_form_migrate_list(struct atom_list_t *list_ptr,
                        float x_min, float x_max, float y_min, float y_max)
{
    return NULL;
}




