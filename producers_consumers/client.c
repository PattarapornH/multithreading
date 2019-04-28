#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "common_threads.h"
#include "sbuf.h"

sbuf_t *shared_data;

sem_t lock;
sem_t smoker_tobac; 
sem_t smoker_paper; 
sem_t smoker_match; 
sem_t agent; 

void *thread_agent_func(void *arg);
void *thread_smoker_paper_func(void *arg);
void *thread_smoker_tobac_func(void *arg);
void *thread_smoker_match_func(void *arg);

void *producer(void *arg)
{
  char *msg = arg;
  int item = 1;
  while (1) {
    sbuf_insert(shared_data, item, msg);
    item++;
    if (item == 6) item = 1;
  }
  
  return NULL;
}  

void *consumer(void *arg)
{
  char *msg = arg;
  int item;
  while (1) {
    item = sbuf_remove(shared_data, msg);
  }
  
  return NULL;
}

int main(int argc, char *argv[])
{

  pthread_t thread_match, thread_tobac, thread_paper, thread_agent;

  printf("size of sd = %lu\n", sizeof(sbuf_t));
  shared_data = malloc(sizeof(sbuf_t));
  if (shared_data == NULL) {
    fprintf(stderr, "Malloc unsuccessful\n");
    exit(0);
  }
  Sem_init(&lock,0,1)
  Sem_init(&smoker_paper, 0, 0); /* mutex = 1 */
  Sem_init(&smoker_tobac, 0, 0); /* mutex = 1 */
  Sem_init(&smoker_match, 0, 0); /* mutex = 1 */
  Sem_init(&agent, 0, 0); /* mutex = 1 */

  printf("create thread\n");

  Pthread_create(&thread_agent, NULL, thread_agent_func, NULL);
  Pthread_create(&thread_paper, NULL, thread_smoker_paper_func, NULL);
  Pthread_create(&thread_tobac, NULL, thread_smoker_tobac_func, NULL);
  Pthread_create(&thread_match, NULL, thread_smoker_match_func, NULL);

  // // Initialize buffer of size 5
  // sbuf_init(shared_data, 5);
  
  // // join waits for the threads to finish
  Pthread_join(thread_paper, NULL);
  Pthread_join(thread_tobac, NULL);
  Pthread_join(thread_match, NULL);
  Pthread_join(thread_agent, NULL);

  return 0;
}

void *thread_agent_func(void *arg){
  while(1){
    // RANDOM
    int random_number = rand() % 4;
    if(random_number < 1)
    {
      random_number += 1;
    }
    if(random_number == 1){                 // rand = 1 --> place tobacco and paper
      printf("Agent places paper and tobacco\n");
      sleep(1);
      V(&smoker_match);
    }
    else if(random_number == 2){            // rand = 2 --> place tobacco and match
      printf("Agent places tobacco and match\n");
      sleep(1);
      V(&smoker_paper);
    }
    else if(random_number == 3){            // rand = 3 --> place paper and match
      printf("Agent places paper and match\n");
      sleep(1);
      V(&smoker_tobac);
    }
    P(&agent)

  }
  return NULL;
}

void *thread_smoker_tobac_func(void *arg){
  while(1){

    P(&smoker_tobac);
    printf("Tobacco smoker smokes\n");
    sleep(1);
    V(&agent);
  }
  return NULL;
}

void *thread_smoker_paper_func(void *arg){
  while(1){
    P(&smoker_paper);
    printf("Paper smoker smokes\n");
    sleep(1);
    V(&agent);
  }
  return NULL;
}

void *thread_smoker_match_func(void *arg){
  while(1){
    P(&smoker_match);
    printf("Match smoker smokes\n");
    sleep(1);
    V(&agent);
  }
  return NULL;
}