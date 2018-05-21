/* Matrizenmultiplikation - einfache Parallelisierung.
/  Es sollen so viele Faeden (POSIX-Threads) instanziiert werden, wie es
/  Zeilen in der Eingabe-Matrix gibt. Da die pthread_create-Funktion (siehe pthread_create(3)) nur einen
/  Parameter fuer die Uebergabe von Argumenten an die Thread-Funktion vorsieht, werden die Argumente
/  in der Praxis ueblicherweise mittels einer Strukturvariable (struct) uebergeben. Da jedoch unsere
/  Implementierung fuer die Aufgabe a2) mit nur einem Argument (naemlich der Matrix-Reihe "row") auskommt, koennen wir uns
/  das Struct sparen, muessen aber das Argument von Datentyp int in einen void-Zeiger und umgekehrt
/  ueberfuehren. Um Plattformunabhaengigkeit zu gewaehrleisten und Fehler zu vermeiden, sollte dazu der
/  Datentyp intptr_t verwendet werden, hier ein Beispiel:
/  void* argvoidptr = (void*)(intptr_t) argint
/
/  Zum Nachlesen: pthread_create(3), pthread_join(3), pthread_exit(3).
/
/  Um das Programm mit GCC zu kompilieren: "gcc -std=c11 -Wall -pthread -march=native -O3 a2_parallel.c -o a2p".
/  Um das Programm mit Valgrind auf Speicherfehler zu ueberpruefen: "valgrind --leak-check=full -v ./a2p".
/   |_ Hier interessant sind definitely, indirectly und (eventuell) possibly lost. Still reachable koennt Ihr ignorieren.
/
*/

#include <stdio.h>       // u.a. fuer printf()
#include <stdlib.h>      // u.a. fuer rand()
#include <time.h>        // u.a. fuer time()
#include <sys/time.h>    // u.a. fuer gettimeofday()
#include <sys/sysinfo.h> // u.a. fuer get_nprocs()
#include <pthread.h>     // u.a. fuer pthread_create()
#include <stdint.h>      // u.a. fuer intptr_t

// Matrizen nach der Berechnung mit ausgeben? 1 ja, 0 nein.
#define PRINT_MATRICES 0

// Anzahl der Matrix-Zeilen. Da wir von quadratischen Matrizen ausgehen,
// entspricht das der Anzahl der Matrix-Spalten
#define ARRAY_SIZE 100

// Zudem definieren wir uns einen neuen Datentyp matrix_t.
typedef int matrix_t[ARRAY_SIZE * ARRAY_SIZE];
matrix_t MA,MB,MBT,MC;


/* *** Dein CODE hier ***
/  Deklariere ein Feld (array) fuer die Verwaltung von Threads. Dimensioniere es so,
/  dass ein Thread pro Matrix-Zeile gespeichert werden kann.
*/

pthread_t array_threads[ARRAY_SIZE];

/* Funktion fuer die zufallsbasierte Initialisierung einer Matrix */
void init_matrix(matrix_t matrix) {
   int row,column, size = ARRAY_SIZE;
   for (row=0; row < size; row++) {
      for (column=0; column < size; column++) {
         (matrix[row * size + column]) = rand() % 10 + 1;  // Wir nutzen den Modulo-Operator, um den Wert der Zufallszahlen einzugrenzen.
      }                                                    // Hier werden Zahlen zwischen 1 und 10 "gewuerfelt".
   }
}

/* Funktion fuer das Ausgeben einer Matrix */
void print_matrix(matrix_t matrix) {
   int row, column, size = ARRAY_SIZE;
   printf("MATRIX:\n");
   for(row = 0; row < size; row ++) {
      for (column = 0; column < size; column++) {
         printf("%d ", (matrix[row * size + column]));
      }
   printf("\n");
   }
}

/* Die Thread-Funktion zur Multiplikation einer MA-Zeile mit MB-Spalten (MBT-Zeilen).
/  Die Ergebnisse der Multiplikationen werden in der Zielmatrix unter MC[Zeile] abgelegt.
*/
void* mmult(void* r){
	int row = (intptr_t) r;		     // Matrix-Zeile
   int index;                                // Bestimmt die Position der zu multiplizierenden Faktoren
   int column, tmp, size = ARRAY_SIZE;
   for(column = 0; column < size; column++){ // Spalte der MB-Matrix (d.h. Zeile der MBT-Matrix)
      tmp = 0;
      for(index = 0; index < size; index++){
		 
         tmp += MA[row * size + index]  *  MBT[column * size + index];
        
      }
      MC[row* size + column] = tmp;
   }
   /* *** Dein CODE hier ***
   /  Beende diesen Faden nach getaner Arbeit.
   */
	pthread_exit(NULL); //keine Fehlerabfrage nötig, da immer erfolgreich
}

/* Einsprungpunkt fuer das Programm */
int main(void) {
   int size = ARRAY_SIZE, row, column;
   struct timeval start, end;
   double exectime;

   printf("Es werden (#%d) Faeden instanziiert. Dieses System verfuegt ueber (#%d) Prozessoren.\n", size, get_nprocs());

   // Initialisiere den Zufallszahl-Generator (seed), um Zufallszahlen generieren zu koennen.
   // Die Initialisierung erfolgt ueber einen Zeitstempel.
   srand(time(NULL));

   // Initialisiere zufallsbasiert die Matrizen MA und MB
   init_matrix(MA);
   init_matrix(MB);

   // Zeitmessung
   gettimeofday(&start, NULL);

   //Berechne die transponierte Matrix MBT aus MB
   for(row = 0; row < size; row++)
      for(column = 0; column < size; column++)
         MBT[row*size+column] = MB[column * size + row];

   /* *** Dein CODE hier ***
   /  Erstelle so viele Faeden, wie Zeilen in der Eingabematrix existieren. Uebergebe den Faeden den
   /  passenden Funktionszeiger. Ueberpruefe den Rueckgabewert.
   /  Gebe im Fehlerfall eine Fehlermeldung aus.
   */
   
	int i;
	for(i = 0; i < size; i++){
	if(pthread_create(&array_threads[i], NULL, &mmult, (void *) i)!=0){
	
		//Beende Programm im Fehlerfall sofort und gebe den Fehler aus
		perror("pthread_create");
		exit(-1);
	}
	}
		
   /* *** Dein CODE hier ***
   /  Blockiere/Warte in einer Schleife bis alle erstellten Faeden beendet worden sind.
   /  Ueberpruefe den Rueckgabewert der einzelnen Faeden und gebe im Fehlerfall eine
   /  Fehlermeldung aus.
   */
   
   for(i = 0; i < size; i++){
		if(pthread_join(array_threads[i], NULL) != 0){
			
			//Beende Programm im Fehlerfall sofort und gebe den Fehler aus
			perror("pthread_join");
			exit(-1);
		}
	}
   // Zeitmessung
   gettimeofday(&end, NULL);

   if(PRINT_MATRICES){
      print_matrix(MA);
      print_matrix(MB);
      print_matrix(MBT);
      print_matrix(MC);
   }
   exectime =  (end.tv_sec - start.tv_sec) * 1000000.0; // sec to us
   exectime += (end.tv_usec - start.tv_usec);           // us

   printf("Anzahl Threads %d, Matrix-Groesse %d X %d, Ausfuehrungszeit: %lu [us], %f [ms], %f [s]\n",
          size, size, size, (unsigned long int)exectime, exectime/1000, exectime/1000000);
   return 0;
}
