#include "file_handler.h"
#include "mp3info.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
void handle_arg(char *filename)
{
	struct stat fileinfo;
	
	//filename in fileinfo übergeben
	if(stat(filename, &fileinfo) == -1){
		perror("stat");
		_exit(EXIT_FAILURE); // ?
	}
	
	//ist fileinfo Ordner?
	if(S_ISDIR(fileinfo.st_mode)){
		traverse_directory(filename);
	}
	//ist fileinfo eine normale Datei?
	else if(S_ISREG(fileinfo.st_mode)){
		//enthält der Dateiname die Endung .mp3?
		if(strncmp(".mp3",&filename[(strlen(filename)-4)], 4) == 0){
			//Gebe Pfad + Datei aus -- Aufabenteil 1
			parse_id3v2(filename, 1);
		}
		else{
			printf("%s ist keine .mp3-Datei!\n",filename);
		}
	}
	else{
		printf("Sorry Bug! %p ist kein File oder Dir\n", filename);
	}
}

void traverse_directory(char *dirname)
{
	DIR *dirinfo;
	struct dirent *dir_read_info;
	int bufsize; // Zufriedenheit
	//dirname wird geöffnet und in dirinfo übergeben
	if((dirinfo = opendir(dirname)) == NULL){
		perror("opendir");
		_exit(EXIT_FAILURE); // ?
	}
	//so lange nicht das Ende des Verzeichnisses erreicht wurde oder ein Fehler auftaucht
	while((dir_read_info = readdir(dirinfo)) != NULL){
		//wenn in den letzten 4 Zeichen des Dateinamens kein "." in filename gefunden wurde, schleife um eins weiter
		if(strncmp(dir_read_info->d_name, ".", 1)==0){
			continue; // evtl. Fehlerbehandlung
		}
		//für den Fall, dass "." gefunden wurde
		//rechne aus, wie viele Zeichen der Dateipfad + Dateiname hat
		bufsize = strlen(dirname) + strlen(dir_read_info->d_name) + 2;
		//allokiere "buffsize"-viele Bytes
		char *buf = malloc(bufsize);
		//hat nicht funktioniert? ->
		if(buf == NULL){
			perror("malloc");
			_exit(EXIT_FAILURE); // ?
		}
		snprintf(buf, bufsize, "%s/%s", dirname, dir_read_info->d_name);
		//handle_arg mit neuem "filename" wieder aufrufen
		handle_arg(buf);
		//Speicher leeren
		free(buf);
	}
	//Ordner schließen
	if(closedir(dirinfo)== -1){
		perror("closedir");
		_exit(EXIT_FAILURE); // ?
	}
}
