/* bst.c
 * Aaron Bruner
 * ajbrune@clemson.edu
 * ECE 2230 Spring 2021
 * MP 5
 *
 * Purpose: Implementation of tree interface for basic binary tree
 *
 * Assumptions: We make the assumption that the user who is running our application is knowledgeable as to how the
 *              program operates and what command line inputs are considered valid. Also we must assume that the
 *              user provides valid input that will not deliberately break the lab5.c file.
 *
 * Bugs: There is an issue with adding multiple nodes with the same key which results in a segmentation fault.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "bst.h"

#define MYMAX(a, b) (a > b ? a : b)
// counters for statistics
int CompCalls = 0;
int NumRotations = 0;


/* definitions for use in bst.c only */
void ugly_print(bst_node_t *N, int level, int policy);
int bst_debug_validate_rec(bst_node_t *N, int min, int max, int *count);
int rec_height(bst_node_t *N);
int children(bst_node_t *N);
void pretty_print(bst_t *T);
// My helper functions
void recursive_bst_destruct(bst_node_t *N);
int recursive_bst_int_path_len(bst_node_t *N, int level);
bst_node_t* bst_insert_node(bst_node_t *N, bst_key_t key, data_t elem_ptr, int policy);
int getBalance(bst_node_t * node);
int getHeight(bst_node_t * node);
bst_node_t* rightRotate(bst_node_t* N);
bst_node_t* leftRotate(bst_node_t* N);
void heightUpdate(bst_node_t * x, bst_node_t * y);


/* The function bst_access was designed to be able to find the tree element that matches key and return the data that is
 *     stored in this node in the tree.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to search.
 *  bst_key_t key: The key to find inside of T
 *
 *  Return:
 *      A pointer to the data stored in the found node or NULL if no match is found.
 *
 * Structures Modified: N/A
 */
data_t bst_access(bst_t *T, bst_key_t key)
{
    CompCalls = 0;
    bst_node_t *N = T->root; // Start our search at the root

    while (N != NULL && N->key != key) { // Search until the final child's leaf is NULL
        //if (key == node->key) return node->data_ptr; // Found the node with the matching key
        N = key > N->key ? N->right : N->left; // Check if we need to go left or right
        CompCalls += 2;
        T->num_recent_key_comparisons = CompCalls;
    }
    return N == NULL ? NULL : N->data_ptr;
}

/* The function bst_construct was designed to create the header block for the tree with the provided management policy,
 * and initialize it with default 'blank' data.
 *
 * Inputs:
 *  int tree_policy: Tree management policy to use either AVL or BST.
 *
 *  Return:
 *      A pointer to the newly created tree.
 *
 * Structures Modified:
 *  bst_t * T: This newly created structure will have all of our information initialized to either 0 or NULL; not
 *             including the tree_policy that the user requested.
 */
bst_t *bst_construct(int tree_policy)
{
    if (tree_policy != AVL && tree_policy != BST) { // This should never happen
        printf("\n\n[ERROR] tree_policy passed into bst_construct is not valid. Must be AVL or BST...\n\n");
        exit(5);
    }
    bst_t * T = malloc(sizeof(bst_t)); // Allocate memory for the head
    if (T == NULL){ // This should never happen
        printf("\n\n[ERROR] Malloc Failed in bst_construct\n\n");
        exit(5);
    }
    T->root = NULL; // Set integers to 0 and pointers to NULL
    T->size = 0;
    T->num_recent_rotations = 0;
    T->policy = tree_policy;
    T->num_recent_key_comparisons = 0;

    return T; // Pointer to new tree
}

/* The function bst_destruct was designed to free all items stored in the tree including the memory block with the data
 * and the bst_node_t structure. Also frees the header block.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to destroy.
 *
 *  Return: N/A
 *
 * Structures Modified:
 *  bst_t * T: All information in the structure will be removed and the memory will be free'd.
 *
 *  This function uses the recursive_bst_destruct which makes removing the nodes more simple.
 *
 *  We could have used an iterative method but then we would have had to traverse the tree to determine the number
 *  of children and then traverse again to free them. This way we're only having to traverse once.
 */
void bst_destruct(bst_t *T)
{
    recursive_bst_destruct(T->root);
    free(T);
    T = NULL;
}

