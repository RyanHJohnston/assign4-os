#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/*
 * design a simulator that implements the OSs address translationa nd page replacement mechanisms.
 * we only need to design a simulator that handles one process.
 * 1kb physical memory (10 bits)
 * 4kb virtual memory (12 bits)
 * 128 bytes per page and frame (d=7 bits)
 *
 * IMPORTANT:
 * Maximum number of pages a process can access: 32 pages and p = 5 bits
 * Total number of frames: 8 frames and f = 3 bits
 * Number of entries the page table (PT) contains: 32 entries
 *
 * TWO PARTS:
 * Part 1 Address Translation and IO (20 pts):
 * Implement the first program called part1.c
 * it takes two command line parameters 'infile' which is the name of a sequence file containing
 * logical memory addresses and 'outfile' which is the name of the file to which output is written
 * to.
 * Run it as > ./part1.o infile outfile
 * Each logical address (LA) in 'infile' has already been stored as 8-byte (unsigned long) value in
 * binary fomr.at So don't open 'infile' with a text editor b/c the content will look like garbage.
 * Instead, part1.c should open the infile in binary format and reach each logical address into an
 * unsigned long variable LA.
 * Then it maps/converts each LA to the corresponding physical address (PA) 
*/



int 
main(int argc, char *argv[])
{
    printf("hello world!\n");
    
    exit(EXIT_SUCCESS);
}


