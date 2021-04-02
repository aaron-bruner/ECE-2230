/* mem.c
 * Aaron Bruner
 * ajbrune
 * ECE 2230 Spring 2021
 * MP4
 *
 * Propose: The purpose of MP4 is for us, the students, to rewrite our standard memory management functions. We have
 *          always relied on the OS to manage our memory. This MP will allow us to better understand the heap and
 *          issues related to dynamic memory management. We will be rewriting the malloc function and the free function.
 *          We will be using a free list to manage free blocks of memory. The free list will provide memory regions to
 *          satisfy memory allocation calls.
 *
 * Assumptions: We make the assumption that the user who is running our application is knowledgeable as to how the
 *              program operates and what command line inputs are considered valid. We also must assume that the OS
 *              has enough resources for whatever the user requests. Also we must assume that the user provides
 *              valid input that will not deliberately break the lab4.c file.
 *
 * Bugs: None.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "mem.h"

/* chunk_t definition (do not modify).
 * sizeof(chunk_t) must equal SIZEOF_CHUNK_T in mem.h.
 */
typedef struct chunk_tag {
    struct chunk_tag *next;  /* pointer to the next free block */
    long size;               /* size of memory block in units of
                                sizeof(chunk_t) */
} chunk_t;

typedef struct heap_stats {
    int numItems;    /* number of chunks in the free list    */
    int min;         /* size of the smallest chunk, in bytes */
    int max;         /* size of the largest chunk, in bytes  */
    int average;     /* average size of the chunks, in bytes */
    int totalBytes;  /* total size of all chunks, in bytes   */

    /* the following two fields are updated in morecore() */
    int numSbrkCalls;  /* number of successful calls to sbrk()  */
    int numPages;      /* number of pages allocated with sbrk() */
} heap_stats_t;

/* global variables exported via mem.h */
int SearchPolicy = FIRST_FIT;
int SearchLoc = HEAD_FIRST;
int Coalescing = FALSE;

/* global variables restricted to mem.c only */
static chunk_t Dummy = {
    &Dummy,  /* next */
    0        /* size */
};
static chunk_t * Rover = &Dummy;
static heap_stats_t stats;  /* initialized by the O/S to all 0s */


/* prototypes for functions private to mem.c */
void mem_validate(void);
chunk_t *morecore(int);
void findMem (chunk_t * start, chunk_t * pos_ptr, long rmv_size, const long size);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 */
chunk_t *morecore(int new_bytes) 
{
    char *cp;
    chunk_t *new_p;

    /* assert preconditions */
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(chunk_t) == 0);

    /* allocate memory using sbrk() */
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (chunk_t *) cp;
    
    /* update statistics */
    stats.numSbrkCalls++;
    stats.numPages += new_bytes/PAGESIZE;

    return new_p;
}

/* The function Mem_free will deallocate the space pointed to by return_ptr.
 *
 * Input:
 *  void * return_ptr: A pointer to an already allocated memory block. This memory is being passed in
 *                     to be free'd and put in the free memory list.
 *
 * Return: N/A
 *
 * Structures Modified: N/A
 *
 * Assuming Coalescing is TRUE then we will coalesce the free memory into already free memory.
 */
void Mem_free(void *return_ptr) {
    assert(Rover != NULL && Rover->next != NULL); // Ensure Rover and Rover->next are valid

    chunk_t *free_mem = (chunk_t *) return_ptr;
    free_mem--;
    chunk_t *temp = NULL;

    if (Coalescing && Rover->next != Rover) {
        Rover = &Dummy;
        do { // Go to node before head
            temp = Rover;
            Rover = Rover->next;
        } while (Rover != &Dummy && (free_mem < temp || free_mem > Rover));

        int after = FALSE;  // after free_ptr
        int before = FALSE; // before free_ptr

        Rover == (free_mem + free_mem->size) ? (after = TRUE) : (after = FALSE);
        free_mem == (temp + temp->size) ? (before = TRUE) : (before = FALSE);

        if (after == TRUE && before == TRUE) { // Coalesce logic
            temp->size += free_mem->size + Rover->size;
            Rover = Rover->next;
            temp->next = Rover;
        } else if (after == TRUE && before == FALSE) {
            temp->next = free_mem;
            free_mem->size += Rover->size;
            Rover = Rover->next;
            free_mem->next = Rover;
        } else if (after == FALSE && before == TRUE) {
            temp->size += free_mem->size;
        } else {
            free_mem->next = temp->next;
            temp->next = free_mem;
            Rover = Rover->next;
        }
    }
    else {
        free_mem->next = Rover->next;
        Rover->next = free_mem;
    }
}

