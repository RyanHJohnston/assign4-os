#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define PAGE_TABLE_SIZE 2049
#define FRAME_COUNT 2049

// Define the PTE structure
typedef struct {
    int valid_bit;
    int frame_number;
} PTE;

// Function prototypes
void processLogicalAddress(FILE *infile, FILE *outfile, PTE *page_table, int *free_frames, int *rev_map, int *lru_count);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        return -1;
    }

    // Initialize the page table
    PTE page_table[PAGE_TABLE_SIZE] = {{0, -1}};

    // Frame management arrays
    int free_frames[FRAME_COUNT] = {0, 1, 1, 1, 1, 1, 1, 1};
    int rev_map[FRAME_COUNT] = {-1};
    int lru_count[FRAME_COUNT] = {0};

    // Open the input and output files
    FILE *infile = fopen(argv[1], "rb");
    if (!infile) {
        perror("Error opening input file");
        return -1;
    }

    FILE *outfile = fopen(argv[2], "wb");
    if (!outfile) {
        perror("Error opening output file");
        fclose(infile);
        return -1;
    }

    // Process each logical address
    processLogicalAddress(infile, outfile, page_table, free_frames, rev_map, lru_count);

    // Cleanup
    fclose(infile);
    fclose(outfile);

    return 0;
}

void processLogicalAddress(FILE *infile, FILE *outfile, PTE *page_table, int *free_frames, int *rev_map, int *lru_count) {
    uint64_t LA; // Logical Address
    uint64_t PA; // Physical Address
    int current_time = 0; // To track the most recent usage of pages

    const int d = 7; // Number of bits for the offset
    const uint64_t mask = 0x7F; // Mask for the last 7 bits (offset)
    
    int page_fault_count = 0;

    while(fread(&LA, sizeof(uint64_t), 1, infile) == 1) {
        int pnum = LA >> d; // Extract page number
        uint64_t dnum = LA & mask; // Extract offset

        if (pnum >= PAGE_TABLE_SIZE) {
            fprintf(stderr, "Error: Page number out of bounds\n");
            continue;
        }

        if (page_table[pnum].valid_bit == 0) {
            // Page fault: Need to allocate a frame
            
            ++page_fault_count;
            int frame_found = -1;
            for (int i = 1; i < FRAME_COUNT; i++) { // Starting from 1 as 0 is for OS
                if (free_frames[i]) {
                    frame_found = i;
                    free_frames[i] = 0; // Mark frame as used
                    break;
                }
            }

            if (frame_found == -1) {
                // No free frame found, apply LRU policy
                int lru_frame = -1, lru_time = INT_MAX;
                for (int i = 1; i < FRAME_COUNT; i++) {
                    if (lru_count[i] < lru_time) {
                        lru_time = lru_count[i];
                        lru_frame = i;
                    }
                }

                if (lru_frame != -1) {
                    // Invalidate the old entry
                    int old_page = rev_map[lru_frame];
                    page_table[old_page].valid_bit = 0;

                    frame_found = lru_frame; // Re-allocate this frame
                } else {
                    fprintf(stderr, "Error: LRU frame not found\n");
                    break;
                }
            }

            // Update the page table with the new frame
            page_table[pnum].frame_number = frame_found;
            page_table[pnum].valid_bit = 1;
            rev_map[frame_found] = pnum; // Update reverse mapping
        }

        // Update access time for LRU
        lru_count[page_table[pnum].frame_number] = current_time++;

        // Compute the physical address and write to the outfile
        int fnum = page_table[pnum].frame_number;
        PA = ((uint64_t)fnum << d) + dnum;
        fwrite(&PA, sizeof(uint64_t), 1, outfile);
    }

    printf("Part 2 Page Faults: %d\n", page_fault_count);
}

