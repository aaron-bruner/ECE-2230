/* lab3.c
 * Prof. Jon Calhoun                  
 * jonccal                              
 * ECE 2230 Spring 2021
 * MP3
 *
 * 
 *
 * Propose: A driver for MP3. DO NOT MODIFY!
 *
 * Assumptions: This file is simple.  Input is collected and the
 * appropriate function is called.
 *
 *
 *
 * See the ECE 223 programming guide
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "atom_support.h"

int main(int argc, char * argv[])
{
    list_t * mp3_list;
    list_t * sorted_list;
    list_t * unsorted_list;
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int list_size = -1;
    int num_items;
    float arg1;
    int arg2;

    if (argc != 2) {
        printf("Usage: ./lab3 list_size\n");
        exit(1);
    }
    
    //list_size = 100000;
    list_size = atoi(argv[1]);
    if (list_size < 1) {
        printf("Invalid list size %d\n", list_size);
        exit(2);
    } 

    printf("Lab3 list size is %d. Possible commands:\n", list_size);
    printf("MP3: ADDTAIL atomID\nSORT sort# direction\nPRINTMP3\n");
    printf("List: INSERT\nFIND potential_energy\nREMOVE potential_energy\nUPDATE dt\nMIGRATE\nREVERSE\nPRINT\n");
    printf("List: INSERT\nFIND atomic#\nREMOVE atomic#\nPRINT\nSTATS\nMIGRATE\nREVERSE\nQUIT\n");

    printf("Queue      : ADDTAIL; RMHEAD; PRINTHEAD; PRINTQ\n");
    printf("           : STATS; QUIT\n\n\n");

    // Create three List objects
    mp3_list = atom_list_create();
    
    /* this list is sorted */
    sorted_list = atom_list_create();

    /* this list is unsorted and has no duplicates */
    unsorted_list = atom_list_create();

    /* remember fgets includes newline \n unless line too long */
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s %f %d %s", command, &arg1, &arg2, junk);
        if (num_items == 1 && strcmp(command, "INSERT") == 0) {
            atom_list_add(sorted_list, list_size);
        } else if (num_items == 2 && strcmp(command, "FIND") == 0) {
            atom_list_lookup_potential_energy(sorted_list, arg1);
        } else if (num_items == 2 && strcmp(command, "REMOVE") == 0) {
            atom_list_remove_potential_energy(sorted_list, arg1);
        } 
        // MP1 ops
        else if (num_items == 2 && strcmp(command, "UPDATE") == 0) {
            atom_list_update(sorted_list, arg1); 
        } else if (num_items == 1 && strcmp(command, "MIGRATE") == 0) {
            atom_list_migrate(sorted_list); 
        }
        
        //new in MP2
        else if (num_items == 1 && strcmp(command, "REVERSE") ==0) {
            atom_list_reverse(sorted_list);
        } else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            atom_list_print(sorted_list, "List");
        }
        // unsorted list commands
        else if (num_items == 1 && strcmp(command, "ADDTAIL") == 0) {
            atom_list_add_tail(unsorted_list);
        } else if (num_items == 1 && strcmp(command, "RMHEAD") == 0) {
            atom_list_remove_head(unsorted_list);
        } else if (num_items == 1 && strcmp(command, "PRINTHEAD") == 0) {
            atom_list_print_head(unsorted_list);
        } else if (num_items == 1 && strcmp(command, "PRINTQ") == 0) {
            atom_list_print(unsorted_list, "Queue");
        }
        
        
        // MP3 functions
        else if (num_items == 2 && strcmp(command, "ADDTAIL") == 0) {
            atom_list_add_tail_mp3(mp3_list, arg1);
        } else if (num_items == 3 && strcmp(command, "SORT") == 0) {
            atom_list_sort(&mp3_list, arg1, arg2);
        } else if (num_items == 1 && strcmp(command, "PRINTMP3") == 0) {
            atom_list_print(mp3_list, "List");
        } else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            atom_list_print(mp3_list, "List");
            atom_list_stats(sorted_list, unsorted_list, mp3_list);
        }
        
        
        
        else if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            atom_list_cleanup(sorted_list);
            atom_list_cleanup(unsorted_list);
            atom_list_cleanup(mp3_list);
            printf("Goodbye\n");
            break;
        } else {
            printf("UNKNOWN COMMAND: %s", line);
        }
    }
    return 0;
}
/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