/* The function Mem_alloc will allocate the space of size nbytes and return a pointer to that memory. Since we are
 * handling our own free memory instead of letting the OS get the memory for us we will collect either the FIRST_FIT,
 * BEST_FIT or WORST_FIT. This means we will either get the first available block of memory of any size that fits for
 * FIRST_FIT. For BEST_FIT we will look over all of our free memory and find the smallest amount of memory to fit (best).
 * Lastly, for WORST_FIT we will look for the largest amount of memory to fulfill our request. In all cases, if we don't
 * have enough memory we call morecore which asks the operating system nicely for more memory using magic.
 *
 * Inputs:
 *  int nbytes: An integer value containing the number of bytes we need in memory.
 *
 *  Return: A pointer to the memory block that we allocated for the user (in size nbytes).
 *
 *  Structures Modified:
 *      Our free memory pool has shrunk and the occupied memory has grown.
 *
 * A great guide on how to write this function can be found here:
 * https://danluu.com/malloc-tutorial/
 * The guide above was based on the following tutorial which also seems pretty good:
 * https://web.archive.org/web/20171014020821/http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf
*/
void *Mem_alloc(int nbytes) {
    assert(nbytes > 0); // Ensure nbytes is valid

    long size = (nbytes/SIZEOF_CHUNK_T) + 1; // # of bytes needed
    0 == nbytes % SIZEOF_CHUNK_T ? size : size++;
    //size = nbytes%SIZEOF_CHUNK_T==0 ? size : size+1;

    chunk_t *start;
    chunk_t *block = NULL;

    switch(SearchLoc) {
        case HEAD_FIRST:
            start = Rover = &Dummy;
            break;
        case ROVER_FIRST:
            start = Rover;
            break;
        default:
            printf("\n\nERROR: Invalid input for [SearchLoc]...\n\n");
            exit(5); // This shouldn't happen
    }

    long removal_size = Rover->size;

    findMem(start, block, removal_size, size-1);

    if (block == NULL) { // Unable to find available memory
        int page = nbytes;
        chunk_t *temp = Rover->next;
        // Increase removal_size to be a multiple of PAGESIZE
        if (page%PAGESIZE != 0) page += PAGESIZE - (page % PAGESIZE); // We can only have multiples of PAGESIZE
        block = morecore(page); if(block == NULL) exit(5); // "malloc" NULL check
        block->size = removal_size = (page/SIZEOF_CHUNK_T) - 1;
        Rover->next = block;
        block->next = temp;
        block->size = removal_size;
        temp = NULL;
    }

    while (Rover->next != block) { Rover = Rover->next; } // Move back to start
    if (removal_size > size-1) { // Cleanup header block
        Rover->next = block + size;
        Rover->next->next = block->next;
        Rover->next->size = removal_size - size;
        block->size = size-1;
    } else { Rover->next = block->next; } // No free space

    return (block + 1); // Point to next free block
}

/* The function findMem was designed to determine if we have a memory block that is available based on the user's
 * criteria.
 *
 * Inputs:
 *  chunk_t * start: A constant pointer to the start of our list
 *  chunk_t * pos_ptr: A rover style pointer for us to manipulate and move around (our return value too)
 *  long rmv_size: The size of memory we need to allocate
 *  const long size: The size of our free memory
 *
 *  Return:
 *      Even though the return value is void we will be looking at the pos_ptr in the caller function and using the final
 *      value in logical checks. If the value is NULL then we were not able to find a memory block big enough, otherwise
 *      we were.
 *
 * Structures Modified: N/A
 */
