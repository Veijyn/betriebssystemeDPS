#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char *argv[]){

char command[255];
char param[255];
int paramcount;
int execlpfail;
int pid_fork;
int status;
int sleep_time;

if(argc == 1){
	sleep_time = 5;
}else if(argc == 3 && (strcmp(argv[1], "-n") == 0)){
	sleep_time = atoi(argv[2]);
}else{
	printf("Bitte keinen Parameter oder ein Intervall in der Form [-n Zahl] eingeben!\n");
	return 0;
}
printf("Zu beobachtendes Programm mit einem Parameter eingeben: ");
paramcount = scanf("%255s %255s", command, param);  
if(errno < 0){
	perror("scanf");
	exit(EXIT_FAILURE);
}
while(getchar() != '\n');
if(paramcount != 2){
	printf("Bitte einen Befehl und genau einen Parameter eingeben!\n");
}else{
	printf("Kommando: %s, Parameter: %s, Intervall: %d\n", command, param, sleep_time);
}
for(;;){
	pid_fork = fork();
	if(pid_fork == -1){
		perror("fork"),
		exit(EXIT_FAILURE);
	}	
	if(pid_fork == 0) {
		execlpfail = execlp(command, param, NULL);
		if(execlpfail == -1){
			if(errno < 0){
				perror("execlp");
				exit(EXIT_FAILURE);
			}	
		}
		_exit(status);
	}
	wait(&status);
	if(errno < 0){
		perror("wait");
		exit(EXIT_FAILURE);
	}
	printf("!<--------------------------------------------------------------------------------->!\n");
	sleep(sleep_time);
}
return 0;
}
