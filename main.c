#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/* DEFINES */

#define LEFT 'L'
#define  RIGHT 'R'
#define STOP 'S'

#define TR "tr\n"
#define ACC "acc\n"
#define MAX "max\n"
#define RUN "run\n"

#define EQUALS 0
#define FIRST_ARG 1

#define TAB '\t'
#define NEWLINE '\n'
#define EOL '\0'
#define SPACE ' '
#define MAX_LINE_SIZE 160

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
    enum input_state state;
    char line[MAX_LINE_SIZE];

    while (fgets(line, MAX_LINE_SIZE, stdin) != NULL) {
        state = inputManager(line);

        printf("Line: %s ", line);
        printf("State: ");

        switch (state) {
            case Tr:
                printf("Transition");
                break;

            case Acc:
                printf("Acceptance");
                break;

            case Max:
                printf("Max");
                break;

            case Run:
                printf("Run");
                break;

            case Data:
                printf("Data");
        }

        printf("\n");
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