/* The function recursive_bst_destruct was designed to free all items stored in the tree and their data.
 *
 * Inputs:
 *  bst_node_t * N: A pointer to a node in which we're wanting to remove.
 *
 *  Return: N/A
 *
 * Structures Modified:
 *  bst_node_t * N: All information in the structure will be removed and the memory will be free'd.
 */
void recursive_bst_destruct(bst_node_t * N) {
    if (N != NULL) {
        recursive_bst_destruct(N->left); // Keep moving down the left subtree until we get to the lowest child
        recursive_bst_destruct(N->right); // Keep moving down the right subtree until we get to the lowest child
        free(N->data_ptr);
        free(N);
    }
}

/* The function bst_insert was designed to insert data_t into the tree with the associated key. Insertion MUST follow
 * the tree's property (i.e., AVL or BST)
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to insert our new node into.
 *  bst_key_t key: Search key to determine if key is in the tree.
 *  data_t elem_ptr - Data to be stored at tree node indicated by key
 *
 *  Return:
 *   0 if key is found and element is replaced
 *   1 if key is not found and element is inserted
 *
 * Structures Modified:
 *  bst_t * T: We will be inserting the elem_ptr into our list T.
 */
int bst_insert(bst_t *T, bst_key_t key, data_t elem_ptr)
{
    assert(elem_ptr != NULL);
    CompCalls = 0;
    NumRotations = 0;
    bst_node_t *N = T->root;
    int check = -1;
    while(N != NULL && check == -1) {
        CompCalls++;
        if (key == N->key) {
            free(N->data_ptr);
            N->data_ptr = elem_ptr;
            check = 0;
        } else {
            CompCalls++;
            if (key < N->key) N = N->left;
            else N = N->right;
        }
    }
    if (check == -1) {
        if (T->policy == AVL) {
            check = bst_avl_insert(T, key, elem_ptr);
        } else if (T->policy == BST) {
            T->root = bst_insert_node(T->root, key, elem_ptr, T->policy);
        }
        check = 1;
        T->size++;
    }

    assert (check != -1);
    T->num_recent_rotations = NumRotations;

#ifdef VALIDATE
    bst_debug_validate(T);
#endif

    return check;
}

/* The function bst_avl_insert was designed to be the gateway function for AVL insertions. We will be inserting data_t
 * into the tree with the associated key. Insertion MUST follow the tree's property AVL. This function should be called
 * from bst_insert for AVL tree's inserts.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to add our data to.
 *  bst_key_t key: The search key for particular node in the tree 'T'
 *  data_t elem_ptr: A pointer to the data to be stored at tree node indicated by key
 *
 *  Return:
 *   0 if key is found and the element is replaced
 *   1 if key is not found and element is inserted
 *
 * Structures Modified:
 *  bst_t * T: A node that matches key is being inserted to the tree.
 */
int bst_avl_insert(bst_t *T, bst_key_t key, data_t elem_ptr)
{
    T->root = bst_insert_node(T->root, key, elem_ptr, T->policy);
    return 1;
}

/* The function bst_insert_node was designed to insert an item in a specific tree. If the tree already has a node with
 * a matching key we will replace it.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to remove the specified key from.
 *  bst_key_t key: The search key for particular node in the tree 'T'
 *
 *  Return:
 *      The pointer to the data memory block and free the bst_node_t memory block.  If the key is not found in the
 *      tree, return NULL. If the tree's policy is AVL, then ensure all nodes have the AVL property.
 *
 * Structures Modified:
 *  bst_t * T: A node that matches key is being removed from the tree, assuming it exists.
 *
 *  The algorithm is based on the implementation here: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */
bst_node_t *bst_insert_node(bst_node_t *N, bst_key_t key, data_t elem_ptr, int policy) {
    // BST insertion
    if ( N == NULL ) {
        bst_node_t *NewN = malloc(sizeof(bst_node_t)); // Create a new node if one does not exist
        NewN->data_ptr = elem_ptr;
        NewN->key = key;
        NewN->height = 0;
        NewN->left = NULL;
        NewN->right = NULL;
        return NewN; // Return pointer to new node since we didn't have to put it in the right place.
    }
    else if (key > N->key)
        N->right = bst_insert_node(N->right, key, elem_ptr, policy);
    else
        N->left = bst_insert_node(N->left, key, elem_ptr, policy); // We cannot have equal keys in BST

    N->height = 1 + MYMAX(getHeight(N->left), getHeight(N->right));

    if (policy == AVL) {
        int balance = getBalance(N);
        if (balance > 1 && key < N->left->key) return rightRotate(N); // Left-Left
        if (balance < -1 && key > N->right->key) return leftRotate(N); // Right-Right
        if (balance > 1 && key > N->left->key) {// Left-Right
            N->left = leftRotate(N->left);
            return rightRotate(N);
        }
        if (balance < -1 && key < N->right->key) { // Right-Left
            N->right = rightRotate(N->right);
            return leftRotate(N);
        }
    }
    return N;
}

