/* Name:         Aaron Bruner
 * Course #:     2230
 * Semester:     Spring 2021
 * Assignment #: Lab 1
 *
 * Purpose: The purpose of this file is to give function to the lab1.c file. Lab1.c prints a menu for the user to do
 *          whaterver they want with each option. This file provides the functionality to each command that is entered.
 *
 * Assumptions: There is only one true assumption for this machine problem. We are assuming that the user who is running
 *              this application uses realistic list sizes and that their machine can handle whatever size they input.
 *
 *              Massive numbers for the list size could crash the program which is not protected by design.
 *
 * Known Bugs: None that I know of.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atom_list.h"

/* The function atom_list_construct is designed to create our header block along with 'size' amount of NULL atom_ptr's.
 *
 * Input:
 *  int size: Size is passed in from the command line as the size of our list of atoms.
 *
 * Return:
 *  struct *atom_list_head: atom_list_head is a pointer to the head block of our new structure.
 *
 * Structures Modified:
 *  atom_list_head: Created with initial atom_count being 0 and atom_size being whatever value was passed in.
 */
struct atom_list_t *atom_list_construct(int size)
{
    struct atom_list_t * atom_list_head = malloc(sizeof(struct atom_list_t));
    if (atom_list_head == NULL) { // Check if malloc failed (clean exit)
        exit(1);
    }

    atom_list_head->atom_ptr = malloc(size * sizeof(struct atom_t));
    if (atom_list_head->atom_ptr == NULL) { // Check if malloc failed (clean exit)
        exit(1);
    }

    for(int i = 0; i < size; i++) { // Initialize all atoms to NULL
        atom_list_head->atom_ptr[i] = NULL;
    }

    atom_list_head->atom_count = 0;
    atom_list_head->atom_size = size;

    return atom_list_head;
}

/* The function atom_list_destruct frees the allocated memory for the header block that is passed in.
 *
 * Input:
 *  struct atom_list_t *ptr: Pointer to header block to be freed.
 *
 * Return: N/A
 *
 * Structures Modified: N/A
 */
void atom_list_destruct(struct atom_list_t *ptr)
{
    free(ptr->atom_ptr); // Release all memory that was allocated
    free(ptr);
}

/* The function atom_list_number_entries returns the number of atoms in the structure's list.
 *
 * Input:
 *  struct atom_list_t *list_ptr: Pointer to header block in question.
 *
 * Return:
 *  int atom_count: Number of atoms in the array
 *
 * Structures Modified: N/A
 */
int atom_list_number_entries(struct atom_list_t *list_ptr)
{
    return list_ptr->atom_count;
}

/* The function atom_list_add will add the atom that is passed in to the end of the list provided.
 *
 * Input:
 *  struct atom_list_t *list_ptr: This is a pointer to the header block that the atom should be added to.
 *  struct atom_t *rec_ptr: This is a pointer to the atom (with information) that should be put in the list.
 *
 * Return:
 *  int returnVal:
 *               -1: If -1 is returned then the list is full.
 *                1: If 1 is returned then the atom was successfully added.
 *
 * Structures Modified:
 *  struct atom_list_t *list_ptr: The struct atom_t was added to the end of the list.
 */
int atom_list_add(struct atom_list_t *list_ptr, struct atom_t *rec_ptr)
{
    int returnVal;

    if (list_ptr->atom_count == list_ptr->atom_size) // Checks to see if list is full
        returnVal = -1;
    else {
        list_ptr->atom_ptr[list_ptr->atom_count] = rec_ptr; // If the list isn't empty then add the given atom and
        returnVal = 1;                                      // increase the atom_count variable.
        list_ptr->atom_count += 1;
    }

    return returnVal;
}

/* The function atom_list_access returns a pointer to the atom_t memory block of 'index,' assuming it's valid.
 *
 * Input:
 *  struct atom_list_t *list_ptr: The pointer to the head of our list.
 *  int index: The index value we want the pointer for.
 *
 * Return:
 *  struct *atom_t: A pointer to the atom in our list at value 'index' OR NULL if it's not a valid index value.
 *
 * Structures Modified: N/A
 */
