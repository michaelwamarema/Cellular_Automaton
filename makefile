CC = gcc
CFLAGS = -Wall -Werror -pedantic

all: automata.o automata-menu.o automata-array.o automata-simulation.o automata-file.o automata-util.o
	$(CC) $(CFLAGS) -o automata automata.o automata-menu.o automata-array.o automata-simulation.o automata-file.o automata-util.o

automata.o: automata.c automata.h automata-menu.h
	$(CC) $(CFLAGS) -c automata.c

automata-menu.o: automata-menu.c automata.h automata-menu.h automata-array.h automata-simulation.h automata-file.h automata-util.h
	$(CC) $(CFLAGS) -c automata-menu.c

automata-array.o: automata-array.c automata.h automata-array.h
	$(CC) $(CFLAGS) -c automata-array.c

automata-simulation.o: automata-simulation.c automata.h automata-array.h automata-simulation.h
	$(CC) $(CFLAGS) -c automata-simulation.c

automata-file.o: automata-file.c automata.h automata-array.h automata-file.h automata-util.h
	$(CC) $(CFLAGS) -c automata-file.c

automata-util.o: automata-util.c automata.h automata-util.h
	$(CC) $(CFLAGS) -c automata-util.c

clean:
	rm automata *.o
