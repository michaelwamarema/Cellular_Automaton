#include <stdbool.h>

// Displays the menu to the user, automatically looping until the user chooses
// to exit.
void menu();

// Asks the user for and sets the values of rule (if not Life), columns, rows,
// xWrap, yWrap (if Life), cTrue, and cFalse. Only sets the global variables if
// the user enters values for all variables. This function does not initialise
// the output array.
int setupOptions(bool gameOfLife);

// Prints the current generation and presents the user with the options to
// advance by a certain number of generations, save the current state to a file,
// or quit to the main menu. This function requires that the ouput array is
// already initialised, and the initial state is already set.
void gameOfLifeInterface();

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
int runGameOfLife(long int generations);

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

// Gets a line of input from the user.
//
// Takes a (NULL) char double-pointer (str) and a (0-value or NULL) size_t
// pointer (size). *str will become a pointer to a null-delimited string (NOT
// including the new-line character at the end), and size's value (if size was
// not NULL) will become the length of the string in chars (not including the
// null char).
//
// *str must be manually freed outside of the function.
int getInput(char **str, size_t *size);

// Clears the stdin buffer.
// This should be called after every scanf call.
void clearBuffer();

// Converts the unsigned char 'n' to a null-terminated string made up of the
// characters '0' and '1', corresponding to the binary representation of 'n'.
// The returned pointer must be manually freed.
char *toBinary(unsigned char rule);

// Gets a boolean value from the user (eg. 'y'/'n'/'yes'/'true'/'F').
// Error codes:
//  -1: input error
//  -2: empty string
//  -3: unrecognised string
int getBool();

// Sets str to either "true" or "false", depending on the value of 'b'.
// str must be freed manually.
int boolToString(bool b, char **str);

int setupInitialRow();

int setupInitialGameOfLife();

void pressEnterToContinue();
