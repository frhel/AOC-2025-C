#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/4

struct Map {
    int width;
    int length;
    char *map;
};

// TODO: Load and parse input
void load_data(struct Map *m) {
    char line[1024];
    m->length = 0;
    m->width = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        strcat(m->map, line);
        strcat(m->map, "|");
        m->length += strlen(line) + 1;
        m->width = strlen(line);

        // TODO: Parse and store data
    }
}

void print_map(struct Map *m) {
    int l = m->length;
    char *map = m->map;

    for (int i = 0; i < l; i++) {
        if (map[i] == '|') {
            putchar('\n');
        } else {
            putchar(map[i]);
        }
    }
    putchar('\n');
}

int solve_part1(struct Map *m, int dirs[]) {
    int l = m->length;
    char *map = m->map;
    int dir_len = 8;
    int total = 0;

    for (int i = 0; i < l; i++) {
        if (map[i] == '@') {
            int adj_count = 0;
            for (int d = 0; d < dir_len; d++) {
                int temp = i + dirs[d];
                // printf("Curr = %d Temp: %d\n", i, i + dirs[d]);
                if (temp < 0 || temp >= l || map[temp] == '|') {
                    continue;
                }
                if (map[temp] == '@') {
                    adj_count++;
                }
            }
            if (adj_count < 4) {
                // printf("Found one at: %d\n", i);
                total++;
            }
        }
    }

    return total;
}

int solve_part2(struct Map *m, int dirs[]) {
    int current_total = __INT_MAX__;
    int l = m->length;
    char *map = m->map;
    int dir_len = 8;
    int total = 0;

    while (current_total > 0) {
        current_total = 0;
        for (int i = 0; i < l; i++) {
            if (map[i] == '@') {
                int adj_count = 0;
                for (int d = 0; d < dir_len; d++) {
                    int temp = i + dirs[d];
                    // printf("Curr = %d Temp: %d\n", i, i + dirs[d]);
                    if (temp < 0 || temp >= l || map[temp] == '|') {
                        continue;
                    }
                    if (map[temp] == '@') {
                        adj_count++;
                    }
                }
                if (adj_count < 4) {
                    // printf("Found one at: %d\n", i);
                    current_total++;
                    map[i] = '.';
                }
            }
        }
        total += current_total;
    }

    return total;
}


int main() {
    timer_start();

    struct Map map;

    map.map = malloc(100000 * sizeof(char));

    // int because we are getting the size of the array back
    load_data(&map);
    printf("%s\n", timer_checkpoint("Parsing"));


    // Because of the added '|' characters for newlines
    // we have to adjust the directions accordingly and add
    // an extra 1 to every vertical movement on adjacent rows
    int dirs[8] = {
        -1 * map.width-2, -1 * map.width - 1, -1 * map.width, -1, 1, map.width, map.width + 1, map.width+2
    };


    int part1 = solve_part1(&map, dirs);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    int part2 = solve_part2(&map, dirs);
    printf("Part 2 Answer: %d - %s\n", part2, timer_checkpoint("Part 2"));



    printf("%s\n", timer_total());
    free(map.map);
    return 0;
}
