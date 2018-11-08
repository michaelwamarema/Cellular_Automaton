#ifndef __AUTOMATA_MENU_H__
#define __AUTOMATA_MENU_H__

#include <stdbool.h>

// Displays the menu to the user, automatically looping until the user chooses
// to exit.
void menu();

// Asks the user for and sets the values of rule (if not Life), columns, rows,
// xWrap, yWrap (if Life), cTrue, and cFalse. Only sets the global variables if
// the user enters values for all variables. This function does not initialise
// the output array.
int setupOptions(bool gameOfLife);

/*
  Initialises the first Row of the Automaton, based on user input.
*/
void setupInitialRow();

/*
  Sets up the initial state for the game of life. Function will allow the user
  to manually change each bit of the board or will allow the user to have the
  board be randomly selected.
*/
void setupInitialGameOfLife();

// Prints the current generation and presents the user with the options to
// advance by a certain number of generations, save the current state to a file,
// or quit to the main menu. This function requires that the ouput array is
// already initialised, and the initial state is already set.
void gameOfLifeInterface();

#endif
