#include <stdio.h>
#include <stdlib.h>
// Zusätzliche Header-Dateien hier!
#include <semaphore.h>
#include <errno.h>
#include "vorgabe.h"

void init_semaphores()
{	
	int i;
	for(i = 0; i < CHEMICAL_NUM; i++){
		if(sem_init(&(chemicals[i].sem), 0, CHEMICAL_NUM)==-1){
			perror("sem_init");
			exit(EXIT_FAILURE);		
		}
	}
	// *** HIER EUER CODE ***
}

void destroy_semaphores()
{
	int i;
	for(i = 0; i < CHEMICAL_NUM; i++){
		if(sem_destroy(&(chemicals[i].sem))==-1){
			perror("sem_destroy");
			exit(EXIT_FAILURE);
		}
	}
	// *** HIER EUER CODE ***
}
