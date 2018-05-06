#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]){

char command[255];
char param[255];
int paramcount;
int execlpfail;
int pid_fork;
//int status;

printf("Zu beobachtendes Programm mit einem Parameter eingeben: ");
paramcount = scanf("%255s %255s", command, param);  
if(paramcount == -1){
	perror("scanf");
	exit(EXIT_FAILURE);
}
if(paramcount != 2){
	printf("Bitte einen Befehl und genau einen Parameter eingeben!\n");
}else{
	printf("Kommando: %s, Parameter: %s\n", command, param);
}
while(getchar() != '\n');
for(;;){
	pid_fork = fork();
	if(pid_fork == -1){
		perror("fork"),
		exit(EXIT_FAILURE);
	}	
	if(pid_fork == 0) {
		execlpfail = execlp(command, command, param, NULL);
		if(execlpfail == -1){
			if(errno < 0){
				perror("execlp");
				exit(EXIT_FAILURE);
			}	
		}
		//_exit(status);
	}	
	if(wait(NULL) == -1){
		perror("wait");
		exit(EXIT_FAILURE);
	}
	printf("-----------------------------------------------------\n");
	sleep(5);
}
return 0;
}