struct atom_t *atom_list_access(struct atom_list_t *list_ptr, int index)
{
    struct atom_t * atom_t_ptr = NULL;

    if (index < 0 || index >= list_ptr->atom_count) // Check for valid index number
        atom_t_ptr = NULL;

    else if (list_ptr->atom_ptr[index] != NULL) // If valid, return the pointer to that index value
            atom_t_ptr = list_ptr->atom_ptr[index];

    return atom_t_ptr;
}

/* The function atom_list_remove will delete index value out of the list, assuming it's valid.
 *
 * Input:
 *  struct atom_list_t *list_ptr: A pointer to the head of our structure.
 *  int index: The index value we want to delete.
 *
 * Return:
 *  struct *atom_t: The pointer to our memory block
 *
 * Structures Modified:
 *  struct atom_list_t *list_ptr: The 'index' value was removed from the list and all elements to the right were shifted
 *                                left.
 */
struct atom_t *atom_list_remove(struct atom_list_t *list_ptr, int index)
{
    struct atom_t * atom = atom_list_access(list_ptr, index);

    if (atom == NULL) atom = NULL; // If list is empty then we can't remove anything

    for (int i = index; i < list_ptr->atom_count - 1; i++) // Take whatever is to the right and set it to current spot
        list_ptr->atom_ptr[i] = list_ptr->atom_ptr[i+1];

    --list_ptr->atom_count;

    return atom;
}

/* The function atom_list_advance_momenta takes our list pointer and applies a mathematical function to the momenta of
 * every atom in our list.
 *
 * Input:
 *  struct atom_list_t *list_ptr: This is a pointer to the header block that we want to apply our math to.
 *  float dt: A float value for delta t in our function.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  struct atom_list_t *list_ptr: The momenta for every atom in our list is modified by the following formula:
 *
 *  momenta = momenta + dt * force
 */
void atom_list_advance_momenta(struct atom_list_t *list_ptr, float dt)
{
    for (int i = 0; i < list_ptr->atom_count; i++) // Loop through every atom
        for (int j = 0; j < 2; j++)
            list_ptr->atom_ptr[i]->momenta[j] += dt * list_ptr->atom_ptr[i]->force[j];
}

/* The function atom_list_advance_position will apply a mathmatical formula to the position of every atom in our list.
 *
 * Input:
 *  struct atom_list_t *list_ptr: The pointer to the head block of which we're going to apply our math to.
 *  float dt: The delta t value we're going to use in our formula.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  struct atom_list_t *list_ptr: All atoms in our list will have the following formula applied to their position:
 *
 *  position = position + (dt * momenta)/mass
 */
void atom_list_advance_position(struct atom_list_t *list_ptr, float dt)
{
    for (int i = 0; i < list_ptr->atom_count; i++) // Loop through every atom
        for (int j = 0; j < 2; j++)
            list_ptr->atom_ptr[i]->position[j] += (dt*list_ptr->atom_ptr[i]->momenta[j])/(list_ptr->atom_ptr[i]->mass);
}

/* The function atom_list_compute_forces assigns a new force & potential energy to all of the atoms.
 *
 * Input:
 *  struct atom_list_t *list_ptr: The pointer to a header block of which we're going assign random values to force &
 *                                potential energy.
 *
 * Return: N/A
 *
 * Structures Modified:
 *  struct atom_list_t *list_ptr: All atoms in this list will have their force and potential_energy
 *                                randomized by drand48.
 */
void atom_list_compute_forces(struct atom_list_t *list_ptr)
{
    for (int i = 0; i < list_ptr->atom_count; i++) { // Loop through every atom and assign a random number
        for (int j = 0; j < 2; j++)
            list_ptr->atom_ptr[i]->force[j] = drand48();
        list_ptr->atom_ptr[i]->potential_energy = drand48();
    }
}

