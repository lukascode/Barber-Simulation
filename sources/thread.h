#ifndef THREAD_H
#define THREAD_H

#include<pthread.h>

typedef struct Thread {

  int id;
  pthread_cond_t condition;
  pthread_mutex_t mutex;

} Thread;

//for static creating
#define THREAD_INITIALIZER { 0, PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER }

void init(Thread* thread,int id);
int lock(Thread* thread);
int unlock(Thread* thread);
int wait(Thread* thread);
int signal(Thread* thread);
void destroy(Thread* thread);

#endif /// THREAD_H //~
