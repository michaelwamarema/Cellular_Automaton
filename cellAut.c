/*
  CELLULAR AUTOMATON IN C
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
                 Michael Wamarema (150024823)
  LAST MODIFIED: 2018-11-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cellAut.h"

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

// NOTE: all function descriptions are located in cellAut.h // TODO

int main(int argc, char const *argv[]) {
  menu();
  return 0;
}

void menu() {
  while (true) { // loop until the function is returned from
    printf("================================\n");
    printf("    CELLULAR AUTOMATA - MENU\n");
    printf("================================\n");
    printf(" [1] Run 1D automaton\n");
    printf(" [2] Run Game of Life\n");
    printf(" [3] Load Game of Life state\n");
    printf(" [0] Exit program\n");

    bool incorrectInput;
    do {
      incorrectInput = false;
      printf("Please enter a number: ");
      int input;
      int status = scanf("%d", &input);
      clearBuffer();
      if (status <= 0) { // ie. they did not enter a number
        continue;
      }

      switch (input) {
        // [1] Run 1D automaton
        case 1:
          if (!setupOptions(false)) { break; }
          initOutput();
          setupInitialRow();

          printf("================================\n");
          runAutomaton(true);
          printf("================================\n");
          pressEnterToContinue();

          // prompt to save to file
          while (true) {
            printf("Do you want to save the output to a file? [y/n]: ");

            int shouldSave = getBool();
            if (shouldSave == -1) { // input error
              fprintf(stderr, "\nmenu: Error when reading from stdin\n");
              break;
            } else if (shouldSave == -2) { // empty string
              continue;
            } else if (shouldSave == -3) { // unrecognised string
              printf("Please type 'yes' or 'no'. ");
              continue;
            } else {
              if (shouldSave) {
                if (!saveToFile(false)) {
                  continue; // if it fails, ask them again
                }
              }
              break;
            }
          }

          freeOutput();
          break;

        // [2] Run Game of Life
        case 2:
          if (!setupOptions(true)) { break; }
          initOutput();
          setupInitialGameOfLife();
          gameOfLifeInterface();
          freeOutput();
          break;

        // [3] Load Game of Life state
        case 3:
          if (!loadFile()) { break; }
          gameOfLifeInterface();
          freeOutput();
          break;

        // [0] Exit program
        case 0:
          return;

        default:
          printf("'%d' is not a valid choice. ", input);
          incorrectInput = true;
          break;
      }
    } while (incorrectInput);
  }
}

////////////////////////////////////////////////////////////////////////////////

int setupOptions(bool gameOfLife) {
  //printf("Leave a field blank at any point to cancel setup and undo your choices.\n"); // this doesn't work on account of scanf being used for most variables

  // temporary variables; if the user cancels, the global variables are unchanged
  unsigned char tRule;
  size_t tRows, tCols;
  int tXWrap, tYWrap;
  char tCTrue, tCFalse;

  bool looping;

  // rule (if 1d automaton)
  if (!gameOfLife) {
    do {
      looping = false;
      printf("Please enter the desired rule (0-255) [previous: %hhu]: ", rule);

      int n = scanf("%hhu", &tRule);
      clearBuffer();
      if (n == EOF) {
        fprintf(stderr, "\nsetupOptions: EOF error when reading from stdin\n");
        return 0;

      } else if (n == 0) {
        if (getchar() == '\n') { // field was left blank
          printf("Setup cancelled.\n");
          return 0;

        } else {
          printf("That was not a valid rule. ");
          looping = true;
        }
      }
    } while (looping);
    char *ruleBin = toBinary(tRule);
    printf("Set rule to %hhu (%s).\n", tRule, ruleBin);
    free(ruleBin);
  }

  // columns
  do {
    looping = false;
    if (gameOfLife) {
      printf("Please enter the number of columns in the grid [previous: %zu]: ", columns);
    } else {
      printf("Please enter the number of cells in each generation (ie. columns) [previous: %zu]: ", columns);
    }

    int n = scanf("%zu", &tCols);
    clearBuffer();
    if (n == EOF) {
      fprintf(stderr, "\nsetupOptions: EOF error when reading from stdin\n");
      return 0;

    } else if (n == 0) {
      if (getchar() == '\n') {
        printf("Setup cancelled.\n");
        return 0;

      } else {
        printf("That was not a valid number. ");
        looping = true;
      }

    } else if (tCols == 0) {
      printf("You must have at least one column. ");
      looping = true;
    } else if (tCols > 64) {
      printf("Sorry, you cannot have more than 64 columns. ");
      looping = true;
    }
  } while (looping);
  printf("Set column number to %zu.\n", tCols);

  // rows
  do {
    looping = false;
    if (gameOfLife) {
      printf("Please enter the number of rows in the grid [previous: %zu]: ", rows);
    } else {
      printf("Please enter the number of generations to iterate through (ie. rows) [previous: %zu]: ", rows);
    }

    int n = scanf("%zu", &tRows);
    clearBuffer();
    if (n == EOF) {
      fprintf(stderr, "\nsetupOptions: EOF error when reading from stdin\n");
      return 0;

    } else if (n == 0) {
      if (getchar() == '\n') {
        printf("Setup cancelled.\n");
        return 0;

      } else {
        printf("That was not a valid number. ");
        looping = true;
      }

    } else if (tRows == 0) {
      printf("You must have at least one row. ");
      looping = true;
    } else if (tRows > 64) {
      printf("Sorry, you cannot have more than 64 rows. ");
      looping = true;
    }
  } while (looping);
  printf("Set row number to %zu.\n", tRows);

  // xWrap
  do {
    looping = false;
    if (gameOfLife) {
      printf("Do you want to wrap the left and right sides of the grid? [y/n]: ");
    } else {
      printf("Do you want to wrap the left and right sides of each line? [y/n]: ");
    }

    tXWrap = getBool();
    if (tXWrap == -1) {
      fprintf(stderr, "\nsetupOptions: Error when reading from stdin\n");
      return 0;

    } else if (tXWrap == -2) {
      printf("Setup cancelled.\n");
      return 0;

    } else if (tXWrap == -3) {
      printf("Please type 'yes' or 'no'. ");
      looping = true;

    }
  } while (looping);
  char *str = NULL;
  boolToString(tXWrap, &str);
  printf("Set x-wrap to '%s'.\n", str);
  free(str);

  // yWrap
  if (gameOfLife) {
    do {
      looping = false;
      printf("Do you want to wrap the top and bottom sides of the grid? [y/n]: ");

      tYWrap = getBool();
      if (tYWrap == -1) {
        fprintf(stderr, "\nsetupOptions: Error when reading from stdin\n");
        return 0;

      } else if (tYWrap == -2) {
        printf("Setup cancelled.\n");
        return 0;

      } else if (tYWrap == -3) {
        printf("Please type 'yes' or 'no'. ");
        looping = true;

      }
    } while (looping);
    char *str = NULL;
    boolToString(tYWrap, &str);
    printf("Set y-wrap to '%s'.\n", str);
    free(str);
  }

  // cTrue
  do {
    looping = false;
    printf("Please enter a single character to use for 'populated' cells [previous: '%c']: ", cTrue);

    int n = scanf("%c", &tCTrue);
    clearBuffer();
    if (n == EOF) {
      fprintf(stderr, "\nsetupOptions: EOF error when reading from stdin\n");
      return 0;

    } else if (n == 0) {
      if (getchar() == '\n') {
        printf("Setup cancelled.\n");
        return 0;

      } else {
        // i'm not sure that this can even happen..?
        printf("That was not a valid character. ");
        looping = true;
      }

    } else if (tCTrue == '\n') {
      printf("Character cannot be newline. ");
      looping = true;

    }
  } while (looping);
  printf("Set 'populated cell' character to '%c'.\n", tCTrue);

  // cFalse
  do {
    looping = false;
    printf("Please enter a single character to use for 'empty' cells [previous: '%c']: ", cFalse);

    int n = scanf("%c", &tCFalse);
    clearBuffer();
    if (n == EOF) {
      fprintf(stderr, "\nsetupOptions: EOF error when reading from stdin\n");
      return 0;

    } else if (n == 0) {
      if (getchar() == '\n') {
        printf("Setup cancelled.\n");
        return 0;

      } else {
        printf("That was not a valid character. ");
        looping = true;
      }

    } else if (tCFalse == '\n') {
      printf("Character cannot be newline. ");
      looping = true;

    } else if (tCFalse == tCTrue) {
      printf("The empty cell character cannot be the same as the populated cell character. ");
      looping = true;
    }
  } while (looping);
  printf("Set 'empty cell' character to '%c'.\n", tCFalse);


  if (!gameOfLife) { rule = tRule; }
  columns = tCols;
  rows = tRows;
  xWrap = tXWrap;
  if (gameOfLife) { yWrap = tYWrap; }
  cTrue = tCTrue;
  cFalse = tCFalse;
  printf("Options setup complete! ");
  pressEnterToContinue();
  return 1;
}

////////////////////////////////////////////////////////////////////////////////

void setupInitialRow() {

  //set defaults
  for (size_t col = 0; col < columns; col++) {
    output[0][col] = 0;
  }
  //default 'on' bit is the middle of the automaton
  output[0][columns / 2] = 1;

  while (true) {
    //print the numbers above the row
    for (size_t col = 0; col < columns; col++) {
      //print out with spaces so that the user can read easier
      printf("%zu ", col);
    }
    printf("\n");
    for (size_t col = 0; col < columns; col++) {
      if (col <= 9) {
        printf("%hhd ", output[0][col]);
      } else {
        printf(" %hhd ", output[0][col]);
      }
    }
    printf("\n");
    printf("Please enter a bit number to toggle, -2 to randomise all bits, or -1 to finish.\n>");

    int input;
    int n = scanf("%d", &input);
    clearBuffer();

    if (n == EOF) {
      fprintf(stderr, "\nsetupInitialRow: Error reading stdin\n");

    } else if (n <= 0) {
      printf("That was not a valid number. ");
      pressEnterToContinue();

    } else if (input == -1) {
      break;

    } else if (input == -2) {
      for (size_t col = 0; col < columns; col++) {
        output[0][col] = rand() % 2;
      }

    } else if ((input > columns - 1) || input < 0) {
      printf("'%d' is not a valid bit number. ", input);
      pressEnterToContinue();

    } else {
      output[0][input] = !output[0][input];
    }
  }

  printf("Initial state setup complete! ");
  pressEnterToContinue();
}

////////////////////////////////////////////////////////////////////////////////

void setupInitialGameOfLife() {

  clearOutput(); // initialise the data in the output array to 0

  while (true) {
    //print the numbers for the user
    printf("\n    ");
    for (size_t col = 0; col < columns; col++) {
      if (col <= 9) {
        printf("%zu  ", col);
      } else {
        printf("%zu ", col);
      }
    }
    printf("\n");
    //print the actual table
    for (size_t row = 0; row < rows; row++) {
      // row number
      if (row <= 9) {
        printf(" %zu ", row);
      } else {
        printf("%zu ", row);
      }

      // row data
      for (size_t col = 0; col < columns; col++) {
        printf(" %hhd ", output[row][col]);
      }

      printf("\n");
    }
    printf("Please enter the row number of a bit to toggle, -2 to randomise all bits, or -1 to finish\n>");

    int inputRow;
    int n = scanf("%d", &inputRow);
    clearBuffer();

    if (n == EOF) {
      fprintf(stderr, "\nsetupInitialGameOfLife: Error reading stdin\n");

    } else if (n <= 0) {
      printf("That was not a valid number. ");
      pressEnterToContinue();

    } else if (inputRow == -1) {
      break;

    } else if (inputRow == -2) {
      for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < columns; col++) {
          output[row][col] = rand() % 2;
        }
      }

    } else if ((inputRow > rows - 1) || inputRow < 0) {
      printf("'%d' is not a valid row number. ", inputRow);
      pressEnterToContinue();

    } else {
      printf("Please enter the column number of the bit to toggle in that row.\n>");
      int inputColumn;
      int x = scanf("%d", &inputColumn);
      clearBuffer();

      if (x == EOF) {
        fprintf(stderr, "\nsetupInitialGameOfLife: Error reading stdin\n");

      } else if (x <= 0) {
        printf("That was not a valid number. ");
        pressEnterToContinue();

      } else if (inputColumn == -1) {
        continue; // cancel bit toggle

      } else if ((inputColumn > columns - 1) || inputColumn < 0) {
        printf("'%d' is not a valid column number. ", inputColumn);
        pressEnterToContinue();

      } else {
        output[inputRow][inputColumn] = !output[inputRow][inputColumn];
      }
    }
  }

  printf("Initial state setup complete! ");
  pressEnterToContinue();
}

////////////////////////////////////////////////////////////////////////////////

void gameOfLifeInterface() {
  long int generation = 0;
  while (true) { // loop until the function is returned from
    printf("\033c");
    printOutput();

    printf("================================\n");
    printf("GENERATION %ld\n", generation);
    printf(" [1] Advance by one generation\n");
    printf(" [2] Advance by a number of generations\n");
    printf(" [3] Save current state to file\n");
    printf(" [0] Exit Game of Life\n");

    bool incorrectInput;
    do {
      incorrectInput = false;
      printf("Please enter a number: ");
      int input;
      int status = scanf("%d", &input);
      clearBuffer();
      if (status <= 0) { // ie. they did not enter a number
        continue;
      }

      switch (input) {
        // [1] Advance by one generation
        case 1:
          runGameOfLife(1);
          generation++;
          break;

        // [2] Advance by a number of generations
        case 2:
          ; // why is c like this
          // get number of generations to advance by
          long int gens;
          bool genValid;
          do {
            genValid = false;
            printf("Please enter the number of generations to advance by: ");

            int n = scanf("%ld", &gens);
            clearBuffer();
            if (n == EOF) {
              fprintf(stderr, "\ngameOfLifeInterface: EOF error when reading from stdin\n");
              break;
            } else if (n == 0) {
              if (getchar() == '\n') {
                break;
              } else {
                printf("That was not a valid number. ");
              }
            } else if (gens == 0) {
              break;
            } else if (gens < 0) {
              printf("You cannot run the automaton backwards. ");
            } else {
              genValid = true;
            }
          } while (!genValid);
          if (!genValid) { break; }

          if (gens == 1) {
            printf("Advancing by 1 generation. ");
          } else {
            printf("Advancing by %ld generations. ", gens);
          }
          pressEnterToContinue();

          runGameOfLife(gens);
          generation += gens;
          break;

        // [3] Save current state to file
        case 3:
          saveToFile(true);
          break;

        // [0] Exit Game of Life
        case 0:
          ;
          int shouldSave;
          bool sInputValid;
          do {
            sInputValid = false;
            printf("Do you want to save the current generation before exiting? [y/n]: ");

            shouldSave = getBool();
            if (shouldSave == -1) {
              fprintf(stderr, "\ngameOfLifeInterface: Error when reading from stdin\n");
              break;
            } else if (shouldSave == -2) {
              printf("Exit cancelled. ");
              break;
            } else if (shouldSave == -3) {
              printf("Please type 'yes' or 'no'. ");
            } else {
              sInputValid = true;
            }
          } while (!sInputValid);
          if (!sInputValid) {
            pressEnterToContinue();
            break;
          }

          if (shouldSave) {
            if (!saveToFile(true)) {
              printf("Exit cancelled. ");
              pressEnterToContinue();
              break;
            }
          } else {
            int exitConfirm;
            bool eInputValid;
            do {
              eInputValid = false;
              printf("Are you sure you want to discard the current automaton? [y/n]: ");

              exitConfirm = getBool();
              if (exitConfirm == -1) {
                fprintf(stderr, "\ngameOfLifeInterface: Error when reading from stdin\n");
                break;
              } else if (exitConfirm == -3) {
                printf("Please type 'yes' or 'no'. ");
              } else if (exitConfirm == -2 || exitConfirm == false) {
                printf("Exit cancelled. ");
                break;
              } else {
                eInputValid = true;
              }
            } while (!eInputValid);
            if (!eInputValid || !exitConfirm) { // if something went wrong or they chose 'n'
              pressEnterToContinue();
              break;
            }
            printf("Returning to main menu...\n");
          }
          return;

        default:
          printf("'%d' is not a valid choice. ", input);
          incorrectInput = true;
          break;
      }
    } while (incorrectInput);
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void initOutput() {
  if (output != NULL) {
    return; // no need to reallocate memory, and the data does not have to be initialised
  }
  output = (char**)malloc(sizeof(char*) * rows);
  for (size_t i = 0; i < rows; i++) {
    output[i] = (char*)malloc(sizeof(char) * columns);
  }
}

void clearOutput() {
  for (size_t row = 0; row < rows; row++) {
    for (size_t col = 0; col < columns; col++) {
      output[row][col] = 0;
    }
  }
}

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

int printLine(size_t line) {
  if (output == NULL) {
    fprintf(stderr, "printLine: Output array has not been initialised\n");
    return 0;
  }
  if (line < 0 || line > rows - 1) {
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
    initOutput(); // convert the "output" pointer to a new 2D array

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
        } else if (parentSum >= 2 || parentSum <= 3) { // ie. 2 <= parentSum <= 3
          output[row][col] = 1;
        } else {
          fprintf(stderr, "runGameOfLife: Sum of parents was %hhd for cell [%zu][%zu] on gen %ld\n", parentSum, row, col, gen);
        }

        // for debug (TODO - remove this)
        //printf("[%zu][%zu] parentSum = %hhd; output = %hhd\n", row, col, parentSum, output[row][col]);
      } // next col
    } // next row

    // free the parent generation
    for (size_t i = 0; i < rows; i++) {
      free(parent[i]);
    }
    free(parent);

  } // next gen
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FILE *attemptOpen(char* fileName, char* mode) {

  FILE *pFile;

  pFile = fopen(fileName, mode);

  if (pFile == NULL) {
    fprintf(stderr, "The file '%s' could not be found.\n", fileName);
  }

  return pFile;
}

int saveToFile(bool gameOfLife) {

  if (output == NULL) {
    fprintf(stderr, "saveToFile: Cannot save NULL output\n");
    return 0;
  }

  printf("Please enter the filename to save to: ");
  char *fileName = NULL;
  getInput(&fileName, NULL);

  if (strlen(fileName) == 0) {
    free(fileName);
    return 0;
  }

  //Open file in write mode
  FILE *pFile = attemptOpen(fileName, "w");

  if (pFile == NULL) {
    free(fileName);
    return 0;
  }

  if (gameOfLife) {
    // note that xWrap and yWrap will always either be 0 or 1, ie. single characters
    fprintf(pFile, ".%d%d%c%c\n", xWrap, yWrap, cTrue, cFalse);
  }

  //Loop writes the array to a file
  for (size_t row = 0; row < rows ; row++) {
    for (size_t col = 0; col < columns ; col++) {
      fputc(output[row][col] ? cTrue : cFalse, pFile);
    }
    //put a newline to signify the end of the line, used when reading
    fputc('\n', pFile);
  }

  //close file
  fclose(pFile);
  printf("Successfully saved output to '%s'! ", fileName);
  free(fileName);
  pressEnterToContinue();

  return 1;
}

int loadFile() {

  printf("Please enter the filename to load from: ");
  char *fileName = NULL;
  getInput(&fileName, NULL);

  if (strlen(fileName) == 0) {
    free(fileName);
    return 0;
  }

  //open the file in read mode
  FILE *pFile = attemptOpen(fileName, "r");
  free(fileName);

  if (pFile == NULL) {
    fprintf(stderr, "The file '%s' could not be found.\n", fileName);
    free(fileName);
    return 0;
  }
  free(fileName);

  size_t i, c;
  char data;
  i = c = 0;

  //Determine how big the Array is going to be for memory allocation purposes
  int gameOfLifeRules = 0;
  while ((data = fgetc(pFile)) != EOF) {
    if (data == '.') {
      //skip the gameOfLifeRules
      gameOfLifeRules = 10;
      //skip the next five characters that pFile reads in
    } else if (gameOfLifeRules > 5) {
      gameOfLifeRules--;
    } else if (data != '\n') {
      i++;
    } else {
      c++;
    }
  }

  if (gameOfLifeRules == 0) {
    fprintf(stderr, "That file does not contain a valid Game of Life state.\n");
    return 0;
  }

  rows = c;
  columns = i / c;

  char** boardToLoad = (char**)malloc((rows) * sizeof(char *));

  if (boardToLoad == NULL) {
    fprintf(stderr, "loadFile: Out of memory error when trying to allocate rows\n");
    return 0;
  }

  for (size_t row = 0; row < rows; row++) {
    boardToLoad[row] = (char *) malloc((columns) * sizeof(char));
    if (boardToLoad[row] == NULL) {
      fprintf(stderr, "loadFile: Out of memory error when trying to allocate cells\n");
      return 0;
    }
    if (row > 0 && boardToLoad[row] == boardToLoad[row - 1]) {
      // i'm... not sure what's happening here
      fprintf(stderr, "loadFile: Memory corruption error; same pointer returned by malloc multiple times\n");
      return 0;
    }
  }

  i = c = 0;
  //rewind the file pointer to the first value
  rewind(pFile);

  //cycle through the data, equating it to the game of life rules
  while ((data = fgetc(pFile)) != EOF) {

    if (gameOfLifeRules >= 0) {
      switch (gameOfLifeRules) {
        case 4:
          xWrap = data;
          break;
        case 3:
          yWrap = data;
          break;
        case 2:
          cTrue = data;
          break;
        case 1:
          cFalse = data;
          break;
      }
      gameOfLifeRules--;

    } else if (data == '\n') {
      i++;
      c = 0;
    } else {
      //set the array to the data in the file
      boardToLoad[i][c++] = (data == cTrue);
    }
  }

  fclose(pFile);
  freeOutput();
  output = boardToLoad;
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int getInput(char **str, size_t *size) {
  bool discardSize = false;
  if (size == NULL) {
    size = (size_t *)malloc(sizeof(size_t));
    *size = 0;
    discardSize = true;
  }

  *size = getline(str, size, stdin);
  if (str == NULL || *str == NULL) {
    fprintf(stderr, "getInput: Out of memory\n");
    return 0;
  }

  // remove the new-line char
  (*size)--;
  (*str)[*size] = '\0';
  *str = (char *)realloc(*str, (sizeof(char) * (*size)) + 1);

  if (discardSize) { free(size); }

  return 1;
}

void clearBuffer() {
  char input;
  while ((input = getchar()) != '\n' && input != EOF) {  }
}

char *toBinary(unsigned char n) {
  size_t length = sizeof(n) * 8; // length of n in bits
  char *binary = (char*)malloc((sizeof(char) * length) + 1); // array of size (length + 1)
  if (binary == NULL) {
    fprintf(stderr, "toBinary: Out of memory\n");
    return NULL;
  }

  for (int i = length - 1; i >= 0; i--) { // from 7..0
    binary[length - 1 - i] = ((n >> i) & 1) + 48;
  }
  binary[length] = '\0'; // add the null-terminator

  return binary;
}

int getBool() {
  char *input = NULL;
  getInput(&input, NULL);
  if (input == NULL) {
    free(input);
    return -1;
  } else if (strlen(input) == 0) {
    free(input);
    return -2;
  } else if (
        !strcmp(input, "y") || !strcmp(input, "Y") || !strcmp(input, "yes") ||
        !strcmp(input, "Yes") || !strcmp(input, "YES") || !strcmp(input, "t") ||
        !strcmp(input, "T") || !strcmp(input, "true") || !strcmp(input, "True") ||
        !strcmp(input, "TRUE")
      ) {
    free(input);
    return true;
  } else if (
        !strcmp(input, "n") || !strcmp(input, "N") || !strcmp(input, "no") ||
        !strcmp(input, "No") || !strcmp(input, "NO") || !strcmp(input, "f") ||
        !strcmp(input, "F") || !strcmp(input, "false") || !strcmp(input, "False") ||
        !strcmp(input, "FALSE")
      ) {
    free(input);
    return false;
  } else {
    free(input);
    return -3;
  }
}

int boolToString(bool boolean, char **str) {
  if (str == NULL) {
    fprintf(stderr, "boolToString: str cannot be null\n");
    return 0;
  }

  if (boolean) {
    *str = (char *)malloc(sizeof(char) * 5);
    if (*str == NULL) {
      fprintf(stderr, "boolToString: Out of memory\n");
      return 0;
    }
    strcpy(*str, "true");
    return 1;

  } else {
    *str = (char *)malloc(sizeof(char) * 6);
    if (*str == NULL) {
      fprintf(stderr, "boolToString: Out of memory\n");
      return 0;
    }
    strcpy(*str, "false");
    return 1;
  }
}

void pressEnterToContinue() {
  printf("Press ENTER to continue... ");
  clearBuffer(); // clearBuffer() calls getchar()
}
