#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

char command[255];
char param[255];
int paramcount;

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
return 0;
}
