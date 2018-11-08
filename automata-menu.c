/*
  CELLULAR AUTOMATON IN C - automata-menu.c
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
                 Michael Wamarema (150024823)
  LAST MODIFIED: 2018-11-08
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "automata.h" // contains global variables
#include "automata-menu.h"
#include "automata-array.h" // contains output array management functions
#include "automata-simulation.h" // contains automaton simulation functions
#include "automata-file.h" // contains save/load functions
#include "automata-util.h" // contains miscellaneous utility functions

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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
          if (!initOutput()) { break; }
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
          if (!initOutput()) { break; }
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
    ruleBin = NULL;
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
  str = NULL;

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
    str = NULL;
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
