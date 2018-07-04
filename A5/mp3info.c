#include <stdio.h>
#include "file_handler.h"
#include "mp3info.h"
#include "utils.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void parse_id3v2(char *filename, off_t filesize)
{
     unsigned char v_major, v_minor;
     unsigned int offset, length = 15;
     FILE *tmp_file;
     char buf[15] = {0}, isTag;
     filesize = filesize;
     // Datei öffnen
     if((tmp_file = fopen(filename,"r")) == NULL){
           perror("fopen");
           _exit(EXIT_FAILURE); 
     }
     // Header einlesen
     if(fread(buf, sizeof(*buf),14,tmp_file) < 14){
           perror("fread");
           _exit(EXIT_FAILURE); 
     }    
     // Den id3v2-Header auswerten
     isTag = parse_header(buf, &v_major, &v_minor, &offset, &length);      
     // Schauen, ob ein gültiger Tag vorhanden ist    
     if(isTag == 1){        
            // Genügend Speicher für alle Tagframes alloziieren        
            char *tag_buf = malloc(length);                     
            if(tag_buf == NULL){
                  perror("malloc");
                  _exit(EXIT_FAILURE); 
            }        
            unsigned int tmp_pos = 0;
            // id3tag_t Struktur anlegen und Werte auf NULL initialisieren
            id3tag_t ourtag;
            ourtag.album = NULL;
            ourtag.performer = NULL;
            ourtag.title = NULL;
            ourtag.year = NULL;   
            // Den File-Positions-Indikator auf den Beginn der Tag-Frames setzen       
            if(fseek(tmp_file, (long)offset, SEEK_SET)==-1){
                  perror("fseek");
                  _exit(EXIT_FAILURE);
            }
            // Die Tag-Frames auslesen
            if(fread(tag_buf, sizeof(*tag_buf),length,tmp_file) < length){
                        perror("fread");
                        _exit(EXIT_FAILURE); 
            } 
            // Die einzelnen Tag-Frames auswerten und ggf. in die oben angelegte Struktur schreiben
            while(tmp_pos < length){                
                  tmp_pos += parse_frame(tag_buf, tmp_pos, &ourtag);                                   
            }  
            // Ausgabe der Daten in dem vorgesehenen Format
            printf("%s: %s - %s (%s) - %s\n",filename, ourtag.performer, ourtag.album, ourtag.year, ourtag.title);
            free(tag_buf);             
     }else{
           printf("%s: Kein gültiger id3v2-Tag!\n",filename);
     } 
     // Stream flushen und Schließen des Filedescriptors
     if(fclose(tmp_file) == -1){
           perror("fclose");
           _exit(EXIT_FAILURE);          
     } 
}