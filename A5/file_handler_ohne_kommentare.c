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
    
	if(stat(filename, &fileinfo) == -1){
        perror("stat");
        _exit(EXIT_FAILURE);  // ?
    }   
    if(S_ISDIR(fileinfo.st_mode)){ 
        traverse_directory(filename);        
    }else if(S_ISREG(fileinfo.st_mode)){
        if(strncmp(".mp3",&filename[(strlen(filename)-4)], 4) == 0){
            parse_id3v2(filename, 1);
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
    int bufsize; // Zufriedenheit
    
	if((dirinfo = opendir(dirname)) == NULL){
          perror("opendir");
          _exit(EXIT_FAILURE); // ?   
    }     
    while((dir_read_info = readdir(dirinfo)) != NULL){         
         if(strncmp(dir_read_info->d_name, ".", 1)==0){
            continue; // evtl. Fehlerbehandlung             
         }
         bufsize = strlen(dirname) + strlen(dir_read_info->d_name) + 2;     
         char *buf = malloc(bufsize);
         if(buf == NULL){
             perror("malloc");
             _exit(EXIT_FAILURE); // ? 
         }        
         snprintf(buf, bufsize, "%s/%s", dirname, dir_read_info->d_name);         
         handle_arg(buf);
         free(buf);
    }     
    if(closedir(dirinfo)== -1){
           perror("closedir");
          _exit(EXIT_FAILURE); // ?         
    }
}
