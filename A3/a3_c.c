// Hier fertige a3_b.c herkopieren
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
				
		if(pthread_mutex_lock(&lock)!=0){
			perror("mutex_lock");
			exit(EXIT_FAILURE);
		}		
		int j;	
		for(j = 0; j < RECIPE_SIZE; j++){
			if(sem_wait(&chemicals[j].sem)==-1){
				perror("sem_wait");
				exit(EXIT_FAILURE);
			}
		}	
		
		if(pthread_mutex_unlock(&lock)!=0){
			perror("mutex_unlock");
			exit(EXIT_FAILURE);
		}
		int z;
		for(z = 0; z < RECIPE_SIZE; z++){
			sleep(chemicals[z].time_needed);
		}
		int v;
		for(v = 0; v < RECIPE_SIZE; v++){
				if(sem_post(&chemicals[v].sem)==-1){
				perror("sem_post");
				exit(EXIT_FAILURE);
			}		
		}
	}
	pthread_exit(NULL);
	// *** HIER EUER CODE ***
}
