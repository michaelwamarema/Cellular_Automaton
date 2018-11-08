#ifndef __AUTOMATA_SIMULATION_H__
#define __AUTOMATA_SIMULATION_H__

#include <stdbool.h>

// Runs the automaton with the current settings, populating the rest of the
// output array (the first line must already be populated).
// If printOutput is true, each line will be printed to the screen after it is
// calculated.
int runAutomaton(bool printOutput);

// Runs the Game of Life automaton using the current settings, iterating through
// the specified number of generations, updating the 'output' array to each
// generation in turn.
int runGameOfLife(long int generations);

#endif