/* The function getBalance was designed to determine the balance of the specific node we're looking at.
 *
 * Inputs:
 *  bst_node_t * N: The node in which we want to check both the left and right children
 *
 *  Return:
 *      An integer value which is the difference between the height of the right subtree and left subtree.
 *
 * Structures Modified: N/A
 *
 *  The algorithm is based on the implementation here: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */
int getBalance(bst_node_t * N) {
    if ( N == NULL ) return 0;
    return (getHeight(N->left) - getHeight(N->right));
}

/* The function getHeight was designed to determine the height at the specific node. We already store
 * this value in N->height so all we need to do is return it.
 *
 * Inputs:
 *  bst_node_t * N: The node in which we want to return the height
 *
 *  Return:
 *      An integer value which is the height at that node
 *
 * Structures Modified: N/A
 *
 *  The algorithm is based on the implementation here: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */
int getHeight(bst_node_t * N) {
    if ( N == NULL ) return -1;
    return N->height;
}

/* The function rightRotate was designed to right rotate a subtree rooted at y.
 *
 * Inputs:
 *  bst_node_t * y: The node where we're rotating
 *
 *  Return:
 *      A pointer to the new root node after we finish rotating
 *
 * Structures Modified:
 *  bst_node_t * A: The function will undergo a right rotation which will rearrange the position of nodes.
 *
 *  The algorithm is based on the implementation here: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */
bst_node_t* rightRotate(bst_node_t* y) {
    NumRotations++;
    // Rotate nodes (B is left child of y, becomes new root)
    bst_node_t *x = y->left;
    bst_node_t *temp = x->right;
    x->right = y;
    y->left = temp;
    heightUpdate(x, y);
    return x;
}
/* The function leftRotate was designed to left rotate a subtree rooted at y.
 *
 * Inputs:
 *  bst_node_t * y: The node where we're rotating
 *
 *  Return:
 *      A pointer to the new root node after we finish rotating
 *
 * Structures Modified:
 *  bst_node_t * A: The function will undergo a left rotation which will rearrange the position of nodes.
 *
 *  The algorithm is based on the implementation here: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */
bst_node_t* leftRotate(bst_node_t* y) {
    NumRotations++;
    // Rotate nodes (x is right child of y, becomes new root)
    bst_node_t *x = y->right;
    bst_node_t *temp = x->left;
    x->left = y;
    y->right = temp;
    heightUpdate(x, y);
    return x;
}

/* The function heightUpdate was designed to ensure the height of our tree was correct during the rotation process.
 *
 * Inputs:
 *  bst_node_t * N: The node we need to update the height of.
 *
 *  Return: N/A
 *
 * Structures Modified:
 *  bst_node_t * N: The height variable in the structure will be updated.
 *
 *  The algorithm is based on the implementation here: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */
void heightUpdate (bst_node_t * x,bst_node_t * y) {
    int left, right;

    left = y->left == NULL ? -1 : y->left->height;
    right = y->right == NULL ? -1 : y->right->height;

    y->height = 1 + MYMAX(left, right);

    left = x->left == NULL ? -1 : x->left->height;
    right = x->right == NULL ? -1 : x->right->height;

    x->height = 1 + MYMAX(left, right);
}

/* The function bst_remove was designed to remove the item in the tree with the matching key.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to remove the specified key from.
 *  bst_key_t key: The search key for particular node in the tree 'T'
 *
 *  Return:
 *      The pointer to the data memory block and free the bst_node_t memory block.  If the key is not found in the
 *      tree, return NULL. If the tree's policy is AVL, then ensure all nodes have the AVL property.
 *
 * Structures Modified:
 *  bst_t * T: A node that matches key is being removed from the tree, assuming it exists.
 */
data_t bst_remove(bst_t *T, bst_key_t key)
{
    data_t dp = NULL;
    CompCalls = 0;
    NumRotations = 0;
    if (T->policy == AVL)
        dp = NULL; /*TODO: AVL remove */
    else
        dp = NULL; /*TODO: BST remove */

    /*TODO: update tree stats*/

#ifdef VALIDATE
    bst_debug_validate(T);
#endif
    return dp;
}

