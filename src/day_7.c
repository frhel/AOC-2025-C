#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include <stdbool.h>

// Problem description: https://adventofcode.com/2025/day/7

struct Map {
    long long grid[512][512];
    size_t start_y;
    size_t start_x;
    size_t cols;
    size_t rows;
};

// TODO: Load and parse input
void load_data(struct Map *map) {
    char line[256];
    size_t rows = 0;
    size_t cols = 0;
    bool skip = false;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (skip == true) {
            skip = false;
            continue;
        }
        if (skip == false) skip = true;
        // Null terminating each line
        line[strcspn(line, "\n")] = '\0';

        // Grabbing the column count
        if (cols == 0) {
            cols = strlen(line);
        }

        // Iterating through each character in the line and building
        // out a map, substituting strings with ints
        for (size_t i = 0; line[i] != '\0'; i++) {
            if (line[i] == '.') map->grid[rows][i] = 0;
            else if (line[i] == '^') map->grid[rows][i] = -1;
            else if (line[i] == 'S') {
                 map->grid[rows][i] = 1;
                 map->start_x = i; // Saving starting position for marginal performance gain
            }
        }
        rows++;
    }
    map->rows = rows;
    map->cols = cols;
}

// helper function to visualize the map
void print_grid(struct Map *map) {
    for (size_t row = 0; row < map->rows; row++) {
        for (size_t col = 0; col < map->cols; col++) {
            if (map->grid[row][col] < 0) {
                printf(" ^ ");
            } else if (map->grid[row][col] == 0) {
                printf(" . ");
            } else {
                printf(" %lld ", map->grid[row][col]);
            }
        }
        printf("\n");
    }
}


int solve_part1(struct Map *map) {
    // Keep track of all the splitters we touch
    int count = 0;

    // Silly micro-optimization to only search where the actual
    // beams are and expand gradually with the beams.
    int start_x = map->start_x;
    int end_x = map->start_x;

    for (size_t row = 0; row < map->rows; row++) {       
        for (int col = start_x; col <= end_x; col++) {
            // If we're on a point that's not a 0 or negative,
            // we must be on a beam. Check the next row if there is a splitter
            // If there is, propagate the beams and tally how many
            // beams we have created directly on the grid.
            // Else if no beam splitter, propagate the beam downwards.
            if (map->grid[row][col] > 0 && map->grid[row + 1][col] == -1) {
                map->grid[row + 1][col + 1] += map->grid[row][col];
                map->grid[row + 1][col - 1] += map->grid[row][col];
                count++;
            } else if (map->grid[row][col] > 0) {
                map->grid[row + 1][col] += map->grid[row][col];
            }
        }
        // Expand the search space
        start_x--;
        end_x++;
    }
    // print_grid(map);
    return count;
}

long long solve_part2(struct Map *map) {
    // Just sum the last row of the pyramid
    // Don't include negative numbers.
    long sum = 0;
    int rows = map->rows;
    for (size_t i = 0; i < map->cols; i++) {
        if (map->grid[rows - 1][i] == -1) continue;
        sum += (long long)map->grid[rows - 1][i];
    }
    return sum;
}

int main() {
    timer_start();

    struct Map map;

    load_data(&map);
    printf("%s\n", timer_checkpoint("Parsing"));

    int part1 = solve_part1(&map);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    long long part2 = solve_part2(&map);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    printf("%s\n", timer_total());
    return 0;
}
