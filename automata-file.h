#ifndef __AUTOMATA_FILE_H__
#define __AUTOMATA_FILE_H__

#include <stdbool.h>
#include <stdio.h>

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
int saveToFile(bool gameOfLife);

/*
  The following function loads a file using attemptOpen(), then prompts the user
  for a filename to open, and sets 'output' to the loaded array, or returns 0 if
  an error occurred.

  The function also changes the values of the global rows and columns variables
  to match the new array.

  The output array must be freed using freeOutput().
*/
int loadFile();

#endif
