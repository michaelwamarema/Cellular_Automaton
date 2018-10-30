#include <stdio.h>
#include <stdlib.h>

char **loadFile(int *ROWS, int *COLUMNS);
int saveFile(char **boardToSave, int ROWS, int COLUMNS);
FILE *attemptOpen(char* fileName, char* mode);


/*
Daniel Kevin Blackley - 160007728
*/

/*
The following function uses the getLine function to get input fromt he user, it then
returns that input to the user
*/
char *getInput() {

  //buffer holds the input from getLine
  char *buffer;
  size_t bufsize = 32;

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
int saveFile(char **boardToSave, int ROWS, int COLUMNS) {

  printf("Please enter in the file name to be saved\n");
  char *fileName = getInput();
  //Open file in write mode
  FILE *pFile = attemptOpen(fileName, "w");

  if (pFile == NULL) {
    return 0;
  }

  //Loop writes the array to a file
  for (size_t i = 0; i < ROWS ; i++) {
    for (size_t c = 0; c < COLUMNS ; c++) {

      fputc(boardToSave[i][c], pFile);
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
char **loadFile(int *ROWS, int *COLUMNS) {

  printf("Please enter in the file name to be loaded\n");
  char *fileName = getInput();
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

  *ROWS = c;
  //don't need to cast to a float as this will always be a whole number
  *COLUMNS = i/c;

  char** boardToLoad = malloc((*ROWS) * sizeof(char *));

  if (boardToLoad == NULL) {

    printf("Malloc fail, out of memory\n");
    return NULL;
  }

  for (i = 0; i < *ROWS; i++) {

    boardToLoad[i] = (char *) malloc((*COLUMNS) * sizeof(char));

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
