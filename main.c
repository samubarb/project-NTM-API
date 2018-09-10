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
#define TRUE 1
#define FALSE 0

#define TRANSITION_LENGTH 5
#define UNASSIGNED '\n'

#define TAB '\t'
#define NEWLINE '\n'
#define EOL '\0'
#define SPACE ' '

#define MAX_LINE_SIZE 160
#define MAX_STATES_SIZE 50

#define TAIL_POSITION 0
#define READ_POSITION 1
#define WRITE_POSITION 2
#define MOVE_POSITION 3
#define HEAD_POSITION 4

#define ACCEPTANCE_POSITION 0
#define MAX_POSITION 0

#define CHAR_TO_INT_OFFSET '0'


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
	unsigned int acceptance; // TRUE or FALSE (1 or 0)
    ptrState next;
    ptrTransition children_list;
};

enum input_state {Tr, Acc, Max, Run, Data}; // Tr: transitions, Acc: acceptance
enum manager_state {Start, Building_TM, Acceptance_state, Limit, Steady, Running, Exit, ERROR};


/* GLOBAL VARIABLES & CONSTANTS */



/* PROTOTYPES */

enum input_state inputParser (char *input);
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
ptrTransition addTransition(ptrState tail, ptrState head, char read, char write, char move);
ptrState turingMachineBuilder(ptrState stateList, char * cleanLine);
ptrState getStatePtr(ptrState list, unsigned int number);
ptrState addStateOrdered(ptrState list, ptrState state);
void setAcceptanceState(ptrState stateList, unsigned int state_number);
void setAcceptance(ptrState list, char * line);
void setLimit(unsigned int * limit, char * line);

// TESTS
char *inputToString (enum input_state input);
char *stateToString (enum manager_state state);
void inputManagerTest (enum manager_state state, enum input_state input);
void removeSpacesTest (char *line);
void showTM (ptrState s);


/* MAIN */

int main (int argc, char *argv[])
{
	// Needed variables
    enum input_state inputState;
	char *cleanLine;
	char line[MAX_LINE_SIZE];
	ptrState stateList = NULL;
	unsigned int limit;

    enum manager_state inputFSM = Start; // Initialize inputFSM at the beginning

    // Main inputFSM loop
	while (fgets (line, MAX_LINE_SIZE, stdin) != NULL) {
        // get state from input line
	    inputState = inputParser(line);

        switch (inputFSM) {
		    case Building_TM:
                if (inputState == Data) {
                    printf("%s\n", stateToString(inputFSM));
                    stateList = turingMachineBuilder(stateList, removeWhiteSpaces(line));
                }
		        break;

            case Acceptance_state:
                if (inputState == Data)
                    setAcceptance(stateList, line);
                break;

            case Limit:
                if (inputState == Data)
                    setLimit(&limit, line);

            default:
                printf("%s\n", stateToString(inputFSM));
                break;
		}


        // update inputFSM state
        inputFSM = nextState (inputFSM, inputState);
    }

	showTM(stateList);
	printf("Max moves limit: %u\n", limit);
}



/* FUNCTIONS & PROCEDURES */
void setLimit(unsigned int * limit, char * line) {
    nullOK(limit);
    *limit = (unsigned int) atoi(line);
}

void setAcceptance(ptrState list, char * line) {
    unsigned int state_number = (unsigned int) atoi(line);
    setAcceptanceState(list, state_number);
}

void setAcceptanceState(ptrState stateList, unsigned int state_number) {
    ptrState accState = getStatePtr(stateList, state_number);
    nullOK(accState);
    accState->acceptance = TRUE;
}

unsigned int isAcceptanceState(ptrState state) {
    nullOK(state);
    return state->acceptance;
}

ptrState turingMachineBuilder(ptrState stateList, char * cleanLine) {
    char tailChar = cleanLine[TAIL_POSITION];
    char headChar = cleanLine[HEAD_POSITION];
    char read = cleanLine[READ_POSITION];
    char write = cleanLine[WRITE_POSITION];
    char move = cleanLine[MOVE_POSITION];

    unsigned int tail = (unsigned int) tailChar - CHAR_TO_INT_OFFSET;
    unsigned int head = (unsigned int) headChar - CHAR_TO_INT_OFFSET;

    ptrState tailState = getStatePtr(stateList, tail);
    ptrState headState = getStatePtr(stateList, head);

    if (tailState == NULL) {
        tailState = newState(tail);
        stateList = addStateOrdered(stateList, tailState);
    }

    if (headState == NULL) {
        headState = newState(head);
        stateList = addStateOrdered(stateList, headState);
    }

    tailState->children_list = addTransition(tailState, headState, read, write, move);

    return stateList;
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
	ret->acceptance = FALSE;
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

ptrTransition addTransition(ptrState tail, ptrState head, char read, char write, char move) {
    ptrTransition cursor = tail->children_list;
    if (cursor == NULL) {
        cursor = newTransition(head, read, write, move);
        return cursor;
    }

    while (cursor->next != NULL)
        cursor = cursor->next;

    cursor->next = newTransition(head, read, write, move);
    return tail->children_list;
}

ptrState getStatePtr(ptrState list, unsigned int number) {
    if (list == NULL)
        return NULL;

    while (list != NULL)
        if (list->state_number == number)
            return list;
        else list = list->next;

    return NULL;
}

ptrState addStateOrdered(ptrState list, ptrState state) {
    ptrState head, aux;

    if (list == NULL) {
        list = state;
        return list;
    }

    if (list->state_number > state->state_number) {
        state->next = list;
        return state;
    }

    if (list->next == NULL) {
        list->next = state;
        return list;
    }

    head = list;

    while (list->next != NULL) {
        if (list->next->state_number > state->state_number) {
            aux = list->next;
            list->next = state;
            state->next = aux;
            return head;
        }
        list = list->next;
    }

    list->next = state;
    return head;
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

void showTM (ptrState s) {
    ptrTransition t;
    printf("\nNTM states and transitions:\n\n");
    while (s != NULL) {
        printf("> ");
        if (s->acceptance == TRUE)
            printf("ACC ");
        printf("State %u\n", s->state_number);
        t = s->children_list;
        while(t != NULL) {
            printf(">>>> State %u ~ R: %c, W: %c, M: %c\n", t->head->state_number, t->read, t->write, t->move);
            t = t->next;
        }
        s = s->next;
    }
}

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