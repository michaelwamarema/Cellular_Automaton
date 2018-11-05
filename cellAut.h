#include <stdbool.h>

// Displays the menu to the user, automatically looping until the user chooses
// to exit.
void menu();

// Initialises the output array.
// Makes no guarantee of the initial line being empty.
void initOutput();

// Frees the output array and sets output to NULL.
void freeOutput();

// Prints a single, specified line of the output array
int printLine(size_t line);

// Prints the entire output array
int printOutput();

// Runs the automaton with the current settings, populating the rest of the
// output array (the first line must already be populated).
// If printOutput is true, each line will be printed to the screen after it is
// calculated.
int runAutomaton(bool printOutput);

// Runs the Game of Life automaton using the current settings, iterating through
// the specified number of generations, updating the 'output' array to each
// generation in turn.
int runGameOfLife(int generations);

/*
  This function attempts to open the given file in read or write mode.
  The function takes in the file name and the mode and will return either
  the pointer to the file or NULL if there was an error.
*/
FILE *attemptOpen(char* fileName, char* mode);

/*
  The following function saves the output array to a file and then returns 0 if
  a error was encountered or 1 if it was successful.
*/
int saveFile();

/*
  The following function loads a file using attemptOpen(), then prompts the user
  for a filename to open, and returns a 2D pointer array to what was found
  within the file, or will return NULL if an error was encountered.

  The function also changes the values of the global rows and columns variables
  to match the new array.

  The returned array must be freed manually (after each of its rows is freed).
*/
char **loadFile();

// Gets input from the user
int getInput(char **str, size_t *size);
//int *getInput(size_t bufsize);

// Clears the stdin buffer
void clearBuffer();
