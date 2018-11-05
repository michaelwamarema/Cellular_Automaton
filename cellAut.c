/*
  CELLULAR AUTOMATON IN C
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
                 Michael Wamarema (150024823)
  LAST MODIFIED: 2018-11-01
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cellAut.h"

////////////////////////////////////////////////////////////////////////////////

/* global variables for the current automaton */

// The current rule (0..255). Only used for 1D automata.
unsigned char rule = 0;

// The output of the automaton, as a 2-dimensional array.
// Note that for 1D automata, each row of this array is an individual
// generation, advancing downwards. On the other hand, for the Game of Life, the
// array represents the entire current generation (and only the current
// generation).
char **output = NULL;

// The dimensions of the output array.
// rows represents rows of cells in the Game of Life, but represents generations
// in 1D automata. columns always represents columns of cells.
size_t rows = 1;
size_t columns = 1;

// Whether or not to wrap the automaton on the x axis (if false, OOB parents will be treated as 0)
bool xWrap = false;
// Whether or not to wrap on the y axis (ONLY USED FOR GAME OF LIFE)
bool yWrap = false;

// Characters to use when displaying the automaton's output
char cTrue = 'X';
char cFalse = ' ';

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// note: all function descriptions are located in cellAut.h

int main(int argc, char const *argv[]) {
  rule = 30;
  rows = 20;
  columns = (rows * 2) - 1;
  initOutput();
  for (size_t i = 0; i < columns; i++) { output[0][i] = 0; }
  output[0][rows-1] = 1;
  xWrap = false;
  cTrue = 'X';
  cFalse = ' ';

  //runAutomaton(true);
  freeOutput();

  rows = 3;
  columns = 3;
  initOutput();
  printOutput();
  runGameOfLife(1);
  printOutput();
  freeOutput();

  menu();
  return 0;
}

