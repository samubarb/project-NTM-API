#include <stdio.h>
#include <stdlib.h>

/* DEFINES */

#define LEFT 'L'
#define  RIGHT 'R'
#define STOP 'S'

/* STRUCTS & TYPEDEFS */
typedef struct NODE node;
typedef struct NODE_LIST_ELEMENT node_list_element;
typedef struct TRANSITION transition;

typedef node_list_element * node_list_ptr;
typedef node * node_ptr;

struct TRANSITION {
    unsigned int state_number;
    char read;
    char write;
    char move;
};

struct NODE {
    transition t;
    node_list_ptr children;
};

struct NODE_LIST_ELEMENT {
    node ciao;
    node_ptr prev;
    node_ptr next;
};


/* GLOBAL VARIABLES & CONSTANTS */

/* PROTOTYPES */

/* MAIN*/

int main (int argc, char *argv[]) {
    printf ("Hello, World!\n");
    return 0;
}

/* FUNCTIONS & PROCEDURES */