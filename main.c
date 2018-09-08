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
#define TRANSITION_LENGTH 5

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
    node this;
    node_ptr prev;
    node_ptr next;
};

enum input_state {Tr, Acc, Max, Run, Data}; // Tr: transitions, Acc: acceptance
enum manager_state {Start, Building_TM, Acceptance_state, Limit, Steady, Running, Exit, ERROR};





/* GLOBAL VARIABLES & CONSTANTS */



/* PROTOTYPES */

enum input_state inputParser(char * input);
void printTest(char *line);
enum manager_state nextState(enum manager_state actualState, enum input_state input);
char * removeWhiteSpaces(char * input);
// TESTS
char * inputToString(enum input_state input);
char * stateToString(enum manager_state state);
void inputManagerTest(enum manager_state state, enum input_state input);
void removeSpacesTest(char *line);




/* MAIN*/

int main (int argc, char *argv[]) {
    enum input_state inputState;
    enum manager_state FSM;
    char * cleanLine;

    char line[MAX_LINE_SIZE];

    FSM = Start; // set the input manager in the initial state

    while (fgets(line, MAX_LINE_SIZE, stdin) != NULL) {
        inputState = inputParser(line);
        if (inputState == Data)
            removeSpacesTest(line);
        inputManagerTest(FSM, inputState);
        FSM = nextState(FSM, inputState);

    }
}



/* FUNCTIONS & PROCEDURES */

enum input_state inputParser(char * input) {
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

enum manager_state nextState(enum manager_state actualState, enum input_state input) {

    switch (actualState) {
        case Start:
            if (input == Tr)
                return Building_TM;
            break;

        case Building_TM:
            if (input == Acc)
                return Acceptance_state;
            if (input == Data)
                return actualState;
            break;

        case Acceptance_state:
            if (input == Max)
                return Limit;
            if (input == Data)
                return actualState;
            break;

        case Limit:
            if (input == Data)
                return Steady;
            break;

        case Steady:
            if (input == Run)
                return Running;
            break;

        case Running:
            if (input == Data)
                return actualState;
            if (input == EOF)
                return Exit;
            break;

        case Exit:
            return Exit;

        case ERROR:
            return ERROR;
    }

    return ERROR;
}

char * removeWhiteSpaces(char * input) {
    int i, j;
    size_t len = strlen(input);
    char output[len];
    char *ret;

    j = 0;
    for (i = 0; i < len; i++) {
        if (input[i] != SPACE) {
            output[j] = input[i];
            j++;
        }
    }

    j--;

    ret = (char *) malloc(sizeof(char) * j);

    for (i = 0; i < j; i++)
        ret[i] = output[i];

    return ret;
}



/* TESTS */

void inputManagerTest(enum manager_state state, enum input_state input) {
    printf ("Actual state: %s\n", stateToString(state));
    printf ("Input: %s\n", inputToString(input));
    printf ("Next state: %s\n", stateToString(nextState(state, input)));
    printf ("\n");
}

void inputParserTest(char* line) {
    enum input_state state = inputParser(line);

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

void removeSpacesTest(char *line) {
    char *cleanLine;

    cleanLine = removeWhiteSpaces(line);
    printf("Clean line: %5s\n", cleanLine);
}

char * inputToString(enum input_state input) {
    switch (input) {
        case Tr:
            return "Transition";

        case Acc:
            return "Acceptance";

        case Max:
            return "Max";

        case Run:
            return "Run";

        case Data:
            return "Data";
    }
}

char * stateToString(enum manager_state state) {
    switch (state) {
        case Start:
            return "Start";

        case Building_TM:
            return "Building TM";

        case Acceptance_state:
            return "Acceptance state";

        case Limit:
            return "Limit";

        case Steady:
            return "Steady";

        case Running:
            return "Running";

        case Exit:
            return "Exit";

        case ERROR:
            return "ERROR";
    }
}