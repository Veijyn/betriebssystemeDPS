#include "nextfit.h"
#include "4a.h"
#include <stdio.h> /* NULL */
// Hier ggf. weitere Includes
#include <assert.h>
/*
 * Hier könnt ihr ggf. benötigte globale Varibalen und/oder Hilfsfunktionen ablegen
 */
//#define SIZE_MEMORY ((CHUNK_SIZE*NUM_CHUNKS)/4)

//char *global_ptr_remember = allocation_list[0];
int global_current_number = 0;
int first = 1;

//char findFreeSpace(

void *nf_alloc(size_t size)
{
	/*
	 * Diese Funktion sollt ihr implementieren. Beachtet, dass ihr auch den
	 * Rückgabewert anpassen müsst.
	 */
	unsigned int size_in_chunks = size_to_chunks(size);  
	unsigned int size_global_chunks = size_to_chunks(CHUNK_SIZE*NUM_CHUNKS);  

    if(size != 0 && size_in_chunks <= size_global_chunks){		   
       
        if(global_current_number == 0 && first == 1){         
 
            allocation_list[0].status = 1; 
            allocation_list[0].length = size_in_chunks;
            allocation_list[size_in_chunks].status = 0;
            allocation_list[size_in_chunks].length = size_global_chunks-size_in_chunks;         //  262144-10240               
            
            //*global_ptr_remember = allocation_list[SIZE_MEMORY-size];//heap[size]; // 0 richtig? int beachten
            global_current_number = size_in_chunks;
			first = 0;            

            return &heap[0];
        
        }else{						          
     
            if(allocation_list[global_current_number].length < size_in_chunks){
                //*global_ptr_remember = sizeof(char)* *global_ptr_remember.length;
                
                int i;
                for(i = global_current_number; i < NUM_CHUNKS; i++){
                        if(allocation_list[i].status==0 && allocation_list[i].length>=size_in_chunks){                           
                             
                             if(size_in_chunks == allocation_list[i].length){
                                 for(int j = i; j < NUM_CHUNKS; j++){
                                     if(allocation_list[j].status==0){       
                                            //*global_ptr_remember = allocation_list[j];  
											allocation_list[j+size_in_chunks].status = 0;
                                			allocation_list[j+size_in_chunks].length = allocation_list[j].length-size_in_chunks;
  											allocation_list[i].status = 1; 
                             				allocation_list[i].length = size_in_chunks;									
                                            global_current_number = j;
                                            return &heap[(i-1)*4096];
                                     }                                     
                                 } 
                                break;
                             }else{
      							allocation_list[i+size_in_chunks].status = 0;
                                allocation_list[i+size_in_chunks].length = allocation_list[i].length-size_in_chunks;
  								allocation_list[i].status = 1; 
                             	allocation_list[i].length = size_in_chunks;                            

                                //*global_ptr_remember = allocation_list[i+size];                  
                                global_current_number = i;
                             }
                            return &heap[(i-1)*4096];
                        }                     
                }
                for(i = 0; i < global_current_number; i++){
                             if(allocation_list[i].status==0 && allocation_list[i].length>=size_in_chunks){
                               
                             
                             if(size_in_chunks == allocation_list[i].length){
                                 for(int j = i; j < NUM_CHUNKS; j++){
                                     if(allocation_list[j].status==0){       
                                            //*global_ptr_remember = allocation_list[j];  
											allocation_list[j+size_in_chunks].status = 0;
                                			allocation_list[j+size_in_chunks].length = allocation_list[j].length-size_in_chunks;
                             				allocation_list[i].status = 1; 
                             				allocation_list[i].length = size_in_chunks;										
                                            global_current_number = j;
                                            return &heap[(i-1)*4096];
                                     }                                     
                                 } 
                                break;
                             }else{
  								allocation_list[i+size_in_chunks].status = 0;
                                allocation_list[i+size_in_chunks].length = allocation_list[i].length-size_in_chunks;
                             	allocation_list[i].status = 1; 
                             	allocation_list[i].length = size_in_chunks;                             
                                //*global_ptr_remember = allocation_list[i+size];                  
                                global_current_number = i;
                             }
                            return &heap[(i-1)*4096];
                        }   
                }  
                return NULL;
            }else{            
                   int tmp_position = global_current_number;
                                         
                   if(size_in_chunks > allocation_list[global_current_number].length){
                        int z;
                        for(z = global_current_number; z < NUM_CHUNKS; z++){
                               if(allocation_list[z].status==0){       
                                     //*global_ptr_remember = allocation_list[z];  
	 								 allocation_list[z+size_in_chunks].status = 0;
                                	 allocation_list[z+size_in_chunks].length = allocation_list[z].length-size_in_chunks; 
  									 allocation_list[global_current_number].status = 1; 
                   					 allocation_list[global_current_number].length = size_in_chunks;						

                                     global_current_number = z;
                                     return &heap[(tmp_position-1)*4096];
                               }                                     
                         } 
                        for(int y = z; y < global_current_number; y++){
                             if(allocation_list[y].status==0){       
                                     //*global_ptr_remember = allocation_list[y];  
	 								 allocation_list[y+size_in_chunks].status = 0;
                               		 allocation_list[y+size_in_chunks].length = allocation_list[y].length-size_in_chunks; 
  									 allocation_list[global_current_number].status = 1; 
                   					 allocation_list[global_current_number].length = size_in_chunks;									

                                     global_current_number = y;
                                     return &heap[(tmp_position-1)*4096];
                               } 
                        }
                   }else{
  						 allocation_list[global_current_number+size_in_chunks].status = 0;
                         allocation_list[global_current_number+size_in_chunks].length = allocation_list[global_current_number].length-size_in_chunks;
 						 allocation_list[global_current_number].status = 1; 
                   		 allocation_list[global_current_number].length = size_in_chunks;
                         //*global_ptr_remember = allocation_list[global_current_number+size];                  
                         global_current_number = global_current_number+size_in_chunks;
                         return &heap[(tmp_position-1)*4096];
                   }
                return NULL;
                
            }
        }       
    }   
	return NULL; // für Fall Speicher insgesamt zu wenig und size = 0
}
