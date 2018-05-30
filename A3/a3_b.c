#include <stdlib.h>
#include <stdio.h>
// Zusätzliche Header-Dateien hier!
#include "vorgabe.h"
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

void *work(void *arg)
{
	int my_num = *((int *)arg);

	printf("[Ch%d] Ich bin bereit zu arbeiten!\n", my_num);

	int i;
	for(i = 0; i < RECIPES_PER_CHEMIST; i++){
			
		get_recipe((chemical_t**) chemicals);
			
		int j;	
		for(j = 0; j < RECIPE_SIZE; j++){			
			if(sem_wait(&chemicals[j].sem)==-1){
				perror("sem_wait");
				exit(EXIT_FAILURE);
			}
				sleep(chemicals[j].time_needed);
		}
		int z;	
		for(z = 0; z < RECIPE_SIZE; z++){	
			if(sem_post(&chemicals[z].sem)==-1){
			perror("sem_post");
			exit(EXIT_FAILURE);
			}		
		}
	}
	pthread_exit(NULL);
	// *** HIER EUER CODE ***
}
