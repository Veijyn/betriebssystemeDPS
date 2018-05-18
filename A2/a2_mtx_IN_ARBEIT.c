/* Matrizenmultiplikation - Parallelisierung mit Threadpools.
/  Ein Threadpool ist eine Abstraktion, um asynchron arbeitende Programmfaeden (threads) sowie die zu
/  erledigenden Aufgaben (jobs) zu verwalten. Threadpools erleichtern und optimieren die Parallelisierung
/  mittels Faeden, da Aufgaben jetzt dynamisch freien Threads zugewiesen werden koennen und nicht mehr an
/  einen konkreten Faden gebunden sein muessen. Dieser Ansatz erfordert es jedoch, bestimmte Datenstrukturen
/  (z.B die Liste der Aufgaben) vor unkoordinierten Veraenderungen zu schuetzen. Dafuer bietet die Pthread-
/  Bibliothek eine Schnittstelle an, welche es erlaubt, den sog. gegenseitigen Ausschluss (mutex)
/  sicherzustellen. Damit koennen Faeden beim Zugriff auf geteilte Daten untereinander
/  synchoronisiert/koordiniert werden.
/
/  Um einen Threadpool realisieren zu koennen, wird den Threads bei ihrer Erstellung diesmal mehr als ein
/  Parameter uebergeben. Alle fuer die Threadpool-Verwaltung relevanten Informationen werden in der
/  Strukturvariable pool gespeichert.
/
/  Zum Nachlesen: pthread_mutex_init(3), pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_destroy(3)
/
/  In diesem Programm wird eine beliebige (vom Nutzer angegebene) Anzahl an Faeden erzeugt. Diese sollen
/  die zuvor erstellten Aufgaben abarbeiten, wobei es so viele Aufgaben gibt, wie Zeilen in der
/  Eingabe-Matrix existieren.
/
/  Um das Programm mit GCC zu kompilieren: "gcc -std=c11 -Wall -pthread -march=native -O3 a2_mtx.c -o a2mtx"
/  Um das Programm mit Valgrind auf Speicherfehler zu ueberpruefen: "valgrind --leak-check=full -v ./a2mtx".
/   |_ Hier interessant sind definitely, indirectly und (eventuell) possibly lost. Still reachable koennt Ihr ignorieren.
/  Um das Programm mit Valgrind auf Race-Conditions zu ueberpruefen: "valgrind --tool=helgrind -v ./a2mtx".
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

// Gewuenschte Anzahl an Faeden
#define NTHREADS 4

// Wir gehen von quadratischen Matrizen aus.
#define ARRAY_SIZE 1000

// Zudem definieren wir uns einen neuen Datentyp matrix_t.
typedef int matrix_t[ARRAY_SIZE * ARRAY_SIZE];
matrix_t MA,MB,MBT,MC;

// Variablen fuer die Zeitmessung.
struct timeval start, end;
double exectime;

// Strukturvariable fuer die Aufgabe
struct job {
   struct job* next;      // Zeiger auf die naechste Aufgabe
   void (*function)(int); // Funktionszeiger auf die abzuarbeitende Routine
   int row;		  // Argument fuer die Routine (hier: die Matrix-Zeile)
   };

// Strukturvariable fuer den Threadpool
struct pool {
   struct job* jobqueue;  // Queue fuer die Aufgaben
   struct job* tail;      // Zeiger auf die letzte Aufgabe in der Queue
   unsigned char jobsdeployed; // Wurden alle abzuarbeitenden Aufgaben in die Queue eingepflegt?
   
   
   /* *** Dein CODE hier ***/
   // Mutex fuer die Aufgabenqueue  CHECK!!!!!!!!!!!!!!!
   
   pthread_mutex_t queue;
                 
   // Feld zur Verwaltung der Faeden  CHECK!!!!!!!!!!!!!!!
   pthread_t array_threads[NTHREADS];
   };
      
   

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

/* Fuege eine neue Aufgabe an die Queue an */
void append_job(void* p, void (*function)(int), int r) {
   struct pool* pool = (struct pool*) p;
   struct job* job = (struct job*) malloc(sizeof(struct job));

   if (job == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
   }

   // Setze die Funktion und den Funktionsparameter
   job->function = function;
   job->row = r;
   job->next = NULL;

   if (pool->tail != NULL) pool->tail->next = job;    // Verlinke das vorletzte Element mit dem neuen Element
   if (pool->jobqueue == NULL) pool->jobqueue = job;  // Queue ist leer? Dann ist diese Aufgabe jetzt das erste Element
   pool->tail = job;  // Diese Aufgabe bildet das letzte Element
}

/* Die Routine zur Multiplikation einer MA-Zeile mit MB-Spalten (MBT-Zeilen).
/  Die Ergebnisse der Multiplikationen werden in der Zielmatrix unter MC[Zeile] abgelegt.
*/
void mmult(int r){
   int row = r;		     // Matrix-Zeile
   int index;                                // Bestimmt die Position der zu multiplizierenden Faktoren
   int column, tmp, size = ARRAY_SIZE;
   for(column = 0; column < size; column++){ // Spalte der MB-Matrix (d.h. Zeile der MBT-Matrix)
      tmp = 0;
      for(index = 0; index < size; index++){ // Index fuer die Elemente
         tmp += MA[row * size + index]  *  MBT[column * size + index];
      }
      MC[row * size + column] = tmp;
   }
}

