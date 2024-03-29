// C Programs from Chapter 2 of
// Data Structures, Algorithms and Software Principles in C
// by Thomas A. Standish

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef char AirportCode[4];
typedef struct NodeTag {
    AirportCode Airport;
    struct NodeTag *Link;
} NodeType;

// Note: this function assumes that the list L contains at least one
// item
void InsertNewSecondNode(char *A, NodeType *L)
{
    assert(L != NULL);
    NodeType *N;                                   /* let N be a list node */

    N = (NodeType *)malloc(sizeof(NodeType));    /* allocate a new node, N */
    strcpy(N->Airport, A);                         /* set N's Airport to A */
    N->Link = L->Link;          /* let N link to the second node of list L */
    L->Link = N;                           /* let L's first node link to N */
}

/**************************************************************************/
NodeType *ListSearch(char *A, NodeType *L)
{
    NodeType *N;            /* N points to successive nodes on the list L */

    /* Initialization */
    N = L;   /* let N start by pointing to the first node of the list L */

    /* While N points to a non-null node on list L */
    /* examine the node to which N points */

    while (N != NULL) {
        if (strcmp(N->Airport,A) == 0) {          /* if N's Airport == A */
            return N;                     /* return the node pointer in N */
        } else {                                            /* otherwise */
            N = N->Link;        /* advance N to the next node on the list */
        }
    }
    return N;                     /* return NULL if no node's Airport == A */
}

/**************************************************************************/
// Note: **L is the address of the variable L, whose value points to the
// first node of list L
void DeleteLastNode(NodeType **L)
{
    NodeType *PreviousNode, *CurrentNode;

    if (*L != NULL) {               /* do nothing if *L was the empty list */

        if ((*L)->Link == NULL) {      /* if *L has exactly one node, then */
            free(*L);                          /* free the node's storage, */
            *L = NULL;        /* set L to be the empty list, and terminate */
        } else {          /* otherwise, list L must have two or more nodes */

            /* initialize a pair of pointers, (PreviousNode,CurrentNode) */
            /* to point to the first and second nodes. */

            PreviousNode = *L;
            CurrentNode =(*L)->Link ;

            /* Advance the pointer pair along L until CurrentNode */
            /* points to the last node */

            while (CurrentNode->Link != NULL) {
                PreviousNode = CurrentNode ;
                CurrentNode = CurrentNode->Link;
            }

            /* Now PreviousNode points to the next-to-last node on the list */
            /* and CurrentNode points to the last node on the list. */

            PreviousNode->Link= NULL;         /* last node gets NULL link */
            free(CurrentNode);      /* recycle storage for discarded node */
        }
    }
}

/**************************************************************************/
// Again, expect **L to be the address of a variable L containing a
// pointer to the first node of the list
void InsertNewLastNode(char *A, NodeType **L)
{
    NodeType *N, *P;

    /* Allocate a new node N with Airport == A and Link == NULL */
    N = (NodeType *)malloc(sizeof(NodeType));
    strcpy(N->Airport, A);
    N->Link = NULL;

    if ( *L == NULL ) {                             /* If list *L is empty,*/
        *L = N;                       /* let N become the new value for *L */
    } else {                                                 /* Otherwise, */
        /* Locate the last node of list L, using pointer variable P */
        P = *L;
        while (P->Link != NULL) P = P->Link;

        /* Finally, link node N onto the end of the list */
        P->Link = N;
    }
}

/**************************************************************************/
void PrintList(NodeType *L)
{
    NodeType *N;                 /* N points to successive nodes on list L */

    /* First, print a left parenthesis */
    printf( "(" );

    /* Let N start by pointing to the first node on the list L */
    N = L;

    /* Provided N doesn't point to an empty node, print N's Airport */
    /* and advance N to point to the next node on the list */

    while (N != NULL) {
        printf("%s",N->Airport);                   /* print airport code */
        N = N->Link;                /* make N point to next node on list */
        if (N != NULL) printf(", ");        /* print comma between items */
    }

    /* Finally, print a closing right parenthesis */
    printf(")\n");
}

/* Here, insert the typedefs and functions defined above for the types */
/* AirportCode and NodeType and the functions InsertNewLastNode, */
/* InsertNewSecondNode, DeleteLastNode, and PrintList. Then declare */
/* a node pointer variable L as shown below: */
int main (void)
{
    NodeType *L;

    /* First, construct the list L == (DUS,ORD,SAN) and print it */

    /* To start things off, let L be the empty list */
    L = NULL;

    /* Insert a new last node in L with airport code "DUS" */
    InsertNewLastNode("DUS",&L);

    /* Insert a new last node in L with airport code "ORD" */
    InsertNewLastNode("ORD",&L);

    /* Insert a new last node in L with airport code "SAN" */
    InsertNewLastNode("SAN",&L);

    /* Now, print the list, to show what it looks like before changing it */
    PrintList(L);

    /* Then, insert a new second node with the airport code "BRU" */
    InsertNewSecondNode("BRU",L);

    /* Print the modified list */
    PrintList(L);

    /* Delete the last node of the list */
    DeleteLastNode(&L);

    /* Finally, print the shortened list */
    PrintList(L);

    return 0;
}
