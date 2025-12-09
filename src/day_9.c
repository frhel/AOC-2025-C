#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include <stdbool.h>

// Problem description: https://adventofcode.com/2025/day/9

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point ul;
    struct Point ur;
    struct Point dl;
    struct Point dr;
};


int load_data(struct Point points[]) {
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        if (sscanf(line, "%d,%d", &points[count].x, &points[count].y) == 2) {
            count++;
        }

    }

    return count;
}


long long calc_area(struct Point p1, struct Point p2) {
    long long width = (p1.x < p2.x) ? (long long)(p2.x - p1.x + 1) : (long long)(p1.x - p2.x + 1);
    long long height = (p1.y < p2.y) ? (long long)(p2.y - p1.y + 1) : (long long)(p1.y - p2.y + 1);
    return width * height;
}

long long solve_part1(struct Point points[], int num_points) {
    long long max = 0;
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            long long area = calc_area(points[i], points[j]);
            // printf("Checking points (%d,%d) and (%d,%d): Area = %lld\n", points[i].x, points[i].y, points[j].x, points[j].y, area);
            if (area >= max) {
                max = area;
            }
        }
    }
    return max;
}


void get_rect_corners(struct Rect *rect, struct Point *p1, struct Point *p2) {
    // Determine rectangle corners
    rect->ul.x = (p1->x < p2->x) ? p1->x : p2->x;
    rect->ul.y = (p1->y < p2->y) ? p1->y : p2->y;
    rect->ur.x = (p1->x > p2->x) ? p1->x : p2->x;
    rect->ur.y = (p1->y < p2->y) ? p1->y : p2->y;
    rect->dl.x = (p1->x < p2->x) ? p1->x : p2->x;
    rect->dl.y = (p1->y > p2->y) ? p1->y : p2->y;
    rect->dr.x = (p1->x > p2->x) ? p1->x : p2->x;
    rect->dr.y = (p1->y > p2->y) ? p1->y : p2->y;
}

// Check if point is on line segment
bool point_on_segment(struct Point p, struct Point a, struct Point b) {
    // Check if p is on the line segment from a to b
    int min_x = (a.x < b.x) ? a.x : b.x;
    int max_x = (a.x > b.x) ? a.x : b.x;
    int min_y = (a.y < b.y) ? a.y : b.y;
    int max_y = (a.y > b.y) ? a.y : b.y;

    // Point must be within bounding box
    if (p.x < min_x || p.x > max_x || p.y < min_y || p.y > max_y) {
        return false;
    }

    // Check if point is collinear using cross product
    long long cross = (long long)(p.y - a.y) * (b.x - a.x) - (long long)(p.x - a.x) * (b.y - a.y);
    return cross == 0;
}

// Ray casting algorithm to check if a point is inside a polygon
bool point_in_polygon(struct Point test, struct Point polygon[], int num_points) {
    // First check if point is a vertex or on an edge
    for (int i = 0; i < num_points; i++) {
        // Check if it's a vertex
        if (test.x == polygon[i].x && test.y == polygon[i].y) {
            return true;
        }

        // Check if it's on an edge
        int j = (i + 1) % num_points;
        if (point_on_segment(test, polygon[i], polygon[j])) {
            return true;
        }
    }

    // Standard ray casting for interior points
    bool inside = false;
    int j = num_points - 1;

    for (int i = 0; i < num_points; i++) {
        // Check if horizontal ray from test point crosses edge from polygon[i] to polygon[j]
        if (((polygon[i].y > test.y) != (polygon[j].y > test.y)) &&
            (test.x < (polygon[j].x - polygon[i].x) * (test.y - polygon[i].y) /
                      (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
        j = i;
    }
    return inside;
}

// Check if a line segment intersects another line segment
bool segments_intersect(struct Point p1, struct Point p2, struct Point p3, struct Point p4) {
    // Calculate the direction of the cross products
    long long d1 = (long long)(p4.x - p3.x) * (p1.y - p3.y) - (long long)(p4.y - p3.y) * (p1.x - p3.x);
    long long d2 = (long long)(p4.x - p3.x) * (p2.y - p3.y) - (long long)(p4.y - p3.y) * (p2.x - p3.x);
    long long d3 = (long long)(p2.x - p1.x) * (p3.y - p1.y) - (long long)(p2.y - p1.y) * (p3.x - p1.x);
    long long d4 = (long long)(p2.x - p1.x) * (p4.y - p1.y) - (long long)(p2.y - p1.y) * (p4.x - p1.x);

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return true;
    }

    return false;
}

// Check if rectangle is fully contained within polygon
bool rect_in_polygon(struct Rect *rect, struct Point polygon[], int num_points) {
    // First check: all 4 corners must be inside the polygon
    if (!point_in_polygon(rect->ul, polygon, num_points)) return false;
    if (!point_in_polygon(rect->ur, polygon, num_points)) return false;
    if (!point_in_polygon(rect->dl, polygon, num_points)) return false;
    if (!point_in_polygon(rect->dr, polygon, num_points)) return false;

    // Second check: no polygon edges should intersect rectangle edges
    // Rectangle edges
    struct Point rect_edges[4][2] = {
        {rect->ul, rect->ur},  // Top edge
        {rect->ur, rect->dr},  // Right edge
        {rect->dr, rect->dl},  // Bottom edge
        {rect->dl, rect->ul}   // Left edge
    };

    // Check each polygon edge against each rectangle edge
    for (int i = 0; i < num_points; i++) {
        int j = (i + 1) % num_points;
        struct Point poly_p1 = polygon[i];
        struct Point poly_p2 = polygon[j];

        for (int r = 0; r < 4; r++) {
            if (segments_intersect(poly_p1, poly_p2, rect_edges[r][0], rect_edges[r][1])) {
                return false;
            }
        }
    }

    return true;
}

long long solve_part2(struct Point points[], int num_points) {
    long long max = 0;

    // Try all possible rectangles formed by pairs of points
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            struct Rect rect;
            get_rect_corners(&rect, &points[i], &points[j]);

            // Check if this rectangle is fully inside the polygon
            if (rect_in_polygon(&rect, points, num_points)) {
                long long area = calc_area(points[i], points[j]);
                if (area > max) {
                    max = area;
                }
            }
        }
    }

    return max;
}

void print_grid(char grid_cells[100000][100000], int max_x, int max_y) {
    for (int y = 0; y <= max_y; y++) {
        for (int x = 0; x <= max_x; x++) {
            putchar(grid_cells[y][x]);
        }
        putchar('\n');
    }
}

int main() {
    timer_start();

    struct Point points[1024];
    int num_points = load_data(points);
    printf("%s\n", timer_checkpoint("Parsing"));
    
    // sort_points_by_row(points, num_points);
    // sort_points_by_col(points, num_points);

    // for (int i = 0; i < num_points; i++) {
    //     printf("Point %d: (%d,%d)\n", i, points[i].x, points[i].y);
    // }

    long long part1 = solve_part1(points, num_points);
    printf("Part 1 Answer: %lld - %s\n", part1, timer_checkpoint("Part 1"));

    long long part2 = solve_part2(points, num_points);
    printf("Part 2 Answer: %lld - %s\n", part2, timer_checkpoint("Part 2"));

    // printf("%s\n", timer_total());
    return 0;
}
