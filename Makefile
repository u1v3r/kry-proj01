# Makefile pre proj01

CC = gcc
PNAME = proj01
CFLAGS = -Wall -ansi -pthread -g $(PNAME).c -o $(PNAME)

build: $(PNAME).c
	$(CC) $(CFLAGS)

clean: $(PNAME).c
	rm -f $(PNAME)
	
	
	