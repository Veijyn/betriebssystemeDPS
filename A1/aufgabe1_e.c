#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

char command[255];
char param[255];
int paramcount;
int execlpfail;
int pid_fork;
int status;
int times_running;
int out_file_desc;

if(argc == 1){
	times_running = 5;
}else if(argc == 3 && (strcmp(argv[1], "-i") == 0)){
	times_running = atoi(argv[2]);
}else{
	printf("Bitte keinen Parameter oder die Anzahl der gewünschten Iterationen des Programms in der Form [-i Zahl] eingeben!\n");
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
	printf("Kommando: %s, Parameter: %s, Iterationen: %d\n", command, param, times_running);
}
out_file_desc = open("out.txt", O_CREAT | O_APPEND | O_WRONLY);
if(out_file_desc == -1){
	perror("open");
	exit(EXIT_FAILURE);
}
for(int i = 0; i < times_running; i++){
	if(i!=0){
		sleep(5);
	}
	pid_fork = fork();
	if(pid_fork == -1){
		perror("fork"),
		exit(EXIT_FAILURE);
	}	
	if(pid_fork == 0) {
		dup2(out_file_desc, 1);
		if(errno == -1){
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		execlpfail = execlp(command, param, NULL);
		if(execlpfail == -1){
			if(errno < 0){
				perror("execlp");
				exit(EXIT_FAILURE);
			}	
		}
		printf("!<--------------------------------------------------------------------------------->!\n");
		_exit(status);
	}
	wait(&status);
	if(errno < 0){
		perror("wait");
		exit(EXIT_FAILURE);
	}
	printf("%d. Iteration beendet!\n",i+1);
}
close(out_file_desc);
return 0;
}
