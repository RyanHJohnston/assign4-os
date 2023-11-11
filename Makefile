CC=gcc
FLAGS=-Wall -g
SRC=main.c
TARGET=main.o

all:
	# running test and part 1
	$(CC) $(FLAGS) part1.c -o part1.o
	./part1.o docs/part1testsequence docs/part1-out-test
	./part1.o docs/part1sequence docs/part1-output >> OUTPUT.txt
	md5sum docs/part1-output | awk '{print $1}' >> REPORT.txt
	
	# running part 2 
	$(CC) $(FLAGS) part2.c -o part2.o
	./part2.o docs/part2sequence docs/part2-output >> OUTPUT.txt
	./part2.o docs/part2sequence docs/part2-output >> REPORT.txt
	md5sum docs/part2-output | awk '{print $1}' >> REPORT.txt
	
	# remove output files
	rm docs/part1-out-test
	rm docs/part1-output
	rm docs/part2-output

clean:
	rm -f *.o
