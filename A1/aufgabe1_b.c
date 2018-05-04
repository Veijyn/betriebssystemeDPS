#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

char command[255];
char param[255];
int paramcount;
int execlpfail;

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
	printf("Kommando: %s, Parameter: %s\n", command, param);
}
execlpfail = execlp(command, param, NULL);
if(execlpfail == -1){
	if(errno < 0){
		perror("execlp");
		exit(EXIT_FAILURE);
	}
}



return 0;

}
