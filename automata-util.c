/*
  CELLULAR AUTOMATON IN C - automata-util.c
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
#include "automata-util.h"

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

////////////////////////////////////////////////////////////////////////////////

void clearBuffer() {
  char input;
  while ((input = getchar()) != '\n' && input != EOF) {  }
}

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

void pressEnterToContinue() {
  printf("Press ENTER to continue... ");
  clearBuffer(); // clearBuffer() calls getchar()
}
