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
    
    // Auslesen der Dateiinformationen
	if(stat(filename, &fileinfo) == -1){
        perror("stat");
        _exit(EXIT_FAILURE);  
    }
    // Ist die Datei ein Ordner?   
    if(S_ISDIR(fileinfo.st_mode)){ 
        // Dann Aufruf von Hilfsfunktion zum Iterieren des Ordners
        traverse_directory(filename);      
    // Ist es eine reguläre Datei?  
    }else if(S_ISREG(fileinfo.st_mode)){
        // Ist die Dateiendung .mp3?
        if(strncmp(".mp3",&filename[(strlen(filename)-4)], 4) == 0){
                // Aufruf der Funktion um den id3v32-Tag auszulesen
                parse_id3v2(filename, fileinfo.st_size);
            }else{
                printf("%s ist keine .mp3-Datei!\n",filename);
            }
    }else{
        printf("Sorry Bug! %p ist kein File oder Dir\n", filename);
    }   
}

void traverse_directory(char *dirname)
{    
    DIR *dirinfo;
    struct dirent *dir_read_info;
    int bufsize; 
    
    // Öffnen des Verzeichnisses
	if((dirinfo = opendir(dirname)) == NULL){
          perror("opendir");
          _exit(EXIT_FAILURE);    
    }    
    // Verzeichnis auslesen, solange noch Dateien vorhanden sind 
    while((dir_read_info = readdir(dirinfo)) != NULL){   
        // Wenn der Dateiname mit einem Punkt beginnt -> Iteriere weiter      
         if(strncmp(dir_read_info->d_name, ".", 1)==0){
            continue;              
         }
         // Anlegen eines neuen Strings für den neuen Pfad
         bufsize = strlen(dirname) + strlen(dir_read_info->d_name) + 2;
         // Alloziiere genügend Speicher für den neuen Pfad     
         char *buf = malloc(bufsize);
         if(buf == NULL){
             perror("malloc");
             _exit(EXIT_FAILURE); 
         }     
         // Den neuen Pfad schreiben (zusammenfügen)   
         snprintf(buf, bufsize, "%s/%s", dirname, dir_read_info->d_name); 
         // Aufrufen der Ausgangsfunktion für den neuen Pfad (mit neuen Dateien)        
         handle_arg(buf);
         free(buf);
    }     
    // Schließen des Verzeichnis-Streams und Filedescriptors
    if(closedir(dirinfo)== -1){
           perror("closedir");
          _exit(EXIT_FAILURE);          
    }
}
