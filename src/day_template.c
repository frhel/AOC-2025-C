#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include <stdbool.h>

// Problem description: https://adventofcode.com/2025/day/DAY_NUMBER

// struct Data {
//     // fields
// };

int load_data() {
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        count++;
    }

    return count;
}

int solve_part1() {
    
    return 0;
}

int solve_part2() {
    
    return 0;
}

int main() {
    timer_start();

    
    load_data();
    printf("%s\n", timer_checkpoint("Parsing"));

    int part1 = solve_part1();
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    int part2 = solve_part2();
    printf("Part 2 Answer: %d - %s\n", part2, timer_checkpoint("Part 2"));

    // printf("%s\n", timer_total());
    return 0;
}
