#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "timer.h"

// Problem description: https://adventofcode.com/2025/day/6

struct Math {
    char numbers[8][2048][5];
    size_t row_count;
    size_t col_count;
    char symbols[2048];
    int col_len[2048];
};


// TODO: Load and parse input
void load_data(struct Math *math) {
    char line[4096];
    size_t row = 0;
    size_t col = 0;
    int col_len = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
            col = 0;
            if (line[0] != '*' && line[0] != '+') continue;

            col_len = 0;
            size_t str_len = strlen(line);

            // Track whether we are starting a new column
            bool next = true;
            for (size_t i = 0; i < str_len; i++) {
                // When we're starting a new column(next is true)
                // we save the symbol and disable the switch until
                // the next symbol. Also increment the column count
                if (next) {
                    math->symbols[col] = line[i];
                    next = !next;
                    col++;
                }

                // Increment the current column length so we can save it
                // to the length array for quick access later
                col_len++;

                // If the next character is outside the range
                // of the input we save the last column's size
                // and break the loop
                if (i+1 >= str_len) {
                    math->col_len[col - 1] = col_len + 1;
                    break;
                }
                
                // If we're still inside the range of the input
                // we check if the upcoming char is an operation
                // and if so, we set the flag to start a new
                // column on the next iteration. In addition we
                // save the column length and reset it for the next
                // round.
                if (line[i+1] == '*' || line[i+1] == '+') {
                    next = !next;
                     // Take into account extra space between columns
                     // And that we have already incremented the column
                     // count
                    math->col_len[col - 1] = col_len - 1;
                    col_len = 0;
                }
            }
        row++;
    }
    math->col_count = col;
    rewind(stdin);

    row = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '*' || line[0] == '+') break;
        size_t start = 0; // start reading columns at this index
        size_t width;
        char str[5]; // Add one for null termination
        for (size_t col = 0; col < math->col_count; col++) {
            width = math->col_len[col]; // Current column width
            size_t str_idx = 0; // Start counting a new string
            // String in column will be the size of the column width
            for (size_t s = 0; s < width; s++) {
                char ch = line[start + s]; // Current character in raw input
                // Add the character to the new string in the right place
                // Change any spaces to dots.
                str[str_idx++] = (ch == ' ') ? '.' : ch;
            }
            str[str_idx] = '\0'; // Terminate the string
            strcpy(math->numbers[row][col], str); // Copy the string to the struct

            // Set the starting index of the next column within the raw input
            start = start + width + 1;
        }
        row++;
    }
    math->row_count = row;
}

long long do_op(long long a, long long b, char symbol) {
    if (symbol == '*') {
        if (a == 0) a++;
        return a * b;
    }
    return a + b;
}

int str_to_int(char str[5]) {
    while (*str == '.') str++;
    return atoi(str);
}

long long solve_part1(struct Math *math) {
    long long total = 0;
    // Go through on a column by column basis instead of rows
    for (size_t col = 0; col < math->col_count; col++) {
        long long col_total = 0;
        // Process each row entry for the current column
        for (size_t row = 0; row < math->row_count; row++) {
            // The number is stored as a string with dots as padding
            // So we do a converstion to int that discards the dots
            int temp_nr = str_to_int(math->numbers[row][col]);
            // Typecast to long long
            long long nr = (long long)temp_nr;
            // Grab the op symbol for the current column
            char op = math->symbols[col];

            // Perform the operation with the current number on the total
            col_total = do_op(col_total, nr, op);
        }
        // Add the column total to the overall total
        total += col_total;
    }
    return total;
}

long long solve_part2(struct Math *math) {
    long long total = 0;
    // Create a temporary array to hold the rotated column for any given number
    int temp[4][4]; 
    // We are going to go through column by column again
    for (size_t col = 0; col < math->col_count; col++) {
        long long col_total = 0;
        memset(temp, 0, sizeof(temp)); // Reset the array
        char col_symbol = math->symbols[col]; // Grab this column's symbol
        
        // Loop through the size of the column backwards
        // and grab every character from each row in that 
        // position within the column. Assemble a new int
        // from this and use it to perform operations.
        for (int col_char = math->col_len[col] - 1; col_char >= 0; col_char--) {
            // We create a new string to hold each new number
            char str[5];
            size_t idx = 0;
            // For every row in current column's character position
            for (size_t row = 0; row < math->row_count; row++) {
                // Grab the current character
                char ch = math->numbers[row][col][col_char];
                if (ch == '.') continue; // Skip any dots
                str[idx++] = ch; // Save the character to the new string
            }
            str[idx] = '\0'; // Add a terminating character
            long long nr = atoll(str); // convert to long long
            col_total = do_op(col_total, nr, col_symbol); // Perform the op
        }
        total += col_total;
    }
    return total;
}

int main() {
    timer_start();

    struct Math *math = malloc(sizeof(struct Math));
    if (!math) {
        fprintf(stderr, "failed to allocate memory for struct\n");
        return 1;
    }

    load_data(math);
    printf("%s\n", timer_checkpoint("Parsing"));

    long long part1 = solve_part1(math);
    printf("Part 1 Answer: %lld - %s\n", part1, timer_checkpoint("Part 1"));


    long long part2 = solve_part2(math);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    printf("%s\n", timer_total());
    free(math);
    return 0;
}
