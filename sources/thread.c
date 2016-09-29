
#include "thread.h"


void init(Thread* thread,int id) {
  thread->id = id;
  pthread_mutex_init(&thread->mutex,NULL);
  pthread_cond_init(&thread->condition,NULL);
}
int lock(Thread* thread) {
  int ret = pthread_mutex_lock(&thread->mutex);
  return ret;
}
int unlock(Thread* thread) {
  int ret = pthread_mutex_unlock(&thread->mutex);
  return ret;
}
int wait(Thread* thread) {
  int ret = pthread_cond_wait(&thread->condition,&thread->mutex);
  return ret;
}
int signal(Thread* thread) {
  int ret = pthread_cond_signal(&thread->condition);
  return ret;
}
void destroy(Thread* thread) {
  pthread_mutex_destroy(&thread->mutex);
  pthread_cond_destroy(&thread->condition);
}
