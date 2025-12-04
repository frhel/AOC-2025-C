#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/3


struct Bank {
    int battery[100];
    int length;
};

// TODO: Load and parse input
int load_data(struct Bank data[]) {
    char line[1024];
    int count = 0;
    int len = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        len = strlen(line);

        data[count].length = len; // Save the length of each battery bank
        for (int i = 0; i < len; i++) {
            data[count].battery[i] = (int)line[i] - '0';
        }
        
        count++;
    }

    return count;
}

// int solve_part1(struct Bank banks[], int banks_count) {
//     long long total_joltage = 0;
//     for (int i = 0; i < banks_count; i++) {
//         int max = -i;
//         for (int l = 0; l < banks[i].length - 1; l++) {
//             for (int r = l + 1; r < banks[i].length; r++) {
//                 int num = banks[i].battery[l] * 10 + banks[i].battery[r];
//                 if (num > max) {
//                     max = num;
//                 }
//             }   
//         }
//         total_joltage += max;
//     }
//     return total_joltage;
// }

long long solve_part2(struct Bank banks[], int banks_count, int n_batteries) {
    long long total_joltage = 0;
    for (int i = 0; i < banks_count; i++) {

        // Save the last index we used to avoid reusing batteries
        int last_idx = 0;
        long long total = 0;
        
        // Since we only need to find 12 batteries, we can limit our search to 12 iterations per battery
        for (int b = 1; b <= n_batteries; b++) {

             // For every iteration we need to make sure that we leave enough batteries
            // at the end for us to turn on. We do this by limiting the search space
            // to the length of the row minus the number of batteries left to turn on plus
            // the current iteration index (minus one for zero indexing)
            int curr_end = banks[i].length - n_batteries + b - 1;
            
            // The search space for each iteration depends on the last index we used
            // and the current end we calculated above.
            // All we have to do is find the max digit within that span
            long long max = -1;
            for (int s = last_idx; s <= curr_end; s++) {
                if (banks[i].battery[s] > max) {
                    max = banks[i].battery[s];
                    last_idx = s + 1;// Keep track of our current position
                }
            }
            total = total * 10 + max; // Concat the max digit to our total as a string
        }
        total_joltage += total;
    }
    return total_joltage;
}

int main() {
    timer_start();

    struct Bank data[200];
    // int because we are getting the size of the array back
    int data_len = load_data(data);
    printf("%s\n", timer_checkpoint("Parsing"));

    // Using part 2 to solve part 1 for better speed
    int part1 = solve_part2(data, data_len, 2);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    long long part2 = solve_part2(data, data_len, 12);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    printf("%s\n", timer_total());
    return 0;
}