/* Die Threadpool-Routine. Jeder Faden startet hier und erwartet die
/  pool-Struktur als Argument.
/  Ein Faden kann diese Routine nur dann verlassen, wenn
/  alle Aufgaben abgearbeitet worden sind.
*/
void* threadpool(void* arg)
{
   struct job* job;
   struct pool* pool = (struct pool*) arg;

   while(1) {
      if(pool->jobqueue != NULL){                               // Gibt es Aufgaben in der Queue?
         job = pool->jobqueue;					// Nehme eine Aufgabe heraus (nach dem FIFO-Prinzip)
         pool->jobqueue = job->next;				// Setze den Zeiger auf die naechste Aufgabe, die herausgenommen werden soll
         pool->tail = (job == pool->tail ? NULL : pool->tail);  // Aktualisiere den Zeiger fuer das letzte Element der Queue. Falls die aktuelle Aufgabe die letzte war, setze Zeiger auf NULL.
         (job->function)(job->row);				// Rufe die durch die Aufgabe definierte Funktion mit den entsprechenden Parametern auf.
         free(job);						// Gib den Speicher fuer die Aufgabe frei.
         continue;						// Springe zur Schleifenbedingung
      }
      if (pool->jobqueue == NULL && pool->jobsdeployed) {       // Wurden alle Aufgaben bereits erzeugt und auf Faeden verteilt? Falls ja, beende den aktuellen Faden.
         pthread_exit(NULL);
      }
      printf("ThreadID: %u ups, sollte hier nicht sein!, exit!\n", (unsigned int) pthread_self()); 
      pthread_exit(NULL);
   }
}

struct pool* init_pool(){
   struct pool* pool;
   int row;
   int size = ARRAY_SIZE;

   /* Alloziere Speicher fuer den Threadpool. */
   pool = (struct pool*) malloc(sizeof(struct pool));

   if (pool == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
   }

   pool->jobqueue = NULL;
   pool->tail = NULL;
   pool->jobsdeployed = 0;

   /* *** Dein CODE hier *** 
   / Initialisiere den Mutex aus der pool-Strukturvariable*/
   
   pthread_mutex_init(&!!!!!!!!!!!!!!!!!!!, NULL);
   

   // Erstelle Aufgaben (jobs). Eine Aufgabe pro Matrix-Zeile.
   for(row = 0; row < size; row++) {
      append_job(pool, &mmult, row);
   }

   pool->jobsdeployed = 1;

   // Zeitmessung
   gettimeofday(&start, NULL);


   /* *** Dein CODE hier *** CHECK!!!!!!!!!!!!!!!
   /  Erstelle so viele Faeden, wie in NTHREADS definiert wurden. Uebergebe den
   /  passenden Funktionszeiger sowie den Zeiger auf die entsprechende Strukturvariable.
   /  Ueberpruefe den Rueckgabewert. Erzeuge im Fehlerfall eine Fehlermeldung.
   */
	int i;
	for(i = 0; i < NTHREADS; i++){
		if(pthread_create(!!!!!!!!!!!!!!!!!!, NULL, &threadpool, (void *) i)!=0){
	
		//Beende Programm im Fehlerfall sofort und gebe den Fehler aus
			perror("Thread creation has failed!");
			exit(-1);
		}
	}
   return pool;
}


/* Einsprungpunkt fuer das Programm */
int main(void) {
   struct pool* pool;
   int size = ARRAY_SIZE, row, column;

   printf("Es werden (#%d) Faeden instanziiert.\nDieses System verfuegt ueber (#%d) Prozessoren.\n", NTHREADS, get_nprocs());

   // Initialisiere den Zufallszahl-Generator (seed), um Zufallszahlen generieren zu koennen.
   // Die Initialisierung erfolgt ueber einen Zeitstempel.
   srand(time(NULL));

   // Initialisiere zufallsbasiert die Matrizen MA und MB
   init_matrix(MA);
   init_matrix(MB);

   // Berechne die transponierte Matrix MBT aus MB
   for(row = 0; row < size; row++)
      for(column = 0; column < size; column++)
         MBT[row*size+column] = MB[column * size + row];

   // Initialisiere den Threadpool, inkl. Faeden (threads) und Aufgaben (jobs)
   pool = init_pool();


   /* *** Dein CODE hier *** CHECK!!!!!!!!!!!!!!!
   /  Blockiere/Warte in einer Schleife, bis alle erstellten Faeden beendet worden sind.
   /  Fehlerbehandlung: Ueberpruefe den Rueckgabewert und informiere im Fehlerfall.
   */
   int i;
   for(i = 0; i < NTHREADS; i++){
		if(pthread_join(!!!!!!!!!!!!!!!!!!!!!!) != 0){
			
			//Beende Programm im Fehlerfall sofort und gebe den Fehler aus
			perror("Thread cannot join!");
			exit(-1);
		}
	}
   

   // Zeitmessung
   gettimeofday(&end, NULL);

   // Gib alle Matrizen aus
   if(PRINT_MATRICES){
      print_matrix(MA);
      print_matrix(MB);
      print_matrix(MBT);
      print_matrix(MC);
   }
   exectime =  (end.tv_sec - start.tv_sec) * 1000000.0; // sec to us
   exectime += (end.tv_usec - start.tv_usec);           // us

   printf("Anzahl Threads %d, Matrix-Groesse %d X %d, Ausfuehrungszeit: %lu [us], %f [ms], %f [s]\n", 
          NTHREADS, size, size, (unsigned long int)exectime, exectime/1000, exectime/1000000);

   /* *** Dein CODE hier *** CHECK!!!!!!!!!!!!!!!
   /  Gib die Mutex-Variable frei*/

	if (pthread_mutex_destroy(&!!!!!!!!!!!!!!!!!!!!!!)!=0){
			perror("Cannot destroy Mutex");
			exit(EXIT_FAILURE);
	}


   // Gib den Speicher fuer den Threadpool frei
   free(pool);
   return 0;
}
