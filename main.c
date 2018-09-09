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
#define UNASSIGNED '\n'

#define TAB '\t'
#define NEWLINE '\n'
#define EOL '\0'
#define SPACE ' '

#define MAX_LINE_SIZE 160
#define MAX_STATES_SIZE 50




/* STRUCTS & TYPEDEFS */

typedef struct STATE State;
typedef struct TRANSITION Transition;

typedef State *ptrState;
typedef Transition *ptrTransition;

struct TRANSITION {
	ptrState head; // following state
	char read;
	char write;
	char move;
	ptrTransition next; // single-linked list
};

struct STATE {
	unsigned int state_number;
    ptrState next;
    ptrTransition children_list;
};


enum input_state {Tr, Acc, Max, Run, Data}; // Tr: transitions, Acc: acceptance
enum manager_state {Start, Building_TM, Acceptance_state, Limit, Steady, Running, Exit, ERROR};





/* GLOBAL VARIABLES & CONSTANTS */



/* PROTOTYPES */

enum input_state inputParser (char *input);
void printTest (char *line);
enum manager_state nextState (enum manager_state actualState, enum input_state input);
char *removeWhiteSpaces (char *input);
void mallocOK (void *ptr);
void nullOK (void *ptr);
ptrState newStateVoid();
ptrState newState (unsigned int state_number);
unsigned int getStateNumber (ptrState s);
char getRead (ptrTransition t);
char getWrite (ptrTransition t);
char getMove (ptrTransition t);
ptrState getHead (ptrTransition t);
ptrTransition getNext (ptrTransition t);
ptrTransition newTransitionVoid();
ptrTransition newTransition(ptrState head, char read, char write, char move);
ptrState turingMachineBuilder(ptrState stateArray[], unsigned  int *stateNum, unsigned int *maxStateFactor, unsigned int tail, unsigned int head, char read, char write, char move);

// TESTS
char *inputToString (enum input_state input);
char *stateToString (enum manager_state state);
void inputManagerTest (enum manager_state state, enum input_state input);
void removeSpacesTest (char *line);




/* MAIN*/

int main (int argc, char *argv[])
{
	enum input_state inputState;
	enum manager_state FSM;
	char *cleanLine;
	
	char * line = (char *) malloc(sizeof(char) * MAX_LINE_SIZE); // prepare input line
	
	FSM = Start; // set the input manager in the initial state
	
	while (fgets (line, MAX_LINE_SIZE, stdin) != NULL) {
		inputState = inputParser (line);
		if (inputState == Data)
			removeSpacesTest (line);
		inputManagerTest (FSM, inputState);
		FSM = nextState (FSM, inputState);
		
	}
}



/* FUNCTIONS & PROCEDURES */

ptrState turingMachineBuilder(ptrState stateArray[], unsigned int *stateNum, unsigned int *maxStateFactor, unsigned int tail, unsigned int head, char read, char write, char move) {
    ptrState state;
    ptrTransition transition;

    nullOK(stateNum);
    if (*stateNum < tail)
        *stateNum = tail;
    if (*stateNum < head)
        *stateNum = head;
    if (*stateNum >= MAX_STATES_SIZE * (*maxStateFactor)) {//add a check for maximum array size, then realloc
        *maxStateFactor = (*stateNum / MAX_STATES_SIZE) + 1;
        stateArray = (ptrState *) realloc(stateArray,
                                          sizeof(ptrState) * (*maxStateFactor) * MAX_STATES_SIZE);
    }


    return state;
}

enum input_state inputParser (char *input)
{
	if (EQUALS == strcmp (input, TR))
		return Tr;
		
	if (EQUALS == strcmp (input, ACC))
		return Acc;
		
	if (EQUALS == strcmp (input, MAX))
		return Max;
		
	if (EQUALS == strcmp (input, RUN))
		return Run;
		
	return Data;
}

enum manager_state nextState (enum manager_state actualState, enum input_state input)
{

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
	
	char *removeWhiteSpaces (char *input)
{
	int i, j;
	size_t len = strlen (input);
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
	
	ret = (char *) malloc (sizeof (char) * j);
	
	for (i = 0; i < j; i++)
		ret[i] = output[i];
		
	return ret;
}

void mallocOK (void *ptr)
{
	if (ptr == NULL) {
		printf ("ERROR: memory allocation failed");
		exit (1);
	}
}

void nullOK (void *ptr)
{
	if (ptr == NULL) {
		printf ("ERROR: tried to access fields of a NULL pointer");
		exit (2);
	}
}

ptrState newStateVoid()
{
	ptrState ret = (ptrState) malloc (sizeof (State));
	mallocOK (ret);
	ret->next = NULL;
	ret->children_list = NULL;
	return ret;
}

ptrState newState (unsigned int state_number)
{
	ptrState ret = newStateVoid();
	ret->state_number = state_number;
	return ret;
}

ptrTransition newTransitionVoid() {
    ptrTransition ret = malloc(sizeof(Transition));
    mallocOK(ret);
    ret->next = NULL;
    ret->head = NULL;
    return ret;
}

ptrTransition newTransition(ptrState head, char read, char write, char move) {
    ptrTransition ret = newTransitionVoid();
    ret->head = head;
    ret->read = read;
    ret->write = write;
    ret->move = move;
    return ret;
}

unsigned int getStateNumber (ptrState s)
{
	nullOK (s);
	return s->state_number;
}

char getRead (ptrTransition t)
{
	nullOK (t);
	return t->read;
}

char getWrite (ptrTransition t)
{
	nullOK (t);
	return t->write;
}

char getMove (ptrTransition t)
{
	nullOK (t);
	return t->move;
}

ptrState getHead (ptrTransition t)
{
	return t->head;
}

ptrTransition getNext (ptrTransition t)
{
	return t->next;
}







/* TESTS */

void inputManagerTest (enum manager_state state, enum input_state input)
{
	printf ("Actual state: %s\n", stateToString (state));
	printf ("Input: %s\n", inputToString (input));
	printf ("Next state: %s\n", stateToString (nextState (state, input)));
	printf ("\n");
}

void inputParserTest (char *line)
{
	enum input_state state = inputParser (line);
	
	printf ("Line: %s ", line);
	printf ("State: ");
	
	switch (state) {
	case Tr:
		printf ("Transition");
		break;
		
	case Acc:
		printf ("Acceptance");
		break;
		
	case Max:
		printf ("Max");
		break;
		
	case Run:
		printf ("Run");
		break;
		
	case Data:
		printf ("Data");
	}
	
	printf ("\n");
}

void removeSpacesTest (char *line)
{
	char *cleanLine;
	
	cleanLine = removeWhiteSpaces (line);
	printf ("Clean line: %5s\n", cleanLine);
}

char *inputToString (enum input_state input)
{
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

char *stateToString (enum manager_state state)
{
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