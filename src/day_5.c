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


// Just so I don't have to keep rewriting the loop for debugging
// void print_ranges(struct Range arr[], int len) {
//     for (int i = 0; i < len; i++) {
//         printf("%lld-%lld\n", arr[i].lower, arr[i].upper);
//     }
// }

// Load and parse input
void load_data(struct Ids *ids) {
    char line[1024];
    int count = 0;

    // Load the ranges first and stop at the first blank line
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) break;

        if (sscanf(line, "%lld-%lld", &ids->range[count].lower, &ids->range[count].upper) == 2) {
            count++;
        }
    }

    // Save the number of ranges loaded and reset count for IDs
    ids->ranges_len = count;
    count = 0;

    // Continue loading the IDs until EOF
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        if (sscanf(line, "%lld", &ids->id[count]) == 1) {
            count++;
        }
    }

    ids->ids_len = count;
}

int solve_part1(struct Ids *ids) {
    // The count is never going to exceed int limits here given
    // the size of the input, so int is fine
    int count = 0;

    // Keep track of where we start searching ranges from to reduce search space
    // This works because the IDs are sorted
    int range_start = 0;

    // Loop through each ID
    for (int i = 0; i < ids->ids_len; i++) {
        // Loop through each range
        for (int r = range_start; r < ids->ranges_len; r++) {
            // Check if ID is in range and increment count if so
            if (ids->id[i] >= ids->range[r].lower && ids->id[i] <= ids->range[r].upper) {
                count++;
                range_start = r; // Save range index to reduce search space
                break;
            } else if (ids->id[i] < ids->range[r].lower) {
                // If the ID is less than the lower bound of this range,
                // it won't be in any further ranges since they are sorted
                break;
            }
        }
    }
    return count;
}

long long solve_part2(struct Ids *ids) {
    // We're working with big numbers here, so use long long
    long long total = 0;

    // We've already merged all ranges, so we can just sum their sizes
    for (int i = 0; i < ids->ranges_len; i++) {
        total += ids->range[i].upper - ids->range[i].lower;
    }
    // Add the number of IDs to account for inclusive ranges
    return total + ids->ranges_len;
}

// For use with qsort() to sort ranges by lower bound
int compare_ranges(const void *a, const void *b) {
    // Have to typecast to Range pointers first
    // because qsort expects void pointers
    struct Range *r1 = (struct Range *)a;
    struct Range *r2 = (struct Range *)b;
    
    // qsort expects an integer return value so we can't just
    // return the difference because of potential overflow
    if (r1->lower < r2->lower) return -1;
    if (r1->lower > r2->lower) return 1;
    return 0;
}

int compare_ids(const void *a, const void *b) {
    // Have to typecast to long long pointers first
    // because qsort expects void pointers
    long long *id1 = (long long *)a;
    long long *id2 = (long long *)b;
    
    // qsort expects an integer return value so we can't just
    // return the difference because of potential overflow
    if (*id1 < *id2) return -1;
    if (*id1 > *id2) return 1;
    return 0;
}

// Merge overlapping and contiguous ranges
void merge_ranges(struct Ids *ids) {   
    // Then loop through and merge overlapping/contiguous ranges
    for (int i = 0; i < ids->ranges_len - 1; i++) {
        // Because we've sorted, we only ever need to check the next range
        // If the next range's upper is >= this range's lower
        // we have an overlap or contiguous range
        if (ids->range[i].upper + 1 >= ids->range[i+1].lower) {
            // Extend this range's upper if the next range's upper is larger
            if (ids->range[i+1].upper > ids->range[i].upper) {
                ids->range[i].upper = ids->range[i+1].upper;
            }
            // Default case means the next range is fully contained in this range
            // so we can just remove it fully by shifting the array left
            memmove(&ids->range[i+1], &ids->range[i+2], (ids->ranges_len - i - 2) * sizeof(ids->range[0]));
            // have to decrement ranges_len as we have shortened the array
            ids->ranges_len--;
            // have to decrement i to recheck this index as it now has a new range
            // and a new neighbor after the memmove
            i--;
        }
    }
}

int main() {
    timer_start();

    struct Ids ids;

    load_data(&ids);
    printf("%s\n", timer_checkpoint("Parsing"));

    // Sort ranges so we can merge them
    qsort(ids.range, ids.ranges_len, sizeof(struct Range), compare_ranges);
    printf("%s\n", timer_checkpoint("Sorting Ranges"));

    merge_ranges(&ids);
    printf("%s\n", timer_checkpoint("Merging Ranges"));

    // Sort the IDs so we can reduce the search space in part 1
    qsort(ids.id, ids.ids_len, sizeof(long long), compare_ids);
    printf("%s\n", timer_checkpoint("Sorting IDs"));


    int part1 = solve_part1(&ids);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    long long part2 = solve_part2(&ids);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    printf("%s\n", timer_total());
    return 0;
}
