#ifndef __AUTOMATA_H__
#define __AUTOMATA_H__

#include <stdbool.h>
#include <stddef.h>

// for descriptions, see automata.c
extern unsigned char rule;
extern char **output;
extern size_t rows, columns;
extern bool xWrap, yWrap;
extern char cTrue, cFalse;

#endif
