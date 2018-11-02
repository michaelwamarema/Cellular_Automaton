/*
  CELLULAR AUTOMATON IN C
  AC21009 COURSEWORK 2, TEAM 21
        AUTHORS: Daniel Kevin Blackley (160007728)
                 Esther Bolik (170010779)
  LAST MODIFIED: 2018-11-01
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

#include "cellAut.h"

unsigned char rule; // The current rule (0..255)
char **output = NULL; // The output of the automaton, as a 2-dimensional array
size_t rows, columns; // The dimensions of the output array
bool xWrap; // Whether or not to wrap the automaton on the x axis (if false, OOB parents will be treated as 0)
wchar_t cTrue, cFalse; // Characters to use when displaying the automaton's output

////////////////////////////////////////////////////////////////////////////////

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

  runAutomaton(true);
  freeOutput();

  menu();
  return 0;
}

void menu() {

  char *input;
  printf("Please select and option from below:\n");

  //set the output to NULL, so we know if the user actually has an automaton currently set up
  output = NULL;
  rows = columns = 0;

  output = testAutomaton();

  printf("\n\nType 1 to Change the settings for the Automaton:\n");
  printf("Type 2 to run the Cellular Automaton:\n");
  printf("Type 3 to Load an Automaton from a file:\n");
  printf("Type 4 to save the most recent Automaton to a file:\n");
  printf("Type 5 to Print out current automaton\n");
  printf("Type 0 to exit:\n");

  //get input from the getInput function
  while (*(input = getInput(2)) != '0') {

    switch (*input) {
      case '1':
        clearBuffer(input);
        //function for changing settings goes here
        break;
      case '2':
        clearBuffer(input);
        //function to run the Automaton
        break;
      case '3':
        clearBuffer(input);
        output = loadFile();
        //if automaton hasn't yet been initialised
        if (output == NULL) {
          printf("Error encountered\n");
        }
        break;
      case '4':
        clearBuffer(input);
        if (!saveFile()) {
          printf("Error encountered\n");
        }
        break;
      case '5':
        clearBuffer(input);
        if(!printOutput()) {
          printf("Error Encountered\n");
        }
        break;
      default:
        printf("Unkown input %s, please try again: \n\n", input);
        break;
    }

    printf("\n\nType 1 to Change the settings for the Automaton:\n");
    printf("Type 2 to run the Cellular Automaton:\n");
    printf("Type 3 to Load an Automaton from a file:\n");
    printf("Type 4 to save the most recent Automaton to a file:\n");
    printf("Type 5 to Print out current automaton\n");
    printf("Type 0 to exit:\n");
  }

  freeOutput();
}

////////////////////////////////////////////////////////////////////////////////

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
}

/*
This function attempts to open the given file in read or write mode
The function takes in the file name and the mode and will return either
the pointer to the file or NULL if there was an error
*/
FILE *attemptOpen(char* fileName, char* mode) {

  FILE *pFile;

  pFile = fopen(fileName, mode);

  if (pFile == NULL) {
    printf("ERR: Directory or File does not exist.\n");
  }

  free(fileName);

  return pFile;
}

/*
The following function saves the array to a file and then returns 0 if a error
was encountered or 1 if it was successful. The parameters are the array to be
written to file and the numbers of rows and columns the array has
*/
int saveFile() {

  if (output == NULL) {
    printf("Cannot save NULL output\n");
    return 0;
  }

  printf("Please enter in the file name to be saved\n");
  char *fileName = getInput(32);
  //Open file in write mode
  FILE *pFile = attemptOpen(fileName, "w");

  if (pFile == NULL) {
    printf("Malloc ran out of Memory\n");
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

/*
The following function loads a file using the fopen function, It then Prompts the user
for a filename to open, and returns a 2d pointer array to what was found within the file
or will return NULL if an error was encountered.

Also takes in pointers to the rows and columns, so that they can be changed to the
correct values
*/
char **loadFile() {

  printf("Please enter in the file name to be loaded\n");
  char *fileName = getInput(32);
  //open the file in read mode
  FILE *pFile = attemptOpen(fileName, "r");

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
  //don't need to cast to a float as this will always be a whole number
  columns = i/c;

  char** boardToLoad = malloc((rows) * sizeof(char *));

  if (boardToLoad == NULL) {

    printf("Malloc fail, out of memory\n");
    return NULL;
  }

  for (i = 0; i < rows; i++) {

    boardToLoad[i] = (char *) malloc((columns) * sizeof(char));

    if (boardToLoad[i] == NULL) {

      printf("Malloc fail, out of memory\n");
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

//clears the stdin
void clearBuffer(char *input) {
  while ((*input = getchar()) != '\n' && *input !=EOF) {  }
}

//frees memory used by output array
void freeOutput() {
  for (size_t i = 0; i < rows; i++) {
    free(output[i]);
  }
  free(output);
}

//used exclusively for testing and shouldn't be submitted in final version
char **testAutomaton() {

  rows = 2;
  columns = 2;

  output = malloc(sizeof(char) * rows);

  for (size_t i = 0; i < rows; i++) {
    output[i] = malloc(sizeof(char) * columns);
  }

  for (size_t i = 0; i < rows; i++) {
    for (size_t c = 0; c < columns; c++) {
      output[i][c] = '0';
    }
  }

  return output;
}

void initOutput() {
  output = malloc(sizeof(char*) * rows);
  for (size_t i = 0; i < rows; i++) {
    output[i] = malloc(sizeof(char) * columns);
  }
}

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
        lParent = (output[row - 1][col - 1]);
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
