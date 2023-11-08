CC=gcc
FLAGS=-Wall -g
SRC=main.c
TARGET=main.o

all:
	$(CC) $(FLAGS) part1.c -o part1.o
	./part1.o docs/part1testsequence docs/part1-out-test
	./part1.o docs/part1sequence docs/part1-output > OUTPUT.txt
	md5sum docs/part1-output | awk '{print $1}' > REPORT.txt
		
	
clean:
	rm -f *.o
