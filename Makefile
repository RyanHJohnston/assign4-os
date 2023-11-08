CC=gcc
FLAGS=-Wall -g
SRC=main.c
TARGET=main.o

all:
	$(CC) $(FLAGS) $(SRC) -o $(TARGET)


clean:
	rm -f *.o
