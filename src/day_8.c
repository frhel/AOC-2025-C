#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include <stdbool.h>
#include <math.h>

// Problem description: https://adventofcode.com/2025/day/8

// Struct to hold information on distances to peers
struct Peer {
    int id;
    float dist;
};

// Struct to hold 3D point coordinates
struct Point {
    int x;
    int y;
    int z;
};

// Struct to hold box information
struct Box {
    char name[32];
    struct Point coords;
    struct Peer peers[1000]; // Array of peers and their distances
    int peer_count;
    int c_set;
};

// Struct to hold a circuit of connected boxes
struct Circuit {
    int boxes[1024];
    int count;
};

// Struct to hold a pair of boxes and their distance
struct Pair {
    int box1;
    int box2;
    float dist;
};


int load_data(struct Box boxes[]) {
    char line[32];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        strcpy(boxes[count].name, line);

        if (sscanf(line 
                ,"%d,%d,%d" 
                ,&boxes[count].coords.x
                ,&boxes[count].coords.y
                ,&boxes[count].coords.z
            ) == 3
        ) {
            boxes[count].peer_count = 0;
            boxes[count].c_set = -1; 
            count++;
        }

    }

    return count;    
}

// Euclidean distance formula according to Claude:
// distance = √[(x₂-x₁)² + (y₂-y₁)² + (z₂-z₁)²]
float get_dist_3d(struct Point vec1, struct Point vec2) {
    double dx = vec2.x - vec1.x;
    double dy = vec2.y - vec1.y;
    double dz = vec2.z - vec1.z;
    double sum = dx*dx + dy*dy + dz*dz;
    return sqrt(sum);   
}

// Find all neighbours of all boxes and save them with distance
// as peers on a per-box basis
void calculate_distances(struct Box boxes[], int count) {
    // Calculate all distances. Start with the leftmost box and
    // loop through all other boxes to the right. Save the distance
    // to both the left and right box. Next iteration we have n-1 
    // operations and never have to look at the leftmost boxes again
    // as we already have their values saved
    for (int left = 0; left < count - 1; left++) {
        for (int right = left + 1; right < count; right++) {
            float dist = get_dist_3d(boxes[left].coords, boxes[right].coords);

            // Just to make it a little bit more readable
            int l_idx = boxes[left].peer_count;
            int r_idx = boxes[right].peer_count;

            // Save each other's id's in each other's peer entry
            boxes[left].peers[l_idx].id = right;
            boxes[right].peers[r_idx].id = left;

            // Save the distance in each others peer entry
            boxes[left].peers[l_idx].dist = dist;
            boxes[right].peers[r_idx].dist = dist;

            // Increment the peer counter for each
            boxes[left].peer_count++;
            boxes[right].peer_count++;

        }
    }
}

// Sort the peers of each box by distance
int compare_pairs(const void *a, const void *b) {
    const struct Pair *Pair_a = (const struct Pair *)a;
    const struct Pair *Pair_b = (const struct Pair *)b;

    // The input is floats, so we have perform the if statements
    // instead of just returning arithmetic result as qsort
    // expects an int
    if (Pair_a->dist < Pair_b->dist) return -1;
    if (Pair_a->dist > Pair_b->dist) return 1;
    return 0;
}

// Find the shortest distances between all boxes and return them
// as an array of Pair structs
int find_shortest_distances(struct Box boxes[], int count, struct Pair *results, int conns) {
    int max_pairs = (count * (count - 1)) / 2;
    struct Pair *all_distances = malloc(max_pairs * sizeof(struct Pair));
    if (all_distances == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 0;
    }
    
    int total = 0;

    // Collect all unique pairs
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < boxes[i].peer_count; j++) {
            int peer_id = boxes[i].peers[j].id;
            if (i < peer_id) { // Only add each pair once
                all_distances[total].box1 = i;
                all_distances[total].box2 = peer_id;
                all_distances[total].dist = boxes[i].peers[j].dist;
                total++;
            }
        }
    }

    qsort(all_distances, total, sizeof(struct Pair), compare_pairs);

    int result_count = (total < conns) ? total : conns;
    for (int i = 0; i < result_count; i++) {
        results[i] = all_distances[i];
    }

    free(all_distances);
    return result_count;
}

// Helper function to print circuits for debugging
void print_circuits(struct Circuit c_sets[], int count) {

    printf("print_circuits: count=%d\n", count);
    for (int i = 0; i < count; i++) {
        if (c_sets[i].count == 0) continue;
        printf("Circuit %d: ", i);
        for (int j = 0; j < c_sets[i].count; j++) {
            printf("%d ", c_sets[i].boxes[j]);
        }
        printf("\n");
      }
}

// Comparison function for qsort to sort circuits by size
int compare_circuits_by_size(const void *a, const void *b) {
      const struct Circuit *c1 = (const struct Circuit *)a;
      const struct Circuit *c2 = (const struct Circuit *)b;

      // Sort descending (largest first)
      if (c1->count > c2->count) return -1;
      if (c1->count < c2->count) return 1;
      return 0;
  }

// Sort circuits by size in descending order
void sort_circuits_by_size(struct Circuit c_sets[], int count) {
    // sort circuits by descending size
    qsort(c_sets, count, sizeof(struct Circuit), compare_circuits_by_size);
}

// Add box1 to the circuit of box2
void add_to_circuit(int box1, int box2, struct Circuit c_sets[], struct Box boxes[]) {
    int bc_id = boxes[box2].c_set;
    c_sets[bc_id].boxes[c_sets[bc_id].count++] = box1;
    boxes[box1].c_set = bc_id;
}

