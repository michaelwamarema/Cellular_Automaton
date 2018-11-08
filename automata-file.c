/*
  CELLULAR AUTOMATON IN C - automata-file.c
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
#include "automata-array.h"
#include "automata-file.h"
#include "automata-util.h"

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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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
  fileName = NULL;
  pressEnterToContinue();

  return 1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

  if (pFile == NULL) {
    fprintf(stderr, "The file '%s' could not be found.\n", fileName);
    free(fileName);
    return 0;
  }
  free(fileName);
  fileName = NULL;

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
