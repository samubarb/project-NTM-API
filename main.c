#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/* DEFINES */

#define LEFT 'L'
#define  RIGHT 'R'
#define STOP 'S'

#define TR "tr"
#define ACC "acc"
#define MAX "max"
#define RUN "run"

#define EQUALS 0
#define FIRST_ARG 1

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

enum input_state {Tr, Acc, Max, Run, Data}; // Tr: transitions, Acc: acceptance


/* GLOBAL VARIABLES & CONSTANTS */

/* PROTOTYPES */
enum input_state inputManager(char * input);



/* MAIN*/

int main (int argc, char *argv[]) {

    int i; // indxes
    enum input_state state;

    for (i = FIRST_ARG; i < argc; i++) {
        state = inputManager(argv[i]);
        printf("State: %s\n", state == Tr ? "Transition" : state == Acc ? "Acceptance" : state == Max? "Max" : state == Run ? "Run" : "Data");
    }
}

/* FUNCTIONS & PROCEDURES */

enum input_state inputManager(char * input) {
    if (EQUALS == strcmp(input, TR))
        return Tr;

    if (EQUALS == strcmp(input, ACC))
        return Acc;

    if (EQUALS == strcmp(input, MAX))
        return Max;

    if (EQUALS == strcmp(input, RUN))
        return Run;

    return Data;
}
