#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) // argc=Anzahl der Parameter,argv=einzelne Parameter in einem char-array
{
	int b, x, y;
	
	if(argc != 2) //wenn weniger als ein oder mehr als ein Wert in die Shell eingegeben wird, wird ein Fehlertext ausgegeben (argc ist immer mindestens 1, da der Parameter immer den Dateinamen in argv[0] enthält)
	{
		printf("Fehler: Keine Obergrenze für Primzahlen übergeben\n");
		return 0; //Programm beenden
	}
		
	b = atoi(argv[1]); //atoi ist eine Funtion, die einen ASCII-Code in einen Integer umwandelt. In dem Fall wird der ASCII-Code, der sich hinter argv[1] befindet, der also in die Shell eingegeben wurde, in einen Integer-Wert umgewandelt.
			
	for(x=2; x<=b; x++) //Da 1 keine Primzahl ist, beginne die Schleife mit 2. Die Schleife läuft anschließend bis sie den Wert b, der in die Shell eingegeben wurde, erreicht.
	{
		int isPrim = 1; // Integer-Wert 1 bedeutet true
		
		for (y=2; y<x && isPrim; y++) //Da jede Zahl durch 1 teilbar ist, wird die 1 nicht überprüft und sofort mit 2 begonnen zu teilen
		{
				if((x%y)==0) //Sobald es eine Zahl gibt, die x ohne Rest teilt, ist x keine Primzahl mehr
				{
				 isPrim = 0; //und deswegen wird der Integer-Wert auf 0 gesetzt, was gleichbedeutend für false ist
				}
		}
		if(isPrim) //bleibt der Wert 1, also true(...)
		{
				printf("%i,",x); //wird die Primzahl ausgegeben.
		}
	}
	printf("\n");
	return 0; //Programm beenden	
}
