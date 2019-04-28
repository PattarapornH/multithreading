#ifndef __SBUF_H__
#define __SBUF_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* $begin sbuft */
typedef struct {
  sem_t smoker_match;       /* Protects accesses to buf */
  sem_t smoker_paper;       /* Counts available slots */
  sem_t smoker_tobac;       /* Counts available items */
  sem_t agent;
} table;

void init_semaphore(table *t);
void *thread_agent_func(table *t);
void *thread_smoker_paper_func(table *t);
void *thread_smoker_tobac_func(table *t);
void *thread_smoker_match_func(table *t);

#endif /* __TABLE_H__ */


