/*
  CELLULAR AUTOMATON IN C - automata-array.c
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
                 Michael Wamarema (150024823)
  LAST MODIFIED: 2018-11-08
*/

#include <stdio.h>
#include <stdlib.h>

#include "automata.h"
#include "automata-array.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int initOutput() {
  if (output != NULL) {
    return 1; // no need to reallocate memory
  }

  output = (char**)malloc(sizeof(char*) * rows);
  if (output == NULL) {
    fprintf(stderr, "initOutput: Out of memory error when trying to allocate rows\n");
    return 0;
  }

  for (size_t i = 0; i < rows; i++) {
    output[i] = (char*)malloc(sizeof(char) * columns);
    if (output[i] == NULL) {
      fprintf(stderr, "initOutput: Out of memory error when trying to allocate cells\n");
      return 0;
    }
    if (i > 0 && output[i] == output[i - 1]) {
      fprintf(stderr, "initOutput: Memory corruption error; same pointer returned by malloc multiple times\n");
      return 0;
    }
  }

  return 1;
}

////////////////////////////////////////////////////////////////////////////////

void clearOutput() {
  for (size_t row = 0; row < rows; row++) {
    for (size_t col = 0; col < columns; col++) {
      output[row][col] = 0;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void freeOutput() {
  if (output == NULL) {
    return;
  }

  for (size_t i = 0; i < rows; i++) {
    free(output[i]);
  }
  free(output);
  output = NULL;
}

////////////////////////////////////////////////////////////////////////////////

int printLine(size_t line) {
  if (output == NULL) {
    fprintf(stderr, "printLine: Output array has not been initialised\n");
    return 0;
  }
  if (line > rows - 1) {
    fprintf(stderr, "printLine: %zu is out of bounds\n", line);
    return 0;
  }

  for (size_t i = 0; i < columns; i++) { // for each cell
    if (output[line][i]) {
      printf("%c", cTrue);
    } else {
      printf("%c", cFalse);
    }
  }
  printf("\n");
  return 1;
}

////////////////////////////////////////////////////////////////////////////////

int printOutput() {
  if (output == NULL) {
    fprintf(stderr, "printOutput: Output array has not been initialised\n");
    return 0;
  }

  for (size_t i = 0; i < rows; i++) { // for each row
    if (!printLine(i)) { return 0; }
  }
  return 1;
}
