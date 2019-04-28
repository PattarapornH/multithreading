#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "common_threads.h"
#include "smoker.h"

table *shared_table;


int main(int argc, char *argv[])
{
  pthread_t thread_match, thread_tobac, thread_paper, thread_agent;

  shared_table = malloc(sizeof(table));
  if (shared_table == NULL) {
    fprintf(stderr, "Malloc unsuccessful\n");
    exit(0);
  }

  init_semaphore(shared_table);

  Pthread_create(&thread_agent, NULL, thread_agent_func, shared_table);
  Pthread_create(&thread_paper, NULL, thread_smoker_paper_func, shared_table);
  Pthread_create(&thread_tobac, NULL, thread_smoker_tobac_func, shared_table);
  Pthread_create(&thread_match, NULL, thread_smoker_match_func, shared_table);

  Pthread_join(thread_paper, NULL);
  Pthread_join(thread_tobac, NULL);
  Pthread_join(thread_match, NULL);
  Pthread_join(thread_agent, NULL);

  return 0;
}