// Create a new circuit with box1 and box2
void create_circuit(int box1, int box2, struct Circuit c_sets[], int *c_count, struct Box boxes[]) {
    c_sets[*c_count].boxes[c_sets[*c_count].count++] = box1;
    c_sets[*c_count].boxes[c_sets[*c_count].count++] = box2;
    boxes[box1].c_set = *c_count;
    boxes[box2].c_set = *c_count;
    (*c_count)++;
}

// Merge two existing circuits into one if box1 and box2 already belong to different circuits
void merge_circuits(struct Circuit c_sets[], int box1, int box2, struct Box boxes[]) {
    int set1 = boxes[box1].c_set;
    int set2 = boxes[box2].c_set;
    // Find which circuit has a lower index
    int target = (set1 < set2) ? set1 : set2;
    int source = (target == set1) ? set2 : set1;

    // Update all boxes in the source circuit to point to the target circuit
    for (int b = 0; b < c_sets[source].count; b++) {
        int box_id = c_sets[source].boxes[b];
        boxes[box_id].c_set = target;
                }
    // Append all boxes from source to target
    for (int i = 0; i < c_sets[source].count; i++) {
        // Check if box is already in target circuit
        int box_id = c_sets[source].boxes[i];
        bool found = false;
        for (int j = 0; j < c_sets[target].count; j++) {
            if (c_sets[target].boxes[j] == box_id) {
                found = true;
                break;
            }
        }
        if (!found) {
            c_sets[target].boxes[c_sets[target].count++] = box_id;
        }
    }
    c_sets[source].count = 0;
}

// Process a connection between two boxes and create/update/merge circuits accordingly
void process_connection(int box1, int box2, struct Circuit c_sets[], int *c_count, struct Box boxes[]) {
    if (boxes[box1].c_set != -1 && boxes[box2].c_set != -1) {
        // Both boxes are already in a circuit
        if (boxes[box1].c_set != boxes[box2].c_set) {
            merge_circuits(c_sets, box1, box2, boxes);
        }
    }
    else if (boxes[box1].c_set == -1 && boxes[box2].c_set == -1) {
        create_circuit(box1, box2, c_sets, c_count, boxes);
    } else if (boxes[box1].c_set == -1) {
        add_to_circuit(box1, box2, c_sets, boxes);
    } else if (boxes[box2].c_set == -1) {
        add_to_circuit(box2, box1, c_sets, boxes);
    }
}

// Used to check if we have managed to connect all boxes into a single circuit
bool is_single_circuit(struct Box boxes[], int count) {
    // Get the circuit set of the first box
    int c_set = boxes[0].c_set;
    for (int i = 1; i < count; i++) {
        // If any box has a different circuit set, return false
        if (boxes[i].c_set != c_set) {
            return false;
        }
    }
    return true;
}

int solve_part1(struct Box boxes[], int conns, struct Pair *shortest, int found) {
    // Create the circuits array
    static struct Circuit c_sets[1024];
    // Track the circuits array size
    int c_count = 0;

    // Loop through the shortest distances and connect boxes
    // until we have reached the specified number of connections
    for (int i = 0; i < conns; i++) {
        int box1 = shortest[i].box1;
        int box2 = shortest[i].box2;

        // Process the connection between box1 and box2
        process_connection(box1, box2, c_sets, &c_count, boxes);
    }

    // Calculate the product of the 3 largest circuits
    sort_circuits_by_size(c_sets, c_count);
    int sum = 1;
    for (int i = 0; i < 3; i++) {
        int c_size = c_sets[i].count;
        sum = sum * c_size;
    }

    return sum;
}

// Basically same as part 1 but we keep adding connections
// until all boxes are in a single circuit
long long solve_part2(struct Box boxes[], int count, struct Pair *shortest, int found) {
    // Reset circuit sets
    for (int i = 0; i < count; i++) boxes[i].c_set = -1;

    // Create the circuits array
    static struct Circuit c_sets[1024];

    // Reset the circuits array from part 1 since it's static
    memset(c_sets, 0, sizeof(c_sets));
    // Track the circuits array size
    int c_count = 0;

    // Loop through the shortest distances and keep connecting boxes
    // until all boxes are in a single circuit
    for (int i = 0; i < found; i++) {
        int box1 = shortest[i].box1;
        int box2 = shortest[i].box2;

        // Process the connection between box1 and box2
        process_connection(box1, box2, c_sets, &c_count, boxes);

        if (is_single_circuit(boxes, count)) {
            // All boxes are now in a single circuit
            // Return the product of the x coordinates of box1 and box2
            return (long long)boxes[box1].coords.x * (long long)boxes[box2].coords.x;
        }
    }
    return -1; // Should never reach here if input is valid
}



int main() {
    timer_start();

    static struct Box boxes[1024];
    
    int box_count = load_data(boxes);
    printf("%s\n", timer_checkpoint("Parsing"));

    calculate_distances(boxes, box_count);
    printf("%s\n", timer_checkpoint("Find Distances"));

    int max_pairs = (box_count * (box_count - 1)) / 2;
    struct Pair *shortest = malloc(max_pairs * sizeof(struct Pair));
    int found = find_shortest_distances(boxes, box_count, shortest, max_pairs);

    int conns = 1000;
    int part1 = solve_part1(boxes, conns, shortest, found);
    printf("Part 1 Answer: %d - %s\n", part1, timer_checkpoint("Part 1"));

    long long part2 = solve_part2(boxes, box_count, shortest, found);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    printf("%s\n", timer_total());

    free(shortest);
    return 0;
}
