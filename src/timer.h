#ifndef TIMER_H
#define TIMER_H

#include <time.h>

extern struct timespec global_start;
extern struct timespec last_checkpoint;

void timer_start(void);
char* timer_checkpoint(const char *label);
char* timer_total(void);

#endif
