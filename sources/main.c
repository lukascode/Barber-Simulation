#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include "fifo.h"
#include <unistd.h>
#include<string.h>

#define CHAIRS_NUMBER 5
#define MAX_CUSTOMERS_NUMBER 100

//random time for customer's coming (s)
const int tleft = 3,tright = 6;
//random number of customers
const int cnleft = 1, cnright = 5;

Thread barber_thread = THREAD_INITIALIZER;
int in = -1;
int sleeping = 0;
sem_t comunication;
sem_t value_monitor;
int Res[MAX_CUSTOMERS_NUMBER]; int res_index = -1;


void* barber(void* ptr);
void* customer(void* customer_thread);
void* output(void* ptr);
void* output_debug(void* ptr);


int
main(int argc,char* argv[]) {

  void* (*out)(void*);
  if(argc > 2) {
    printf("Incorrect number of parameters\n");
    exit(1);
  } else if(argc == 2) {
      if(!strcmp(argv[1]+1,"debug")) {
         out = &output_debug;
      } else {
        printf("undefined parameter: \"%s\"\n",argv[1]);
        exit(2);
      }
  } else out = &output;


  srand(time(NULL));
  sem_init(&comunication,0,0);
  sem_init(&value_monitor,0,1);
  fifo_init();

  //thread for stdout output
  pthread_t toutput;
  pthread_create(&toutput,NULL,out,NULL);

  //thread for barber
  pthread_t tbarber;
  pthread_create(&tbarber,NULL,barber,NULL);

  //adding to the queue at a random time
  //random number of clients and creating threads for them
  int i;
  pthread_t tcustomers[MAX_CUSTOMERS_NUMBER];
  int nextid = 0; int nextindex = 0;
    do {
    sleep((rand()%(tright-tleft+1)+tleft));
    int cust_number = rand()%(cnright-cnleft+1)+cnleft;
    if((nextindex + cust_number) > MAX_CUSTOMERS_NUMBER) break;

    for(i=0; i<cust_number; ++i) {
      if(fifo_size() == CHAIRS_NUMBER) { //client resign 
          Res[++res_index] = nextid;
          sem_post(&value_monitor);
      } else {
        Thread* thread = (Thread*)malloc(sizeof(Thread));
        init(thread,nextid);
        fifo_push(thread);
        sem_post(&value_monitor);
        pthread_create(&tcustomers[nextindex],NULL,customer,(void*)thread);
      }
      sleep(1);
      ++nextid; ++nextindex;
    }
  } while(1==1);

  //oczekiwanie na zakonczenie watku fryzjera
  pthread_join(tbarber,NULL);


  return 0;
}

void* barber(void* ptr) {
  while(1) {
    lock(&barber_thread);
    if(!fifo_isEmpty()) {
      Thread* customer_t = fifo_top();
      signal(customer_t); //zawolaj klienta do strzyzenia
      sem_post(&comunication); //zglos gotowosc do strzyzenia
        //tutaj fryzjer strzyze klienta
        usleep(4000*1000);
      sem_post(&comunication); //zglos koniec strzyzenia
    } else {
      in = -1;
      sem_post(&value_monitor); //poinformuj o zmianie wartosci
      sleeping = 1;  wait(&barber_thread); sleeping = 0;//spij
      sem_post(&comunication); //zglos gotowosc do strzyzenia
        //tutaj fryzjer strzyze klienta
        usleep(4000*1000);
      sem_post(&comunication); //zglos koniec strzyzenia
    }
    unlock(&barber_thread);
  }
  return 0;
}

void* customer(void* customer_thread) {
  Thread* customer_t = (Thread*)customer_thread;
  lock(customer_t);
   //--------------------------------------
   if(sleeping == 1) {
     lock(&barber_thread);
     signal(&barber_thread); //obudz fryzjera
     unlock(&barber_thread);
   } else wait(customer_t); //czekaj na swoja kolej

   //--------------------------------------
    fifo_pop(); //wyjdz z kolejki
    in = customer_t->id;
    sem_post(&value_monitor); //poinformuj o zmianie wartosci

    sem_wait(&comunication); //czekaj na gotowosc strzyzenia przez fryzjera
      //tutaj fryzjer strzyze klienta
    sem_wait(&comunication); //czekaj na koniec strzyzenia
  unlock(customer_t);

  free(customer_t);

  return 0;
}

void* output(void* ptr) {
      while(1) {
      sem_wait(&value_monitor);
      system("clear");
      if(in == -1)
      printf("Res:%d  WRomm: %d/%d  [in: pusto]\n",res_index+1,fifo_size(),CHAIRS_NUMBER);
      else
      printf("Res:%d  WRomm: %d/%d  [in: %d]\n",res_index+1,fifo_size(),CHAIRS_NUMBER,in);
    }
}

void* output_debug(void* ptr) {
  while(1) {
    sem_wait(&value_monitor);
    system("clear");
    if(in == -1)
    printf("Res:%d  WRomm: %d/%d  [in: pusto]\n",res_index+1,fifo_size(),CHAIRS_NUMBER);
    else
    printf("Res:%d  WRomm: %d/%d  [in: %d]\n",res_index+1,fifo_size(),CHAIRS_NUMBER,in);

    int i;
    printf("CZEKAJACY: ");
    int* waiting_ids = fifo_get_ids();
    for(i=fifo_size()-1; i>=0; --i) {
      printf("%d ",waiting_ids[i]);
    }
    free(waiting_ids);
    printf("\n");

    printf("ZREZYGNOWALI: ");
    for(i=0; i<=res_index; ++i) printf("%d ",Res[i]);
    printf("\n");
  }
}
