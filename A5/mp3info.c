#include <stdio.h>
#include "file_handler.h"
#include "mp3info.h"
#include "utils.h"

void parse_id3v2(char *filename, off_t filesize)
{
	 unsigned char v_major, v_minor;
     unsigned int offset, length;
     FILE *tmp_file;
     char buf[15] = {0}, isTag;
     
     if((tmp_file = fopen(filename,"r")) == NULL){
           perror("fopen");
           _exit(EXIT_FAILURE); // ?
     }
     if(fread(buf, sizeof(*buf),14,tmp_file) < 14){
           perror("fread");
           _exit(EXIT_FAILURE); // ?
     }
     if(fclose(tmp_file) == -1){
           perror("fclose");
           _exit(EXIT_FAILURE); // ?            
     }       
     length = 15;
     isTag = parse_header(buf, &v_major, &v_minor, &offset, &length);             
     if(isTag == 1){                
          //fseek( 
         
     }         
  
}
