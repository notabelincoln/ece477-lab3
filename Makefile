CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS = -lwiringPi

all:	lab3a lab3b

lab3a:		lab3a.o
	$(CC) $(LFLAGS) -o lab3a lab3a.o

lab3a.o:	lab3a.c
	$(CC) $(CFLAGS) -c lab3a.c

lab3b:		lab3b.o
	$(CC) $(LFLAGS) -o lab3b lab3b.o

lab3b.o:	lab3b.c
	$(CC) $(CFLAGS) -c lab3b.c

clean:
	rm -f *~ *.o lab3a lab3b
