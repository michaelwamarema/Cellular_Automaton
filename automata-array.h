#ifndef __AUTOMATA_ARRAY_H__
#define __AUTOMATA_ARRAY_H__

#include <stddef.h>

// Initialises the output array.
// The data in the array will be undefined.
int initOutput();

// Clears the contents of the output array.
void clearOutput();

// Frees the output array and sets output to NULL.
void freeOutput();

// Prints a single, specified line of the output array
int printLine(size_t line);

// Prints the entire output array
int printOutput();

#endif
