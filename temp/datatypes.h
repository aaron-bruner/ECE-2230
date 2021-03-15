/* datatypes.h 
 * Casey Hird
 * crhird
 * ECE 2230 Spring 2019
 * MP3
 *
 * Propose: The data type that is stored in the list ADT is defined here.  We
 *          define a single mapping that allows the list ADT to be defined in
 *          terms of a generic data_t.
 *
 * Assumptions: 
 *
 *     data_t: The type of data that we want to store in the list
 *
 * Bugs:
 * 
 */

enum state {QUEUED, RUNNING, BLOCKED, FINISHED};

typedef struct task_info {
    int task_id;            // unique task id
    int priority;           // scheduling priority
    enum state state;       // scheduling state 
    
    
    double  wallclocktime;  // task runtime in seconds
   
     
    int nargs;              // number of input arguments
    int* args;              // input arguments
    int output;             // task result
} task_t;

/* the list ADT works on task data of this type */
typedef task_t data_t;


/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
