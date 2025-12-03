#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/DAY_NUMBER

// TODO: Define your data structures here
// struct Data {
//     // fields
// };

// TODO: Load and parse input
int load_data(/* TODO: Add parameters */) {
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // TODO: Parse and store data
        count++;
    }

    return count;
}

int solve_part1(/* TODO: Add parameters */) {
    // TODO: Implement solution
    return 0;
}

int solve_part2(/* TODO: Add parameters */) {
    // TODO: Implement solution
    return 0;
}

int main() {
    timer_start();

    // int because we are getting the size of the array back
    int result1 = load_data(/* args */);
    printf("%s\n", timer_checkpoint("Parsing"));

    // TODO: Solve part 1
    // int part1 = solve_part1(/* args */);
    // printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    // TODO: Solve part 2
    // int part2 = solve_part2(/* args */);
    // printf("Part 2 Answer: %d - %s\n", part2, timer_checkpoint("Part 2"));

    // printf("%s\n", timer_total());
    return 0;
}
