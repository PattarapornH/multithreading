/* $begin sbufc */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "common_threads.h"
#include "smoker.h"


void init_semaphore(table *t){
  Sem_init(&t->smoker_paper, 0, 0); /* mutex = 1 */
  Sem_init(&t->smoker_tobac, 0, 0); /* mutex = 1 */
  Sem_init(&t->smoker_match, 0, 0); /* mutex = 1 */
  Sem_init(&t->agent, 0, 0); /* mutex = 1 */
}

void *thread_agent_func(table *t){
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
      V(&t->smoker_match);
    }
    else if(random_number == 2){            // rand = 2 --> place tobacco and match
      printf("Agent places tobacco and match\n");
      sleep(1);
      V(&t->smoker_paper);
    }
    else if(random_number == 3){            // rand = 3 --> place paper and match
      printf("Agent places paper and match\n");
      sleep(1);
      V(&t->smoker_tobac);
    }
    P(&t->agent)
  }
  return NULL;
}

void *thread_smoker_tobac_func(table *t){
  while(1){
    P(&t->smoker_tobac);
    printf("Tobacco smoker smokes\n");
    sleep(1);
    V(&t->agent);
  }
  return NULL;
}

void *thread_smoker_paper_func(table *t){
  while(1){
    P(&t->smoker_paper);
    printf("Paper smoker smokes\n");
    sleep(1);
    V(&t->agent);
  }
  return NULL;
}

void *thread_smoker_match_func(table *t){
  while(1){
    P(&t->smoker_match);
    printf("Match smoker smokes\n");
    sleep(1);
    V(&t->agent);
  }
  return NULL;
}