/* The function bst_size was designed to determine the number of keys in the tree.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to check.
 *
 *  Return:
 *      An integer value which is the number of keys in the tree.
 *
 * Structures Modified: N/A
 */
int bst_size(bst_t *T)
{
    if (T == NULL) return 0;

    return T->size;
}

/* The function bst_key_comps was designed to determine the number of key comparisons.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to check.
 *
 *  Return:
 *      An integer value which is the number of key comparisons.
 *
 * Structures Modified: N/A
 */
int bst_key_comps(bst_t *T)
{
    if (T == NULL) return 0;

    return T->num_recent_key_comparisons;
}

/* The function bst_int_path_len was designed to determine the computed internal path length of the tree T.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to check.
 *
 *  Return:
 *      An integer value which is the computed internal path length of the tree T.
 *
 * Structures Modified: N/A
 *
 * Note: This function uses a recursive call to the function recursive_bst_int_path_len which will actually calculate
 *       the internal path length.
 */
int bst_int_path_len(bst_t *T)
{
    if (T == NULL) return 0;

    return recursive_bst_int_path_len(T->root, 0);
}

/* The function recursive_bst_int_path_len was designed to calculate the internal path length of our provided tree.
 *
 * Inputs:
 *  bst_node_t * N: A pointer to the node in which we want to check both the left and right root
 *  int level: An integer value for what level we are currently at. Every time we go down the left or right we increase
 *             the level count.
 *
 *  Return:
 *      The return value is the sum of all of the recursive calls. We are determining the internal length of our tree.
 *
 * Structures Modified: N/A
 *
 * Code was designed based on this algorithm:
 * https://www.geeksforgeeks.org/write-a-c-program-to-calculate-size-of-a-tree/
 */
int recursive_bst_int_path_len(bst_node_t *N, int level) {
    if (N == NULL) return 0; // Base case for recursion & safety :)

    return recursive_bst_int_path_len(N->left, level+1) + // Find the path length of the left subtree
           recursive_bst_int_path_len(N->right, level+1) + // Find the path length of the right subtree
           level;
}

/* The function bst_rotations was designed to determine the number of rotations from the last remove.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to check.
 *
 *  Return:
 *      An integer value which is the number of rotations from the last remove.
 *
 * Structures Modified: N/A
 */
int bst_rotations(bst_t *T)
{
    if (T == NULL) return 0;

    return T->num_recent_rotations;
}

/* The function bst_debug_print_tree was designed to print the tree T.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to print.
 *
 *  Return: N/A
 *
 * Structures Modified: N/A
 */
void bst_debug_print_tree(bst_t *T)
{
    ugly_print(T->root, 0, T->policy);//XTRA
    printf("\n");
    if (T->size < 64)
        pretty_print(T);
}

/* The function ugly_print was designed to print the binary tree in a manner which is ugly. lol...
 *
 * Inputs:
 *  bst_node_t * N: A node of the tree in which we're going to print.
 *  int level: The level in which the node resides
 *  int policy: A value of either BST or AVL
 *
 *  Return: N/A
 *
 * Structures Modified: N/A
 */
void ugly_print(bst_node_t *N, int level, int policy)
{
    int i;
    if (N == NULL) return;
    ugly_print(N->right, level+1, policy);
    if (policy == AVL) {
        for (i = 0; i<level; i++) printf("       ");
        printf("%5d-%d\n", N->key, N->height);
    } else {
        for (i = 0; i<level; i++) printf("     ");
        printf("%5d\n", N->key);
    }
    ugly_print(N->left, level+1, policy);
}

/* The function bst_debug_validate was designed to validate our tree T.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to check.
 *
 *  Return: N/A
 *
 * Structures Modified: N/A
 *
 * Note: Asserts ensure everything is working properly. If the function returns without ending the program we know that
 *       the tree validated.
 */
void bst_debug_validate(bst_t *T)
{
    int size = 0;
    assert(bst_debug_validate_rec(T->root, INT_MIN, INT_MAX, &size) == TRUE);
    assert(size == T->size);
    if (T->policy == AVL)
        rec_height(T->root);
}

