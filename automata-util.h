#ifndef __AUTOMATA_UTIL_H__
#define __AUTOMATA_UTIL_H__

#include <stdbool.h>
#include <stddef.h>

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

// Converts the unsigned char 'rule' to a null-terminated string made up of the
// characters '0' and '1', corresponding to the binary representation of 'rule'.
// The returned pointer must be manually freed.
char *toBinary(unsigned char rule);

// Gets a boolean value from the user (eg. 'y'/'n'/'yes'/'true'/'F').
// Error codes:
//  -1: input error
//  -2: empty string
//  -3: unrecognised string
int getBool();

// Sets *str to either "true" or "false", depending on the value of 'b'.
// *str must be freed manually.
int boolToString(bool b, char **str);

// Prints the string "Press ENTER to continue..." then waits for the user to
// return.
void pressEnterToContinue();

#endif
