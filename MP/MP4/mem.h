/* mem.h 
 * Prof. Calhoun 
 * jonccal 
 * ECE 2230 Spring 2021
 * MP4
 *
 * Propose: Defines the interface for our version of malloc/free. Also provides
 *               profiling and debugging infomation about the heap
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignment.  
 *
 * Bugs: None known.
 */

/* Constants */
#define PAGESIZE 4096  /* size of a page in bytes */
#define TRUE 1         /* logical true            */
#define FALSE 0        /* logical false           */

/* Search policies */
#define FIRST_FIT  0xA
#define BEST_FIT   0xB
#define WORST_FIT  0xC

/* Search start locations */
#define HEAD_FIRST 0xD
#define ROVER_FIRST 0xE

/* Runtime options configuring memory allocation in mem.c */
extern int SearchPolicy;  /* FIRST_FIT or BEST_FIT or WORST_FIT      */
extern int SearchLoc;     /* HEAD_FIRST or ROVER_FIRST               */
extern int Coalescing;    /* TRUE/FALSE to enable/disable coalescing */

/* prototypes for functions defined in mem.c */
void Mem_free(void *return_ptr);
void *Mem_alloc(int nbytes);
void Mem_stats(void);
void Mem_print(void);

#define SIZEOF_CHUNK_T 16  /* for debugging and test drivers */


/* vi:set ts=8 sts=4 sw=4 et: */
