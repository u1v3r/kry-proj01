# Makefile pre proj01

CC = gcc
PNAME = kry
CFLAGS = -Wall -ansi -pedantic -lm -Ofast

objects = kasiski.o simple_hash_table.o friedman.o other_fcs.o decrypt.o

rebuild: clean all 

all: $(PNAME).c $(objects)
	$(CC) $(CFLAGS) -o $(PNAME) $(PNAME).c $(objects)

kasiski.o: kasiski.h kasiski.c
	$(CC) $(CFLAGS) -c -o kasiski.o kasiski.c 

friedman.o: friedman.h friedman.c
	$(CC) $(CFLAGS) -c -o friedman.o friedman.c 
	
other_fcs.o: other_fcs.h other_fcs.c
	$(CC) $(CFLAGS) -c -o other_fcs.o other_fcs.c 
		
simple_hash_table.o: simple_hash_table.h simple_hash_table.c
	$(CC) $(CFLAGS) -c -o simple_hash_table.o simple_hash_table.c

decrypt.o: decrypt.h decrypt.c
	$(CC) $(CFLAGS) -c -o decrypt.o decrypt.c 
	
clean: $(PNAME).c
	rm -f $(PNAME) *.o *.out *.sum
	
	
	
