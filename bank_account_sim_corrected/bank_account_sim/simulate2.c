#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"
#include "account.h"
#include <semaphore.h>

account_info *account_database;

pthread_mutex_t lock[4];
sem_t can_with[4]; 

int withdraw_amt = 2;
int dep_amt = 1;

void *lender(void *arg)
{
    int tid = (int)arg;
  int i;

  printf("I am lender # %d\n", tid);
  for (i=0; i<ROUND; i++) {
    account_deposit(account_database, tid, dep_amt);
  }
  
  return NULL;
}  

void *borrower(void *arg)
{
  int tid = (int)arg;
  int i;

  printf("I am borrower # %d\n", tid);
  for (i=0; i<ROUND; i++) {
    account_withdraw(account_database, tid, withdraw_amt);
  }
  
  return NULL;
}

void *thread(void *vargp);
void *thread_lender(void *vargp);
void *thread_borrower(void *vargp);

int main(int argc, char *argv[])
{

  int i, num_account;

  num_account = 4;
  pthread_t tid[8];

  printf("size of account_info is %d; size of int is %d; size of void pointer is %d\n", sizeof(account_info), sizeof(int), sizeof(void *));
  account_database = (account_info *)malloc(sizeof(account_info)*num_account);
  if (account_database == NULL) {
    fprintf(stderr, "Malloc unsuccessful\n");
    exit(0);
  }
  // Initialize account_database
  account_init(account_database, num_account);



  for (i=0; i<num_account; i++) {
    printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name, account_database[i].balance);
  }
  printf("\n");

  // Initialize mutex
  for(int i=0; i<4; i++){
    pthread_mutex_init(&lock[i], NULL);
    Sem_init(&can_with[i], 0, 1); /* mutex = 1 */
  }

  int num[4] = {0,1,2,3};

  // Create thread
  for(int i=0; i<8; i+=2){
    Pthread_create(&tid[i+1], NULL, thread_lender, &num[i/2]);
    Pthread_create(&tid[i], NULL, thread_borrower, &num[i/2]);
  }

  for(int i=0; i<8; i++){
    Pthread_join(tid[i], NULL);
  }

  printf("\n");
  for (i=0; i<num_account; i++) {
    printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name, account_database[i].balance);
  } 
  return 0;
}


void *thread_lender(void *vargp)
{
  int cnt = *((int *)vargp);
  pthread_mutex_lock(&lock[cnt]);   // lock aquire
  lender((void *)cnt);
  Sem_post(&can_with[cnt])
  pthread_mutex_unlock(&lock[cnt]);      // lock release
  return NULL;
}

void *thread_borrower(void *vargp)
{
  int cnt = *((int *)vargp);
  pthread_mutex_lock(&lock[cnt]);
  while(account_database[cnt].balance < ROUND*withdraw_amt){
    Sem_wait(&can_with[cnt]);
  }
  borrower((void *)cnt);
  pthread_mutex_unlock(&lock[cnt]);

  return NULL;
}
