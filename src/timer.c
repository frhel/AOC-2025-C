#include "timer.h"
#include <stdio.h>

struct timeval global_start;
struct timeval last_checkpoint;

void timer_start(void) {
    gettimeofday(&global_start, NULL);
    last_checkpoint = global_start;
}

char* timer_checkpoint(const char *label) {
    static char buffer[256];
    struct timeval now;
    gettimeofday(&now, NULL);

    // Time since last checkpoint
    long seconds = now.tv_sec - last_checkpoint.tv_sec;
    long microseconds = now.tv_usec - last_checkpoint.tv_usec;

    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }

    long elapsed_us = seconds * 1000000 + microseconds;

    // Format the elapsed time
    char time_str[64];
    if (elapsed_us >= 1000000) {
        snprintf(time_str, sizeof(time_str), "%.3fs", elapsed_us / 1000000.0);
    } else if (elapsed_us >= 1000) {
        snprintf(time_str, sizeof(time_str), "%.3fms", elapsed_us / 1000.0);
    } else {
        snprintf(time_str, sizeof(time_str), "%ldμs", elapsed_us);
    }

    snprintf(buffer, sizeof(buffer), "%s: %s", label, time_str);

    last_checkpoint = now;
    return buffer;
}

char* timer_total(void) {
    static char buffer[128];
    struct timeval now;
    gettimeofday(&now, NULL);

    // Total time since start
    long seconds = now.tv_sec - global_start.tv_sec;
    long microseconds = now.tv_usec - global_start.tv_usec;

    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }

    long total_us = seconds * 1000000 + microseconds;

    if (total_us >= 1000000) {
        snprintf(buffer, sizeof(buffer), "Total: %.3fs", total_us / 1000000.0);
    } else if (total_us >= 1000) {
        snprintf(buffer, sizeof(buffer), "Total: %.3fms", total_us / 1000.0);
    } else {
        snprintf(buffer, sizeof(buffer), "Total: %ldμs", total_us);
    }

    return buffer;
}
