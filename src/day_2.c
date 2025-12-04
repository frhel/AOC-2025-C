#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/2


struct Range {
    long long lower;
    long long upper;
};

// Load and parse input
int load_data(struct Range data[]) {
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Split line by commas
        char *token = strtok(line, ",");
        while (token != NULL) {
            // Save ranges as lower and upper bounds
            if (sscanf(token, "%lld-%lld", &data[count].lower, &data[count].upper) == 2) {
                count++;
            }
            token = strtok(NULL, ",");
        }
    }

    return count;
}

// Get divisor for number of digits
// More accurate than pow(10, n) for large n for some reason
int get_divisor(int count) {
    int divisor = 1;
    for (int i = 0; i < count; i++) {
        divisor *= 10;
    }
    return divisor;
}

// Count number of digits in a number
int count_digits(long long n) {
    if (n ==0) return 1; // only one digit

    int count = 0;
    while (n > 0) {
        count++;
        n /= 10;
    }
    return count;
}

int main() {
    timer_start();

    // Initialize data array
    struct Range data[500];

    // return type int because we are getting the size of the array back
    int data_count = load_data(data);
    printf("%s\n", timer_checkpoint("Parsing"));

    // As we are working with potentially large ranges, use long long
    long long part1 = 0;
    long long part2 = 0;


    for (int i = 0; i < data_count; i++) {
        for (long long r = data[i].lower; r <= data[i].upper; r++) {
            int len = count_digits(r);

            int half = len >> 1; // Bitwise divide by 2

            int size = half + 1;

            while (--size > 0) {
                if (len % size != 0) { continue; }

                // Track if all chunks are the same
                int valid = 1;

                // Get the pattern to match against from the first chunk
                long long pattern = r / get_divisor(len - size);

                // Check all subsequent chunks, starting from chunk at position size
                for (int c = size; c < len; c += size) {
                    long long chunk = (r / get_divisor(len - c - size)) % get_divisor(size);

                    // Break early if a chunk does not match the pattern
                    if (chunk != pattern) {
                        valid = 0;
                        break;
                    }
                }

                if (valid == 1) {
                    // If size is exactly half and length is even, count for part 1
                    if (size == half && len % 2 == 0) {
                        part1 += r;
                    }
                    part2 += r; // If valid, count for part 2 in all cases
                    break; // No need to check smaller sizes
                }                
            }           
        }
    }

    printf("Part 1: %lld\n", part1);
    printf("Part 2: %lld\n", part2);
    
    printf("%s\n", timer_total());
    return 0;
}
