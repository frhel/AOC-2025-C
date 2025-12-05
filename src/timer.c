#define _POSIX_C_SOURCE 199309L
#include "timer.h"
#include <stdio.h>

struct timespec global_start;
struct timespec last_checkpoint;

void timer_start(void) {
    clock_gettime(CLOCK_MONOTONIC, &global_start);
    last_checkpoint = global_start;
}

char* timer_checkpoint(const char *label) {
    static char buffer[256];
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    // Time since last checkpoint in nanoseconds
    long long elapsed_ns = (now.tv_sec - last_checkpoint.tv_sec) * 1000000000LL +
                           (now.tv_nsec - last_checkpoint.tv_nsec);

    // Format the elapsed time
    char time_str[64];
    if (elapsed_ns >= 1000000000) {
        snprintf(time_str, sizeof(time_str), "%.3fs", elapsed_ns / 1000000000.0);
    } else if (elapsed_ns >= 1000000) {
        snprintf(time_str, sizeof(time_str), "%.3fms", elapsed_ns / 1000000.0);
    } else if (elapsed_ns >= 1000) {
        snprintf(time_str, sizeof(time_str), "%.3fμs", elapsed_ns / 1000.0);
    } else {
        snprintf(time_str, sizeof(time_str), "%lldns", elapsed_ns);
    }

    snprintf(buffer, sizeof(buffer), "%s: %s", label, time_str);

    last_checkpoint = now;
    return buffer;
}

char* timer_total(void) {
    static char buffer[128];
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    // Total time since start in nanoseconds
    long long total_ns = (now.tv_sec - global_start.tv_sec) * 1000000000LL +
                         (now.tv_nsec - global_start.tv_nsec);

    if (total_ns >= 1000000000) {
        snprintf(buffer, sizeof(buffer), "Total: %.3fs", total_ns / 1000000000.0);
    } else if (total_ns >= 1000000) {
        snprintf(buffer, sizeof(buffer), "Total: %.3fms", total_ns / 1000000.0);
    } else if (total_ns >= 1000) {
        snprintf(buffer, sizeof(buffer), "Total: %.3fμs", total_ns / 1000.0);
    } else {
        snprintf(buffer, sizeof(buffer), "Total: %lldns", total_ns);
    }

    return buffer;
}
