#include <stdbool.h>

// Displays the menu to the user, automatically looping until the user chooses
// to exit.
void menu();

// Gets input from the user 
char *getInput(size_t bufsize);

FILE *attemptOpen(char* fileName, char* mode);

int saveFile();

char **loadFile();

void clearBuffer(char *input);

// 
void freeOutput();

// TODO: DELETE THIS
char **testAutomaton();

// Initialises the output array.
// Makes no guarantee of the initial line being empty.
void initOutput();

// Runs the automaton with the current settings, populating the rest of the
// output array (the first line must already be populated).
// If printOutput is true, each line will be printed to the screen after it is
// calculated.
int runAutomaton(bool printOutput);

// Prints a single, specified line of the output array
int printLine(size_t line);

// Prints the entire output array
int printOutput();
