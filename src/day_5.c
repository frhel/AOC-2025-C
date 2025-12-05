#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/5

struct Range {
    long long lower;
    long long upper;
};

struct Ids {
    struct Range range[200];
    long long id[2000];
    int ids_len;
    int ranges_len;
};

void print_ranges(struct Range arr[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%lld-%lld\n", arr[i].lower, arr[i].upper);
    }
}

// TODO: Load and parse input
void load_data(struct Ids *ids) {
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) break;

        if (sscanf(line, "%lld-%lld", &ids->range[count].lower, &ids->range[count].upper) == 2) {
            count++;
        }
    }

    ids->ranges_len = count;
    count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) break;

        if (sscanf(line, "%lld", &ids->id[count]) == 1) {
            count++;
        }
    }

    ids->ids_len = count;
}

int solve_part1(struct Ids *ids) {
    int count = 0;
    for (int i = 0; i < ids->ids_len; i++) {
        for (int r = 0; r < ids->ranges_len; r++) {
            if (ids->id[i] >= ids->range[r].lower && ids->id[i] <= ids->range[r].upper) {
                count++;
            }
        }
    }
    return count;
}

long long solve_part2(struct Ids *ids) {
    long long total = 0;
    for (int i = 0; i < ids->ranges_len; i++) {
        total += ids->range[i].upper + 1 - ids->range[i].lower;
    }
    return total;
}


int compare_ranges(const void *a, const void *b) {
    struct Range *r1 = (struct Range *)a;
    struct Range *r2 = (struct Range *)b;
    
    if (r1->lower < r2->lower) return -1;
    if (r1->lower > r2->lower) return 1;
    return 0;
}



void merge_ranges(struct Ids *ids) {
    // Start by sorting the ranges
    qsort(ids->range, ids->ranges_len, sizeof(struct Range), compare_ranges);
    // printf("ID Ranges after: \n");
    // print_ranges(ids->range, ids->ranges_len);
    for (int i = 0; i < ids->ranges_len - 1; i++) {
        if (ids->range[i].upper + 1 >= ids->range[i+1].lower) {
            if (ids->range[i+1].upper > ids->range[i].upper) {
                ids->range[i].upper = ids->range[i+1].upper;
            }
            memmove(&ids->range[i+1], &ids->range[i+2], (ids->ranges_len - i - 2) * sizeof(ids->range[0]));
            ids->ranges_len--;
            i--;
        }
    }
    // printf("ID Ranges combined: \n");
    // print_ranges(ids->range, ids->ranges_len);
}

int main() {
    timer_start();

    struct Ids ids;

    // int because we are getting the size of the array back
    load_data(&ids);
    merge_ranges(&ids);
    printf("%s\n", timer_checkpoint("Parsing"));

    // printf("ID Ranges before: \n");
    // print_ranges(ids.range, ids.ranges_len);


    int part1 = solve_part1(&ids);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    long long part2 = solve_part2(&ids);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    printf("%s\n", timer_total());
    return 0;
}
