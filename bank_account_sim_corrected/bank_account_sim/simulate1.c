#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"
#include "account.h"
#include <semaphore.h>

account_info *account_database;

void *lender(void *arg)
{
    int tid = (int)arg;
  int i;

  printf("I am lender # %d\n", tid);
  for (i=0; i<ROUND; i++) {
    account_deposit(account_database, tid, 2);
  }
  
  return NULL;
}  

void *borrower(void *arg)
{
  int tid = (int)arg;
  int i;

  printf("I am borrower # %d\n", tid);
  for (i=0; i<ROUND; i++) {
    account_withdraw(account_database, tid, 1);
  }
  
  return NULL;
}

void *thread(void *vargp);

int main(int argc, char *argv[])
{

  int i, num_account;

  num_account = 4;
  pthread_t tid[4];

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

  int num[4] = {0,1,2,3};

  // Create thread
  for(int i=0; i<4; i++){
    Pthread_create(&tid[i], NULL, thread, &num[i]);
  }

  for(int i=0; i<4; i++){
    Pthread_join(tid[i], NULL);
  }

  printf("\n");
  for (i=0; i<num_account; i++) {
    printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name, account_database[i].balance);
  } 
  return 0;
}


void *thread(void *vargp)
{
  int cnt = *((int *)vargp);
  borrower((void *)cnt);
  lender((void *)cnt);
  return NULL;
}