void findMem (chunk_t * start, chunk_t * pos_ptr, long rmv_size, const long size) {
    switch(SearchPolicy) { // We need to determine if we have a big enough block for the specified search policy

        case FIRST_FIT:
            do {
                if (Rover->size >= size) { // Move through each block and find the first one big enough
                    pos_ptr = Rover; rmv_size = Rover->size; } // If found set position and size
                Rover = Rover->next; // Keep moving if not right size
            } while (pos_ptr == NULL && Rover != start); // while not found and we haven't come back to the start
            break;

        case BEST_FIT:
            pos_ptr = Rover;
            do {
                if (Rover->size >= size) {
                    if (rmv_size < size || Rover->size < rmv_size) { // within bounds
                        pos_ptr = Rover; rmv_size = Rover->size; }   // found A fit, may not be the best
                }
                Rover = Rover->next;
            } while (rmv_size != size && Rover != start); // keep going until we either find the exact size or end of list
            if (rmv_size < size) pos_ptr = NULL; // if we can't find the size at all then we set ptr to NULL
            break;

        case WORST_FIT:
            pos_ptr = Rover;
            long max_size = rmv_size;
            do {
                // If a bigger chunk is found, save that size and position
                if (Rover->size > max_size) { // Keep looking for a bigger block
                    pos_ptr = Rover; rmv_size = Rover->size; } // found A bigger block, bigger ones may exist
                Rover = Rover->next; // keep moving
            } while (Rover != start);
            if (rmv_size < size) pos_ptr = NULL; // if we can't find the size at all then we set ptr to NULL
            break;

        default:
            printf("\n\nERROR: Invalid input for [SearchPolicy]...\n");
            exit(5);
    }
    return;
}

/* The function Mem_stats prints stats about the current free list.
 *
 * Inputs: N/A
 *
 * Returns: N/A
 *
 * Structures Modified: None
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void) {
    chunk_t *ptr = Dummy.next;
    int num = 1, min = ptr->size, max = ptr->size, tot = 0, flag = FALSE;

    if (ptr == &Dummy) {num = 1; flag = TRUE;}

    while (ptr != &Dummy && flag == FALSE) {
        num++;
        min = ptr->size < min ? ptr->size : min;
        max = ptr->size > max ? ptr->size : max;
        tot += (ptr->size + 1);
        ptr = ptr->next;
    }

    stats.numItems = num;
    stats.min = min * SIZEOF_CHUNK_T;
    stats.max = max * SIZEOF_CHUNK_T;
    stats.average = (tot * SIZEOF_CHUNK_T) / stats.numItems;
    stats.totalBytes = tot * SIZEOF_CHUNK_T;

    /* ======= DO NOT MODIFY FROM HERE TO END OF Mem_stats() ======= */
    printf("\n\t\tMP4 Heap Memory Statistics\n"
            "----------------------------------------------------\n\n"
            "\tNumber of blocks in free list: %d\n"
            "\tMin: %d\n"
            "\tMax: %d\n"
            "\tAverage: %d\n"
            "\tTotal bytes in free list: %d\n"
            "\tNumber of sbrk calls: %d\n"
            "\tNumber of requested pages: %d\n"
            "\tHeap status:  %s\n",
            stats.numItems, stats.min, stats.max, stats.average, stats.totalBytes,
            stats.numSbrkCalls, stats.numPages,
            stats.totalBytes == stats.numPages*PAGESIZE ? \
            "all memory is in the heap -- no leaks are possible\n"\
            : "heap is in-use -- leaks are possible\n");
}

/* The function Mem_print prints a table of memory in free list.
 *
 * Inputs: N/A
 *
 * Returns: N/A
 *
 * Structures Modified: N/A
 *
 * The print should include the dummy item in the list 
 */
void Mem_print(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    chunk_t *p = Rover;
    chunk_t *start = p; /* save my starting block */
    do {
        printf("p=%p, size=%ld, end=%p, next=%p %s\n", 
                p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start); /* only 1 time through the list */
    //mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    chunk_t *p, *largest, *smallest;

    /* for validate begin at Dummy */
    p = &Dummy;
    do {
        if (p->size == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &Dummy);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                /* this is not good unless at the one wrap */
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   /* stop and use gdb */
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size < p->next);
            }
            p = p->next;
        } while (p != &Dummy);
        assert(wrapped == TRUE);
    }
}

/* vi:set ts=8 sts=4 sw=4 et: */