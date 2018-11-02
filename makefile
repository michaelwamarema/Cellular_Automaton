CC = gcc
CFLAGS = -Wall -Werror -pedantic

all: cellAut.o
	$(CC) $(CFLAGS) -o cellAut cellAut.o

cellAut.o: cellAut.c cellAut.h
	$(CC) $(CFLAGS) -c cellAut.c

clean:
	rm cellAut cellAut.o