/* The function bst_debug_validate_rec was designed to validate a tree based on node position.
 *
 * Inputs:
 *  bst_node_t * N: A pointer to the node in the tree in which we're going to validate.
 *  int min: Temp min for it's left subtree min
 *  int max: Temp max for it's right subtree max
 *  int * count: A running count which increases everytime the recursive call is made
 *
 *  Return:
 *      An integer value used to ensure validity.
 *
 * Structures Modified: N/A
 */
int bst_debug_validate_rec(bst_node_t *N, int min, int max, int *count)
{
    if (N == NULL)
        return TRUE;
    if (N->key <= min || N->key >= max)
        return FALSE;
    assert(N->data_ptr != NULL);
    *count += 1;
    return bst_debug_validate_rec(N->left, min, N->key, count) &&
           bst_debug_validate_rec(N->right, N->key, max, count);
}

/* The function rec_height was designed to verify the AVL tree properties.
 *
 * Inputs:
 *  bst_node_t * T: A pointer to the node we are wanting to check.
 *
 *  Return: The height of our AVL tree based on the AVL tree property.
 *
 * Structures Modified: N/A
 */
int rec_height(bst_node_t *N)
{
    if (N == NULL)
        return 0;
    int lh = rec_height(N->left);
    int rh = rec_height(N->right);
    int lean = lh - rh;
    assert(lean == 0 || lean == 1 || lean == -1);
    return 1 + MYMAX(lh, rh);
}

/* The function children was designed to recursively count the number of children in the provided tree.
 *
 * Inputs:
 *  bst_node_t * T: A pointer to the node we are wanting to check for children.
 *
 *  Return:
 *      An integer value which is the number of children after all recursive calls have returned.
 *
 * Structures Modified: N/A
 */
int children(bst_node_t *N)
{
    if (N == NULL) return 0;
    return 1 + children(N->left) + children(N->right);
}

/* The function pretty_print was designed to print out the tree to the terminal in ASCII art.
 *
 * Inputs:
 *  bst_t * T: A pointer to the tree we are wanting to print.
 *
 *  Return: N/A
 *
 * Structures Modified: N/A
 */
void pretty_print(bst_t *T)
{
    typedef struct queue_tag {
        bst_node_t *N;
        int level;
        int list_sum;
    } queue_t;

    queue_t Q[T->size];
    int q_head = 0;
    int q_tail = 0;
    int i, j;
    int current_level = 0;
    int col_cnt = 0;
    bst_node_t *N;

    Q[q_tail].N = T->root;
    Q[q_tail].level = 0;
    Q[q_tail].list_sum = 0;
    q_tail++;
    for (i = 0; i < T->size; i++)
    {
        assert(q_head < T->size);
        N = Q[q_head].N;
        if (Q[q_head].level > current_level) {
            printf("\n");
            current_level++;
            col_cnt = 0;
        }
        int left_ch = children(N->left);
        int my_pos = 1 + left_ch + Q[q_head].list_sum;
        int left_child_pos = my_pos;
        if (N->left != NULL)
            left_child_pos = 1 + Q[q_head].list_sum + children(N->left->left);
        int right_child_pos = my_pos;
        if (N->right != NULL)
            right_child_pos = my_pos + 1 + children(N->right->left);
        for (j = col_cnt + 1; j <= right_child_pos; j++)
        {
            if (j == my_pos)
                if (left_child_pos < my_pos)
                    if (N->key < 10)
                        printf("--%d", N->key);
                    else if (N->key < 100)
                        printf("-%d", N->key);
                    else
                        printf("%d", N->key);
                else
                    printf("%3d", N->key);
            else if (j == left_child_pos)
                //printf("  |");
                printf("  /");
            else if (j > left_child_pos && j < my_pos)
                printf("---");
            else if (j > my_pos && j < right_child_pos)
                printf("---");
            else if (j == right_child_pos)
                //printf("--|");
                printf("-\\ ");
            else
                printf("   ");
        }
        col_cnt = right_child_pos;

        if (N->left != NULL) {
            Q[q_tail].N = N->left;
            Q[q_tail].level = Q[q_head].level + 1;
            Q[q_tail].list_sum = Q[q_head].list_sum;
            q_tail++;
        }
        if (N->right != NULL) {
            Q[q_tail].N = N->right;
            Q[q_tail].level = Q[q_head].level + 1;
            Q[q_tail].list_sum = Q[q_head].list_sum + left_ch + 1;
            q_tail++;
        }
        q_head++;
    }
    printf("\n");
}

/* vi:set ts=8 sts=4 sw=4 et: */