/* The function atom_list_lookup_max_potential_energy will check the list that is provided and determine if an atom in
 * the list has the provided potential energy. If it is found, we want to return the smallest index value it's in.
 *
 * Input:
 *  struct atom_list_t *list_ptr: The pointer to the head block that contains our list.
 *  float potential_energy: The potential energy value we're looking for.
 *
 * Return:
 *  int found:
 *            -1: The potential energy value was not found
 *         index: The index location where the atom was found that had the value we were looking for.
 *
 * Structures Modified:
 */
int atom_list_lookup_max_potential_energy(struct atom_list_t *list_ptr, float potential_energy)
{
    int found = 0;

    if (list_ptr == NULL || list_ptr->atom_ptr == NULL){ // If the list is empty it's clearly not there
        found = -1;
    }

    if (found != -1) {
        for (int index = 0; index < list_ptr->atom_count; index++) { // Start at 0 and check every potential energy
            if (list_ptr->atom_ptr[index]->potential_energy == potential_energy) {
                found = index;
                break;
            }
        }
    }

    return found;
}

/* The function atom_list_determine_inside_box checks the x and y values of our atom to see if it's within the values
 * provided.
 *
 * Input:
 *  struct atom_t *atom_ptr: A pointer to one specific atom
 *  float x_min: Lower bound for position check.
 *  float x_max: Upper bound for position check.
 *  float y_min: Lower bound for position check.
 *  float y_max: Upper bound for position check.
 *
 * Return:
 *  int returnValue:
 *                 0: Outside Box
 *                 1: Inside Box
 *
 * Structures Modified: N/A
 */
int atom_list_determine_inside_box(struct atom_t *atom_ptr, float x_min, float x_max,
                                                            float y_min, float y_max)
{
    int returnValue = 0;

        if (atom_ptr->position[0] >= x_min && atom_ptr->position[0] <= x_max && atom_ptr->position[1] >= y_min &&
                                                                                atom_ptr->position[1] <= y_max) {
            returnValue = 1;
        }
    return returnValue;
}

/* The function atom_list_form_migrate_list looks at the list provided and checks if atoms are either within or outside
 * of our provided coordinates. If the atom is within the specified coordinates then it remains inside the list.
 * If the atom is outside of the coordinates then it is added to a new list and removed from the original list.
 *
 * Input:
 *  struct atom_list_t *list_ptr: Pointer to header block which contains our 'original' list.
 *  float x_min: Lower bound for inner/outer box check.
 *  float x_max: Upper bound for inner/outer box check.
 *  float y_min: Lower bound for inner/outer box check.
 *  float y_max: Upper bound for inner/outer box check.
 *
 * Return:
 *  struct *atom_list_t: A pointer to our newly created list. Even if no atoms could be migrated we still return an
 *                       empty list.
 *
 * Structures Modified:
 *  struct atom_list_t *list_ptr: Atoms could potentially be removed from the list.
 *  struct atom_list_t *my_list: List is initialized and atoms could be added to the list.
 */
struct atom_list_t *atom_list_form_migrate_list(struct atom_list_t *list_ptr, float x_min, float x_max,
                                                                              float y_min, float y_max)
{
    int insideBoxCheck = 0;
    struct atom_list_t *my_list = atom_list_construct(list_ptr->atom_size); // Create new list (allocate memory)

    for (int index = 0; index < list_ptr->atom_count; index++) { //Determine if each atom is either in or outside of box
        insideBoxCheck = atom_list_determine_inside_box(list_ptr->atom_ptr[index], x_min, x_max, y_min, y_max);
        if (insideBoxCheck == 0) { // If outside of box then remove from original list & add to new list
            atom_list_add(my_list, list_ptr->atom_ptr[index]);
            atom_list_remove(list_ptr, index);
        }
        insideBoxCheck = 0; // Reset
    }
    return my_list;
}