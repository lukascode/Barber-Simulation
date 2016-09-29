#ifndef FIFO_H
#define FIFO_H

#include "thread.h"


void fifo_init();
void fifo_push(Thread* thread);
Thread* fifo_top();
Thread* fifo_pop();
int fifo_isEmpty();
int fifo_size();
int* fifo_get_ids();




#endif /// FIFO_H //~
