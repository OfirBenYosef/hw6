CC=gcc
CCLINK=$(CC)
CFLAGS=-g -Wall -std=c99
OBJS=ransom.o
EXEC=prog.exe
RM=*.exe

all: $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)

ransom.o: ransom.c
	$(CC) $(CFLAGS) -c ransom.c
	

clean:
	rm -fr $(RM) 
