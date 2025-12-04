#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/2


struct Range {
    long long lower;
    long long upper;
};

// TODO: Load and parse input
int load_data(struct Range data[]) {
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, ",");
        while (token != NULL) {
            if (sscanf(token, "%lld-%lld", &data[count].lower, &data[count].upper) == 2) {
                count++;
            }
            token = strtok(NULL, ",");
        }        
    }

    return count;
}

int get_divisor(int count) {
    int divisor = 1;
    for (int i = 0; i < count; i++) {
        divisor *= 10;
    }
    return divisor;
}

int count_digits(int n) {
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

    struct Range data[500];
    // int because we are getting the size of the array back
    int data_count = load_data(data);
    printf("%s\n", timer_checkpoint("Parsing"));

    long long part1 = 0;
    long long part2 = 0;

    for (int i = 0; i < data_count; i++) {
        for (long long r = data[i].lower; r <= data[i].upper; r++) {
            char num[20];
            sprintf(num, "%lld", r);

            int len = strlen(num);
            int half = (len >> 1); // Bitwise divide by 2
            int size = half + 1;

            while (--size > 0) {
                if (len % size != 0) {
                    continue;
                }
                int valid = 1;
                for (int c = 0; c < len - size; c++) {
                    if (num[c] != num[c + size]) {
                        valid = 0;
                        break;
                    }
                }
                if (valid == 1) {
                    if (size == half && len % 2 == 0) {
                        part1 += r;
                    }
                    part2 += r;
                    break;
                }
            }
            
        }
    }

    printf("Part 1: %lld\n", part1);
    printf("Part 2: %lld\n", part2);
    
    printf("%s\n", timer_total());
    return 0;
}
