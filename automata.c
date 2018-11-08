/*
  CELLULAR AUTOMATON IN C - automata.c
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
                 Michael Wamarema (150024823)
  LAST MODIFIED: 2018-11-08
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automata.h"
#include "automata-menu.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* global variables for the current automaton */

// The current rule (0..255). Only used for 1D automata.
unsigned char rule = 30;

// The output of the automaton, as a 2-dimensional array.
// Note that for 1D automata, each row of this array is an individual
// generation, advancing downwards. On the other hand, for the Game of Life, the
// array represents the entire current generation (and only the current
// generation).
char **output = NULL;

// The dimensions of the output array.
// rows represents rows of cells in the Game of Life, but represents generations
// in 1D automata. columns always represents columns of cells.
size_t rows = 5;
size_t columns = 9;

// Whether or not to wrap the automaton on the x axis (if false, OOB parents will be treated as 0)
bool xWrap = false;
// Whether or not to wrap on the y axis (ONLY USED FOR GAME OF LIFE)
bool yWrap = false;

// Characters to use when displaying the automaton's output
char cTrue = 'X';
char cFalse = ' ';

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// NOTE: All function descriptions are located in the relevant header files,
//       rather than in the source files.

int main(int argc, char const *argv[]) {
  menu();
  return 0;
}
