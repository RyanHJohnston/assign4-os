#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // for using uint64_t, which ensures an unsigned long is 8 bytes

int main(int argc, char *argv[]) {
    // Check for proper usage and arguments
    if (argc != 3) {
        fprintf(stderr, "ERROR: Improper parameters when running file\n");
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        return -1;
    }

    // Page table as given in the instructions
    int PT[8] = {2, 4, 1, 7, 3, 5, 6, -1}; // -1 will indicate an invalid mapping
    uint64_t LA, PA;
    int pnum, fnum;
    const int d = 7;
    const uint64_t mask = 0x7F; // Mask for the last 7 bits

    // Open the input file for reading in binary mode
    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        perror("Error opening input file");
        return -1;
    }

    // Open the output file for writing in binary mode
    FILE *outfile = fopen(argv[2], "wb");
    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile); // Close the infile before returning
        return -1;
    }

    // Read each logical address and process it
    while (fread(&LA, sizeof(uint64_t), 1, infile) == 1) {
        // Extract the page number and offset
        pnum = LA >> d;
        uint64_t dnum = LA & mask;

        // Get frame number from page table
        fnum = PT[pnum];

        if (fnum == -1) {
            fprintf(stderr, "Invalid page number: %d\n", pnum);
            fclose(infile);
            fclose(outfile);
            return -1;
        }

        // Calculate the physical address
        PA = ((uint64_t)fnum << d) + dnum;

        // Write the physical address to the output file
        if (fwrite(&PA, sizeof(uint64_t), 1, outfile) != 1) {
            perror("Error writing to output file");
            fclose(infile);
            fclose(outfile);
            return -1;
        }

        printf("Logical address: %lu\n", LA);
    }
    
    // Close both the input and output files
    fclose(infile);
    fclose(outfile);

    return 0;
}
