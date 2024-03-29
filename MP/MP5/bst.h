/* bst.h 
 * Aaron Bruner
 * ajbrune@g.clemson.edu
 * ECE 2230 Spring 2021
 * MP 5
 *
 * Purpose: Provides structure for interface and tree definition for basic binary tree.
 *
 * Assumptions: All pointers entered into the bst_node_t structure are either valid or NULL. Also, the structure needs
 *              to be full when it's allocated. Meaning, all elements of the structure need to have a value, even if
 *              it's 0 or NULL.
 *
 * Bugs: N/A
 */

enum balanceoptions {BST, AVL};

#define TRUE 1
#define FALSE 0

typedef void *data_t;
typedef int bst_key_t;

typedef struct bst_node_tag {
    data_t data_ptr;
    bst_key_t key;
    int height;
    struct bst_node_tag *left;
    struct bst_node_tag *right;
} bst_node_t;

typedef struct bst_tag {
    bst_node_t *root;
    int size;       // number of keys in tree
    int num_recent_rotations;  // number of rotations in last operation
    int policy;     // must be BST or AVL
    int num_recent_key_comparisons; // number of comparisons in last operation
} bst_t;


/* prototype definitions for functions in bst.c */
data_t bst_access(bst_t *bst_ptr, bst_key_t key);
bst_t *bst_construct(int);
void bst_destruct(bst_t *bst_ptr);
int bst_insert(bst_t *bst_ptr, bst_key_t key, data_t elem_ptr);
int bst_avl_insert(bst_t *bst_ptr, bst_key_t key, data_t elem_ptr);
data_t bst_remove(bst_t *bst_ptr, bst_key_t key);

int bst_size(bst_t *bst_ptr);
int bst_key_comps(bst_t *bst_ptr);
int bst_int_path_len(bst_t *);
int bst_rotations(bst_t *bst_ptr);
void bst_debug_print_tree(bst_t *bst_ptr);
void bst_debug_validate(bst_t *T);


/* vi:set ts=8 sts=4 sw=4: */
