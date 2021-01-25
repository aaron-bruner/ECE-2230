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
    struct atom_list_t * atom_list_head = malloc(sizeof(struct atom_list_t));
    if (atom_list_head == NULL) {
        exit(1);
    }

    atom_list_head->atom_ptr = malloc(size * sizeof(struct atom_t));
    if (atom_list_head->atom_ptr == NULL) {
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
    free(ptr->atom_ptr);
    free(ptr);
}

int atom_list_number_entries(struct atom_list_t *list_ptr)
{
    return list_ptr->atom_count;
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

    if (index < 0 ||index >= list_ptr->atom_count)
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


void atom_list_advance_momenta(struct atom_list_t *list_ptr, float dt)
{
    for (int i = 0; i < list_ptr->atom_count; i++) {
        list_ptr->atom_ptr[i]->momenta[0] += dt * list_ptr->atom_ptr[i]->force[0];
        list_ptr->atom_ptr[i]->momenta[1] += dt * list_ptr->atom_ptr[i]->force[1];
    }
}

void atom_list_advance_position(struct atom_list_t *list_ptr, float dt)
{
    for (int i = 0; i < list_ptr->atom_count; i++) {
        list_ptr->atom_ptr[i]->position[0] += (dt*list_ptr->atom_ptr[i]->momenta[0])/(list_ptr->atom_ptr[i]->mass);
        list_ptr->atom_ptr[i]->position[1] += (dt*list_ptr->atom_ptr[i]->momenta[1])/(list_ptr->atom_ptr[i]->mass);
    }
}

void atom_list_compute_forces(struct atom_list_t *list_ptr)
{
    for (int i = 0; i < list_ptr->atom_count; i++) {
        list_ptr->atom_ptr[i]->force[0] = drand48();
        list_ptr->atom_ptr[i]->force[1] = drand48();
        list_ptr->atom_ptr[i]->potential_energy = drand48();
    }
}

int atom_list_lookup_max_potential_energy(struct atom_list_t *list_ptr, int potential_energy)
{
    int found = -1;

    

    return found;
}

int atom_list_determine_inside_box(struct atom_t *atom_ptr, float x_min, float x_max,
                                                            float y_min, float y_max)
{


    return 1;
}

struct atom_list_t *atom_list_form_migrate_list(struct atom_list_t *list_ptr, float x_min, float x_max,
                                                                              float y_min, float y_max)
{
    return NULL;
}




