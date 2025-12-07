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

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (cols == 0) {
            cols = strlen(line);
        }

        for (size_t i = 0; line[i] != '\0'; i++) {
            if (line[i] == '.') map->grid[rows][i] = 0;
            else if (line[i] == '^') map->grid[rows][i] = -1;
            else if (line[i] == 'S') map->grid[rows][i] = 1;
        }

        rows++;
    }
    map->rows = rows;
    map->cols = cols;
}

int count_touched(bool touched[256][256]) {
    int count = 0;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if (touched[i][j]) count++;
        }
    }
    return count;
}

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
    bool touched[256][256] = {false};
    for (size_t row = 0; row < map->rows; row++) {
        for (size_t col = 0; col < map->cols; col++) {
            if (map->grid[row][col] > 0 && map->grid[row + 1][col] == -1) {
                map->grid[row + 1][col + 1] += map->grid[row][col];
                map->grid[row + 1][col - 1] += map->grid[row][col];
                touched[row][col] = true;
            } else if (map->grid[row][col] > 0) {
                map->grid[row + 1][col] += map->grid[row][col];
            }
        }
    }
    return count_touched(touched);
}

long long solve_part2(struct Map *map) {
    long sum = 0;
    int rows = map->rows;
    for (size_t i = 0; i < map->cols; i++) {
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
