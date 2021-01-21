/* lab1.c
 * Prof. Jon Calhoun 
 * jonccal                              <--- replace with your user name!
 * ECE 2230 Spring 2021
 * MP1
 *
 *
 * Purpose: Driver for MP1 
 *
 * Assumptions: The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * The program accepts one command line arguement that is the size of the list.
 *
 *
 * Bugs: None that I know.
 *
 * See the ECE 2230 programming guide
 *
 * NOTE: One of the requirements is to verify you program does not have any 
 * memory leaks or other errors that can be detected by valgrind.  Run with
 * your test script(s):
 *      valgrind --leak-check=full ./lab1 5 < your_test_script
 *  and replace "5" with the value that makes sense for your test script
 * 
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "atom_list.h"

#define MAXLINE 256
#define FALSE 0
#define TRUE 1

// function prototypes for lab1.c
void fill_atom_record(struct atom_t *new, int atom_id);
void print_atom_rec_short(struct atom_t *rec);
void print_atom_rec_long(struct atom_t *rec);
void get_bounding_box(float* minX, float* maxX, float* minY, float* maxY);

int main(int argc, char *argv[])
{
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int list_size = -1;
    int num_items = 0;
    float arg1 = 0;
    //float potential_energy = 0;
    int atom_id = 0;
    int i = 0;
    //int found = FALSE;
    int seed = 2230;
    struct atom_t *rec_ptr = NULL;

    assert(NDIM == 2 && "NDIM must be 2");
    if (argc != 2) {
        printf("Usage: ./lab1 list_size\n");
        exit(1);
    }
    list_size = atoi(argv[1]);
    if (list_size < 1) {
        printf("Invalid list size %d\n", list_size);
        exit(2);
    } 
    printf("The list size is %d. Possible commands:\n", list_size);
    //printf("INSERT\nFIND potential_energy\nREMOVE potential_energy\nUPDATE dt\nMIGRATE\nENERGY\nPRINT\nSTATS\nQUIT\n");
    printf("INSERT\nFIND potential_energy\nREMOVE potential_energy\nUPDATE dt\nMIGRATE\nPRINT\nSTATS\nQUIT\n");

    struct atom_list_t *my_list = atom_list_construct(list_size);

    // seed the RNG to yield deterministic results from drand48()
    srand48(seed);
    
    // remember fgets includes newline \n unless line too long 
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s %f %s", command, &arg1, junk);
        

        if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            printf("Goodbye\n");

            /* remove list from back to front to advoid shifting */
            int num_in_list = atom_list_number_entries(my_list);
            for (i = num_in_list-1; i>=0; i--) {
                rec_ptr = atom_list_remove(my_list, i);
                free(rec_ptr);
            }
            
            atom_list_destruct(my_list);
            break;
        }


        
        else if (num_items == 1 && strcmp(command, "INSERT") == 0) {
            rec_ptr = NULL;   
            rec_ptr = (struct atom_t *) malloc(sizeof(struct atom_t));
            fill_atom_record(rec_ptr, atom_id);
            atom_id++;

            int added_return = -2;
            added_return = atom_list_add(my_list, rec_ptr);

            if (added_return == 1) {
                printf("Inserted: %d\n", rec_ptr->atomic_num);
            } 
            else if (added_return == -1) {
                printf("Rejected: %d\n", rec_ptr->atomic_num);
                free(rec_ptr);
            } else {
                printf("Error with return value! Fix your code.\n");
            }
            rec_ptr = NULL;
        }


        else if (num_items == 2 && strcmp(command, "FIND") == 0) {
            int index = -1;
            rec_ptr = NULL;  

            //arg1 is potential_energy
            index = atom_list_lookup_max_potential_energy(my_list, arg1);
            rec_ptr = atom_list_access(my_list, index);

            if (rec_ptr == NULL) {
                printf("Did not find: %f\n", arg1);
            } else {
                printf("Found atom with potential energy %f at index %d\n", arg1, index);
                print_atom_rec_long(rec_ptr);
            }
            rec_ptr = NULL;
        }

        
        else if (num_items == 2 && strcmp(command, "UPDATE") == 0) {
            //arg1 is dt
            atom_list_compute_forces(my_list);
            atom_list_advance_momenta(my_list, arg1);
            atom_list_advance_position(my_list, arg1);
        }


        else if (num_items == 1 && strcmp(command, "MIGRATE") == 0) {
            struct atom_list_t * migrate_ptr = NULL;
            float minX, maxX, minY, maxY = 0.0;

            get_bounding_box(&minX, &maxX, &minY, &maxY);
            migrate_ptr = atom_list_form_migrate_list(my_list, minX, maxX, minY, maxY);

            if (migrate_ptr == NULL) {
                printf("Did not find atoms to migrate in : %e %e %e %e\n", minX, maxX, minY, maxY);
            } else {
                /* print items in structure */
                printf("Found atoms to migrate:\n");
            
                int num_in_list = atom_list_number_entries(migrate_ptr);
                for (i=0; i < num_in_list; i++) {
                     rec_ptr = atom_list_access(migrate_ptr, i);
                    if (rec_ptr != NULL) {
                        print_atom_rec_long(rec_ptr);
                    }
                    else {
                        printf("Error in migrate list: NULL value\n");
                        break;
                    }
                }
            
                // remove list from back to front to advoid shifting
                for (i = num_in_list-1; i>=0; i--) {
                    rec_ptr = atom_list_remove(migrate_ptr, i);
                    free(rec_ptr);
                }
                atom_list_destruct(migrate_ptr);
                rec_ptr = NULL;
            }
        }


        else if (num_items == 2 && strcmp(command, "REMOVE") == 0) {
            int index = -1;
            rec_ptr = NULL;   

            // arg1 is
            index = atom_list_lookup_max_potential_energy(my_list, arg1);
 
            if (index == -1) {
                printf("Did not remove atom with potential energy: %f\n", arg1);
            } else {
                rec_ptr = atom_list_remove(my_list, index);
                printf("Removed: %f\n", arg1);
                //print_atom_rec_short(rec_ptr);
                print_atom_rec_long(rec_ptr);
                free(rec_ptr);
            }
            rec_ptr = NULL;
        }
        
        /*
         else if (num_items == 1 && strcmp(command, "ENERGY") == 0) {
            float energy = atom_list_compute_kinetic_energy(my_list);
            printf("Kinetic Energy: %f\n", energy);
        }
        */

        
        else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            // get the number in list and order of the list
            int num_in_list = 0;   
            num_in_list = atom_list_number_entries(my_list);
            printf("Number records: %d\n", num_in_list);
        }

        
        else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            int num_in_list = 0;   
            num_in_list = atom_list_number_entries(my_list);
            
            if (num_in_list == 0) {
                printf("List empty\n");
            } else {
                printf("List has %d records\n", num_in_list);
                for (i = 0; i < num_in_list; i++) {
                    printf("%4d: ", i);
                    rec_ptr = NULL;    
                    rec_ptr = atom_list_access(my_list, i);
                    print_atom_rec_short(rec_ptr);
                }
            }
            rec_ptr = NULL;
        } else {
            printf("UNKNOWN COMMAND: %s", line);
        }
    }
    return 0;
}

/* If a string ends with an end-of-line \n, remove it.
 */
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
void fill_atom_record(struct atom_t *new, int atom_id)
{
    char line[MAXLINE];
    assert(new != NULL);

    new->atom_id = atom_id;

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
void get_bounding_box(float* minX, float* maxX, float* minY, float* maxY) {

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
void print_atom_rec_short(struct atom_t *rec)
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
void print_atom_rec_long(struct atom_t *rec)
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
