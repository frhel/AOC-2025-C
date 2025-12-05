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

        strcpy(m->map + m->length, line);
        m->length += strlen(line);
        m->width = strlen(line);

        // TODO: Parse and store data
    }
}

int solve_part1(struct Map *m, int dirs[]) {
    int w = m->width;
    int l = m->length;
    char *map = m->map;
    int dir_len = 8;
    int total = 0;

    for (int i = 0; i < l; i++) {
        if (map[i] == '@') {
            int adj_count = 0;
            for (int d = 0; d < dir_len; d++) {
                if (i + dirs[d] < 0 && i + dirs[d] >= l) {
                    continue;
                }
                if (map[i + dirs[d]] == '@') {
                    adj_count++;
                }
            }
            if (adj_count < 4) {
                printf("Found one at: %d\n", i);
                total++;
            }
        }
    }

    return total;
}

int solve_part2(/* TODO: Add parameters */) {
    // TODO: Implement solution
    return 0;
}

int main() {
    timer_start();

    struct Map map;

    map.map = malloc(100000 * sizeof(char));

    // int because we are getting the size of the array back
    load_data(&map);
    printf("%s\n", timer_checkpoint("Parsing"));



    int dirs[8] = {
        -1 * map.width-1, -1 * map.width, -1 * map.width+1, -1, 1, map.width-1, map.width, map.width+1
    };

    printf("Map: %s\n", map.map);

    int part1 = solve_part1(&map, dirs);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    // TODO: Solve part 2
    // int part2 = solve_part2(/* args */);
    // printf("Part 2 Answer: %d - %s\n", part2, timer_checkpoint("Part 2"));

    // printf("%s\n", timer_total());
    free(map.map);
    return 0;
}
