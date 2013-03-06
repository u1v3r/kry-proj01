# Makefile pre proj01

CC = gcc
PNAME = kry
CFLAGS = -Wall -ansi -pthread -g -pg 

objects = kasiski.o

build: $(PNAME).c $(objects)
	$(CC) $(CFLAGS) -o $(PNAME) $(PNAME).c $(objects)

kasiski.o: kasiski.h kasiski.c
	$(CC) $(CFLAGS) -c -o kasiski.o kasiski.c 
	
clean: $(PNAME).c
	rm -f $(PNAME) *.o
	
	
	