void menu() {
  while (true) { // loop until the function is returned from
    printf(" [1] Run 1D automaton\n");
    printf(" [2] Run Game of Life\n");
    //printf(" [3] Load automaton\n");
    //printf(" [4] Save the most recent automaton to a file:\n");
    //printf(" [5] Print out current automaton\n");
    printf(" [0] Exit program\n");

    bool incorrectInput = false;
    do {
      printf("Please enter a number: ");
      int input;
      clearBuffer();
      int status = scanf("%d", &input);
      if (status <= 0) { // ie. they did not enter a number
        continue;
      }

      switch (input) {
        case 1:
          // TODO: function for changing settings goes here
          runAutomaton(true);
          // TODO: prompt to save
          break;

        case 2:
          clearBuffer();
          // TODO: function for changing settings goes here
          // TODO: enter game of life menu
          break;
/*
        case 3:
          clearBuffer();
          output = loadFile();
          //if automaton hasn't yet been initialised
          if (output == NULL) {
            printf("Error encountered\n");
          }
          break;

        case 4:
          clearBuffer();
          if (!saveFile()) {
            printf("Error encountered\n");
          }
          break;

        case 5:
          clearBuffer();
          if(!printOutput()) {
            printf("Error Encountered\n");
          }
          break;
*/
        default:
          printf("'%d' is not a valid choice. ", input);
          incorrectInput = true;
          break;
      }
    } while (incorrectInput);
  }

  freeOutput();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void initOutput() {
  output = malloc(sizeof(char*) * rows);
  for (size_t i = 0; i < rows; i++) {
    output[i] = malloc(sizeof(char) * columns);
  }
}

void freeOutput() {
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

int runGameOfLife(int generations) {
  if (output == NULL) {
    fprintf(stderr, "runGameOfLife: Output array has not been initialised\n");
  }

  // gen 0 is the first generation; gen is incremented at the end of each
  // generation, eg. the step from gen 0 to 1 will increment 'gen' from 0 to 1
  // at the *end* of the loop.
  for (int gen = 0; gen < generations; gen++) {
    // move the previous generation to a separate variable and make a new 2D
    // array to store the new generation
    char **parent = output; // 2D char array for storing the previous state of the board
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
        if (parentSum < 2 || parentSum > 3) {
          output[row][col] = 0;
        } else { // ie. 2 <= parentSum <= 3
          output[row][col] = 1;
        }
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
    fprintf(stderr, "attemptOpen: The file '%s' does not exist.\n", fileName);
  }

  return pFile;
}

int saveFile() {

  if (output == NULL) {
    printf("saveFile: Cannot save NULL output\n");
    return 0;
  }

  printf("Please enter the filename to save to: ");
  char *fileName = NULL;
  getInput(&fileName, NULL);

  //Open file in write mode
  FILE *pFile = attemptOpen(fileName, "w");
  free(fileName);

  if (pFile == NULL) {
    printf("saveFile: Out of memory\n");
    return 0;
  }

  //Loop writes the array to a file
  for (size_t i = 0; i < rows ; i++) {
    for (size_t c = 0; c < columns ; c++) {

      fputc(output[i][c], pFile);
    }
    //put a newline to signify the end of the line, used when reading
    fputc('\n', pFile);
  }

  //close file
  fclose(pFile);

  return 1;
}

char **loadFile() {

  printf("Please enter the filename to load from: ");
  char *fileName = NULL;
  getInput(&fileName, NULL);

  //open the file in read mode
  FILE *pFile = attemptOpen(fileName, "r");
  free(fileName);

  if (pFile == NULL) {
    return NULL;
  }

  size_t i, c;
  char data;
  i = c = 0;

  //Determine how big the Array is going to be for memory allocation purposes
  while ((data = fgetc(pFile)) != EOF) {

    if (data != '\n') {
      i++;
    } else {
      c++;
    }
  }

  rows = c;
  columns = i/c;

  char** boardToLoad = malloc((rows) * sizeof(char *));

  if (boardToLoad == NULL) {
    fprintf(stderr, "loadFile: Out of memory error when trying to allocate rows\n");
    return NULL;
  }

  for (i = 0; i < rows; i++) {

    boardToLoad[i] = (char *) malloc((columns) * sizeof(char));

    if (boardToLoad[i] == NULL) {

      fprintf(stderr, "loadFile: Out of memory error when trying to allocate cells\n");
      return NULL;
    }
  }

  i = c = 0;
  //rewind the file pointer to the first value
  rewind(pFile);

  while ((data = fgetc(pFile)) != EOF) {

    if (data == '\n') {
      i++;
      c = 0;
    } else {
      //set the array to the data in the file
      boardToLoad[i][c++] = data;
    }
  }

  fclose(pFile);
  return boardToLoad;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Gets a line of input from the user.
//
// Takes a (NULL) char double-pointer (str) and a (0-value or NULL) size_t
// pointer (size). *str will become a pointer to a null-delimited string (NOT
// including the new-line character at the end), and size's value will become
// the length of the string in chars (not including the null char).
//
// *str must be manually freed outside of the function.
int getInput(char **str, size_t *size) {
  bool discardSize = false;
  if (size == NULL) {
    size = (size_t *)malloc(sizeof(size_t));
    *size = 0;
    discardSize = true;
    return 0;
  }

  clearBuffer();

  getline(str, size, stdin);
  if (str == NULL) {
    fprintf(stderr, "getInput: Out of memory\n");
    return 0;
  }

  // remove the new-line char
  (*str)[*size] = '\0';
  *str = (char *)realloc(*str, (sizeof(char) * (*size)) + 1);

  if (discardSize) { free(size); }

  return 1;
}

/*
char *getInput(size_t bufsize) {

  fseek(stdin,0,SEEK_END);

  //buffer holds the input from getLine
  char *buffer;

  buffer = (char *)malloc(bufsize * sizeof(char));
  if( buffer == NULL) {

        printf("Malloc fail, Out of memoy.\n");
        return NULL;
    }

    fgets(buffer, bufsize, stdin);

    return buffer;
}*/

void clearBuffer() {
  char input;
  while ((input = getchar()) != '\n' && input != EOF) {  }
}
