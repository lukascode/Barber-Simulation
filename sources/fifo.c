
#include "fifo.h"
#include <stdlib.h>

typedef struct Node {

  //ponizsze funkcje nie zwalniaja pamieci
  //z obszaru na ktory wskazuje ten wskaznik
  Thread* thread;
  struct Node* next;

} Node;

typedef struct Fifo {

  Node* head;
  Node* tail;
  int size;

} Fifo;

static Fifo fifo; //global FIFO queue declaration

static void fifo_clear() {
  Node* fordel = fifo.head;
  Node* tmp;
  while(fordel) {
    tmp = fordel->next;
    destroy(fordel->thread);
    free(fordel);
    fordel = tmp;
  }
}

void fifo_init() {
  fifo.size = 0;
  fifo.head = 0;
  fifo.tail = 0;
  fifo_clear();
}

void fifo_push(Thread* thread) {
  Node* tmp = fifo.head;
  Node* newnode = (Node*)malloc(sizeof(Node));
  newnode->thread = thread;
  newnode->next = 0;
  if(!fifo.head) {
    fifo.head = newnode;
    fifo.tail = newnode;
  } else {
    fifo.tail->next = newnode;
    fifo.tail = newnode;
  }
  ++fifo.size;
}

Thread* fifo_top() {
  if(fifo.head)
  return fifo.head->thread;
  return 0;
}

Thread* fifo_pop() {
  if(fifo.head) {
  Thread* thread = fifo.head->thread;
  Node* next = fifo.head->next;
  free(fifo.head);
  fifo.head = next;
  --fifo.size;
  return thread;
  }
  return 0;
}

int fifo_isEmpty() {
  if(fifo.size == 0) return 1;
  return 0;
}

int fifo_size() {
  return fifo.size;
}

int* fifo_get_ids() {
  int* ids = (int*)malloc(fifo.size*sizeof(int));
  Node* tmp = fifo.head;
  int i;
  for(i=0; i<fifo.size; ++i){
    ids[i] = tmp->thread->id;
    tmp = tmp->next;
  }
  return ids;
}
