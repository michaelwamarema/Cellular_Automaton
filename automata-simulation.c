/*
  CELLULAR AUTOMATON IN C - automata-simulation.c
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
                 Michael Wamarema (150024823)
  LAST MODIFIED: 2018-11-08
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "automata.h"
#include "automata-array.h"
#include "automata-simulation.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int runAutomaton(bool printOutput) {
  if (output == NULL) {
    fprintf(stderr, "runAutomaton: Output array has not been initialised\n");
    return 0;
  }

  if (printOutput) { printLine(0); } // print the initial line

  for (size_t row = 1; row < rows; row++) { // for each row except the first
    for (size_t col = 0; col < columns; col++) { // for each cell in the row
      char lParent;
      if (col > 0) {
        lParent = output[row - 1][col - 1];
      } else if (xWrap) {
        lParent = output[row - 1][columns - 1];
      } else {
        lParent = 0;
      }

      char cParent = output[row - 1][col];

      char rParent;
      if (col < columns - 1) {
        rParent = output[row - 1][col + 1];
      } else if (xWrap) {
        rParent = output[row - 1][0];
      } else {
        rParent = 0;
      }

      // treat the cell's parents as a three-bit binary number
      char parents = (lParent * 4) + (cParent * 2) + rParent;
      if (parents < 0 || parents >= 8) {
        fprintf(stderr, "runAutomaton: State of parents was %hhd/%hhd/%hhd (%hhd) for cell [%zu][%zu]\n", lParent, cParent, rParent, parents, row, col);
        return 0;
      }

      // check that bit of the rule and store it in the current cell
      output[row][col] = !!(rule & (1 << parents));
    }
    if (printOutput) { printLine(row); }
  }
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int runGameOfLife(long int generations) {
  if (output == NULL) {
    fprintf(stderr, "runGameOfLife: Output array has not been initialised\n");
  }

  // gen 0 is the first generation; gen is incremented at the end of each
  // generation, eg. the step from gen 0 to 1 will increment 'gen' from 0 to 1
  // at the *end* of the loop.
  for (long int gen = 0; gen < generations; gen++) {
    // move the previous generation to a separate variable and make a new 2D
    // array to store the new generation
    char **parent = output; // 2D char array for storing the previous state of the board
    output = NULL; // if this is not done, initOutput() will just try to recycle the current array
    if (!initOutput()) { return 0; } // convert the "output" pointer to a new 2D array

    for (size_t row = 0; row < rows; row++) { // for every row
      for (size_t col = 0; col < columns; col++) { // for every cell of that row
        // sum of the values of the 8 neighbours in the parent generation
        char parentSum = 0;

        // upper row
        if (row > 0 || yWrap) {
          char *uRow; // the row that we're treating as being immediately above this one
          if (row > 0) { // ie. there is a row above the current one
            uRow = parent[row - 1];
          } else if (yWrap) {
            uRow = parent[rows - 1]; // bottom row
          }

          // upper-left cell
          if (col > 0) { // ie. there is a column to the left of the current one
            parentSum += uRow[col - 1];
          } else if (xWrap) {
            parentSum += uRow[columns - 1]; // rightmost cell of uRow
          }
          // upper-middle cell
          parentSum += uRow[col]; // the current col evidently exists
          // upper-right cell
          if (col + 1 < columns) { // ie. there is a column to the right of the current one
            parentSum += uRow[col + 1];
          } else if (xWrap) {
            parentSum += uRow[0]; // leftmost cell of uRow
          }
        }

        // center-left cell
        if (col > 0) { // ie. there is a column to the left of the current one
          parentSum += parent[row][col - 1];
        } else if (xWrap) {
          parentSum += parent[row][columns - 1];
        }
        // center-right cell
        if (col + 1 < columns) {
          parentSum += parent[row][col + 1];
        } else if (xWrap) {
          parentSum += parent[row][0];
        }

        // lower row
        if (row + 1 < rows || yWrap) {
          char *dRow;
          if (row + 1 < rows) { // ie. there is a row below the current one
            dRow = parent[row + 1];
          } else if (yWrap) {
            dRow = parent[0]; // top row
          }

          // lower-left cell
          if (col > 0) {
            parentSum += dRow[col - 1];
          } else if (xWrap) {
            parentSum += dRow[columns - 1]; // rightmost cell of dRow
          }
          // lower-middle cell
          parentSum += dRow[col];
          // lower-right cell
          if (col + 1 < columns) {
            parentSum += dRow[col + 1];
          } else if (xWrap) {
            parentSum += dRow[0]; // leftmost cell of dRow
          }
        }

        // Calculate value of current cell according to the rules of Life. It is
        // assumed that each cell holds either the value '0' (dead) or '1'
        // (alive).
        if ((parentSum <= 1 && parentSum >= 0) || (parentSum >= 4 && parentSum <= 8)) {
          output[row][col] = 0;
        } else if (parentSum == 3) {
          output[row][col] = 1;
        } else if (parentSum == 2) {
          output[row][col] = parent[row][col];
        } else {
          fprintf(stderr, "runGameOfLife: Sum of parents was %hhd for cell [%zu][%zu] on gen %ld\n", parentSum, row, col, gen);
        }

      } // next col
    } // next row

    // free the parent generation
    for (size_t i = 0; i < rows; i++) {
      free(parent[i]);
    }
    free(parent);
    parent = NULL;

  } // next gen
  return 1;
}
