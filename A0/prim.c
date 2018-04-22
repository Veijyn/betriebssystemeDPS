#include <stdio.h>

int main(){

int x, y;

for(x=2; x<=20; x++) // Da 1 keine Primzahl ist, beginne die Schleife mit 2.
	{
		int isPrim = 1; // Integer-Wert 1 bedeutet true
		
		for (y=2; y<x && isPrim; y++) // Da jede Zahl durch 1 teilbar ist, wird die 1 nicht überprüft und sofort mit 2 begonnen zu teilen
		{
				if((x%y)==0) //Sobald es eine Zahl gibt, die x ohne Rest teilt, ist x keine Primzahl mehr(...)
				{
				 isPrim = 0; //und deswegen wird der Integer-Wert auf 0 gesetzt, was gleichbedeutend für false ist
				}
		}
		if(isPrim) // bleibt der Wert 1, also true(...)
		{
				printf("%i,",x); // wird die Primzahl ausgegeben.
		}		
	}
	printf("\n");
	return 0; //Programm beenden		
}
