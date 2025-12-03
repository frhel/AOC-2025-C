#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/1

struct Move {
    char dir;
    int steps;
}; // Direction and number of steps

int load_data(struct Move data[]) {
    FILE *fp = stdin;
    char line[1024];

    int data_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0; // Strip newline character

        // Split line into direction and steps
        int result = sscanf(line, "%c%d", &data[data_count].dir, &data[data_count].steps);
        if (result == 2) {
            data_count++;
        }
    }

    return data_count;
}

int adjust_position(struct Move move, int position) {
    if (move.dir == 'L') {
        position -= move.steps;
    } else {
        position += move.steps;
    }
    return position;
}

int solve_part1(struct Move moves[], int move_count) {
    int position = 50;
    int count = 0;
    for (int i = 0; i < move_count; i++) {
        struct Move move = moves[i];
        position = adjust_position(move, position) % 100;
        if (position < 0) {
            position += 100; // Wrap around for negative positions
        }
        if (position == 0) {
            count++;
        }
    }

    return count;
}

int solve_part2(struct Move moves[], int move_count) {
    int position = 50;
    int count = 0;
    for (int i = 0; i < move_count; i++) {
        struct Move move = moves[i];
        int dir_modifier = 1;
        if (move.dir == 'L') {
            dir_modifier = -1;
        }

        for (int s = 0; s < move.steps; s++) {
            position += dir_modifier;
            if (position >= 100) {
                position -= 100;
            } else if (position < 0) {
                position += 100;
            }
            if (position == 0) {
                count++;
            }
        }
    }
    
    return count;
}


int main() {
    timer_start();

    struct Move data[5000];
    int move_count = load_data(data);
    printf("%s\n", timer_checkpoint("Parsing"));

    int part1 = solve_part1(data, move_count);
    printf("Part 1 Answer: %d %s\n", part1, timer_checkpoint(""));

    int part2 = solve_part2(data, move_count);
    printf("Part 2 Answer: %d %s\n", part2, timer_checkpoint(""));

    printf("%s\n", timer_total());
    return 0;
}


