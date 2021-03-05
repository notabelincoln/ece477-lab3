CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS = -lwiringPi

all:	lab3a lab3b

lab2a:		lab3a.o
	$(CC) $(LFLAGS) -o lab3a lab3a.o

lab2a.o:	lab3a.c
	$(CC) $(CFLAGS) -c lab3a.c

lab2b:		lab3b.o
	$(CC) $(LFLAGS) -o lab3b lab3b.o

lab2b.o:	lab3b.c
	$(CC) $(CFLAGS) -c lab3b.c

clean:
	rm -f *~ *.o lab3a lab3b
