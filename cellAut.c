#include <stdio.h>
#include <stdlib.h>

char **loadFile();
int saveFile();
FILE *attemptOpen(char* fileName, char* mode);
char *getInput(size_t bufsize);
void freeAutomata();
void menu();
void clearBuffer(char *input);
char **testAutomata();
int printAutomata();

//Make Automata and it's dimensions availible to all functions
char **automata;
int ROWS, COLUMNS;
/*
Daniel Kevin Blackley - 160007728
*/

/*
The following function uses the getLine function to get input fromt he user, it then
returns that input to the user
*/

int main(int argc, char const *argv[]) {
  menu();
  return 0;
}

void menu() {

  char *input;
  printf("Please select and option from below:\n");

  //set the automata to NULL, so we know if the user actually has an automata currently set up
  automata = NULL;
  ROWS = COLUMNS = 0;

  automata = testAutomata();

  printf("\n\nType 1 to Change the settings for the Automata:\n");
  printf("Type 2 to run the Cellular Automata:\n");
  printf("Type 3 to Load an Automata from a file:\n");
  printf("Type 4 to save the most recent Automata to a file:\n");
  printf("Type 5 to Print out current automata\n");
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
        //function to run the Automata
        break;
      case '3':
        clearBuffer(input);
        automata = loadFile();
        //if automata hasn't yet been initialised
        if (automata == NULL) {
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
        if(!printAutomata()) {
          printf("Error Encountered\n");
        }
        break;
      default:
        printf("Unkown input %s, please try again: \n\n", input);
        break;
    }

    printf("\n\nType 1 to Change the settings for the Automata:\n");
    printf("Type 2 to run the Cellular Automata:\n");
    printf("Type 3 to Load an Automata from a file:\n");
    printf("Type 4 to save the most recent Automata to a file:\n");
    printf("Type 5 to Print out current automata\n");
    printf("Type 0 to exit:\n");
  }

  freeAutomata();
}

int printAutomata() {

  if (automata == NULL) {
    printf("No automata currently loaded\n");
    return 0;
  }
  printf("Current automata:\n\n\n");

  for (size_t i = 0; i < ROWS; i++) {
    for (size_t c = 0; c < COLUMNS; c++) {
      printf("%c", automata[i][c]);
    }
    printf("\n");
  }

  return 1;
}


char *getInput(size_t bufsize) {

  fseek(stdin,0,SEEK_END);

  //buffer holds the input from getLine
  char *buffer;

  buffer = (char *)malloc(bufsize * sizeof(char));
  if( buffer == NULL) {

        printf("Malloc fail, Out of memoy.");
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
written to file and the numbers of ROWS and COLUMNS the array has
*/
int saveFile() {

  if (automata == NULL) {
    printf("Cannot save NULL automata\n");
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
  for (size_t i = 0; i < ROWS ; i++) {
    for (size_t c = 0; c < COLUMNS ; c++) {

      fputc(automata[i][c], pFile);
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

Also takes in pointers to the ROWS and COLUMNS, so that they can be changed to the
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

  ROWS = c;
  //don't need to cast to a float as this will always be a whole number
  COLUMNS = i/c;

  char** boardToLoad = malloc((ROWS) * sizeof(char *));

  if (boardToLoad == NULL) {

    printf("Malloc fail, out of memory\n");
    return NULL;
  }

  for (i = 0; i < ROWS; i++) {

    boardToLoad[i] = (char *) malloc((COLUMNS) * sizeof(char));

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

//frees memory used by automata
void freeAutomata() {

  for (size_t i = 0; i < ROWS; i++) {
    free(automata[i]);
  }
}

//used exclusively for testing and shouldn't be submitted in final version
char **testAutomata() {

  ROWS = 2;
  COLUMNS = 2;

  automata = malloc(sizeof(char) * ROWS);

  for (size_t i = 0; i < ROWS; i++) {
    automata[i] = malloc(sizeof(char) * COLUMNS);
  }

  for (size_t i = 0; i < ROWS; i++) {
    for (size_t c = 0; c < COLUMNS; c++) {
      automata[i][c] = '0';
    }
  }

  return automata;
}
