#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

extern struct timeval global_start;
extern struct timeval last_checkpoint;

void timer_start(void);
char* timer_checkpoint(const char *label);
char* timer_total(void);

#endif
