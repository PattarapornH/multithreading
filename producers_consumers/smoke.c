#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

sem_t smoker_waits[3];
sem_t agent;

typedef void *(*smokers_t)(void *);

/**
 * Smoker 0 has an infinite supply of paper
 */
void* smoker_0(void* arg)
{
	while (true)
	{
		sleep(1);
		// nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker zero waiting to smoke (has paper)\n");

		sem_wait(&smoker_waits[0]);
		printf("Smoker zero received matchges and tobacco. Now making cigarette.\n");
		sem_post(&agent);
	}

	return NULL;
}

/**
 * Smoker 1 has an infinite supply of matches
 */
void* smoker_1(void* arg)
{
	while (true)
	{
		sleep(1);
		// nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker one waiting to smoke (has matches)\n");

		sem_wait(&smoker_waits[1]);
		printf("Smoker zero received matches and tobacco. Now making cigarette.\n");
		sem_post(&agent);
	}

	return NULL;
}

/**
 * Smoker 2 has an infinite supply of tobacco
 */
void* smoker_2(void* arg)
{
	while (true)
	{
		sleep(1);
		// nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker two waiting to smoke (has tobacco)\n");

		sem_wait(&smoker_waits[2]);
		printf("Smoker zero received matches and tobacco. Now making cigarette.\n");
		sem_post(&agent);
	}

	return NULL;
}

/**
 * The main thread handles the agent's abritatin of items.
 */
int main(int argc, char* arvg[])
{
	// Seed our random nmber since we will be using random numbers
	srand(time(NULL));

	// Initalize our agent semaphore
	sem_init(&agent, 0, 1);

	// Setup our smoker threads and function refs
	pthread_t smoker_threads[3];
	smokers_t smoker_functions[3] = { smoker_0, smoker_1, smoker_2 };

	// Create the smoker threads and initalize the semaphores
	for (int i = 0; i < 3; ++i)
	{
		sem_init(&smoker_waits[i], 0, 0);

		if(pthread_create(&smoker_threads[i], NULL, smoker_functions[i], NULL) == EAGAIN)
		{
			perror("Insufficent resources to create thread\n");
			return 0;
		}
	}

	// Let the agent begin distributing his items
	while (true)
	{
		int r = rand()%2;
    		// Lock the agent once he's
		sem_wait(&agent);

		// Distribute two items for one of the smokers
		sem_post(&smoker_waits[r]);
	}

	// Join all of the smoker threads on exit
	for (int i = 0; i < 3; ++i)
	{
		pthread_join(smoker_threads[i], NULL);
	}
}