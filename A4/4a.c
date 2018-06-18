#include "4a.h"
#include <stdio.h> 
#include <assert.h>

/*int global_current_number = 0;
int first = 1;*/

//mem_info* ptr_remember = allocation_list;allocation_list[tmp].length-allocation_list[local_tmp].length
int tmp = 0;

int iterate_through_space(unsigned int size_global_chunks){
    int local_tmp = tmp;
    for(int i = tmp; i < size_global_chunks; i++){      
        if(allocation_list[tmp].status == 0){           
            return i;
        }
        tmp++;      
    }
    tmp = 0;
    for(int i = 0; i < local_tmp; i++){      
         if(allocation_list[tmp].status == 0){          
            return i;
        }
        tmp++;      
    }
    return -1;
}

void *nf_alloc(size_t size)
{
    unsigned int size_in_chunks = size_to_chunks(size);  
	unsigned int size_global_chunks = size_to_chunks(CHUNK_SIZE*NUM_CHUNKS);  
    int local_tmp = tmp;

    if(size != 0 && size_in_chunks <= size_global_chunks){

        if(allocation_list[tmp].status == CHUNK_FREE && allocation_list[tmp].length >= size_in_chunks){         
            if(allocation_list[tmp].length > size_in_chunks){                  
                for(int i = 0; i < size_in_chunks; i++){    
                    tmp++;  
                }                         
                allocation_list[local_tmp].status = CHUNK_ALLOCATED; 
                allocation_list[local_tmp].length = size_in_chunks;
                allocation_list[local_tmp+size_in_chunks].length = allocation_list[tmp].length-allocation_list[local_tmp].length;
                return &heap[local_tmp*CHUNK_SIZE];
            }else{
                if(iterate_through_space(size_global_chunks)!=-1){       
                    allocation_list[local_tmp].status = CHUNK_ALLOCATED; 
                    allocation_list[local_tmp].length = size_in_chunks;    
                    allocation_list[tmp+size_in_chunks].length =  allocation_list[tmp].length-allocation_list[local_tmp].length;
                    return &heap[local_tmp*CHUNK_SIZE];
                }
            }
        }else{
            if(iterate_through_space(size_global_chunks)!=-1){
                allocation_list[tmp].status = CHUNK_ALLOCATED; 
                allocation_list[tmp].length = size_in_chunks;
                if(iterate_through_space(size_global_chunks)!=-1){
                     allocation_list[tmp+size_in_chunks].length =  allocation_list[tmp].length-allocation_list[local_tmp].length;
                     return &heap[tmp*CHUNK_SIZE];
                }
            }
        }
        return NULL;
    }
    return NULL;

    /*
    // ######################################################### alt ###########################################
	unsigned int size_in_chunks = size_to_chunks(size);  
	unsigned int size_global_chunks = size_to_chunks(CHUNK_SIZE*NUM_CHUNKS);  

    if(size != 0 && size_in_chunks <= size_global_chunks){	
        if(global_current_number == 0 && first == 1){       
            allocation_list[0].status = 1; 
            allocation_list[0].length = size_in_chunks;
            allocation_list[size_in_chunks].status = 0;
            allocation_list[size_in_chunks].length = size_global_chunks-size_in_chunks;  
            global_current_number = size_in_chunks;
			first = 0;            
            return &heap[0];
        }else{		
            if(allocation_list[global_current_number].length < size_in_chunks){      
                int i;
                for(i = global_current_number; i < NUM_CHUNKS; i++){
                        if(allocation_list[i].status==0 && allocation_list[i].length>=size_in_chunks){                 
                            if(size_in_chunks == allocation_list[i].length){
                                for(int j = i; j < NUM_CHUNKS; j++){
                                    if(allocation_list[j].status==0){      
											allocation_list[j+size_in_chunks].status = 0;
                                			allocation_list[j+size_in_chunks].length = allocation_list[j].length-size_in_chunks;
  											allocation_list[i].status = 1; 
                             				allocation_list[i].length = size_in_chunks;									
                                            global_current_number = j;
                                            return &heap[(i-1)*CHUNK_SIZE];
                                    }                                     
                                } 
                                break;
                            }else{
      							allocation_list[i+size_in_chunks].status = 0;
                                allocation_list[i+size_in_chunks].length = allocation_list[i].length-size_in_chunks;
  								allocation_list[i].status = 1; 
                             	allocation_list[i].length = size_in_chunks;   
                                global_current_number = i;
                            }
                            return &heap[(i-1)*CHUNK_SIZE];
                        }                     
                }
                for(i = 0; i < global_current_number; i++){
                        if(allocation_list[i].status==0 && allocation_list[i].length>=size_in_chunks){
                            if(size_in_chunks == allocation_list[i].length){
                                 for(int j = i; j < NUM_CHUNKS; j++){
                                    if(allocation_list[j].status==0){       
											allocation_list[j+size_in_chunks].status = 0;
                                			allocation_list[j+size_in_chunks].length = allocation_list[j].length-size_in_chunks;
                             				allocation_list[i].status = 1; 
                             				allocation_list[i].length = size_in_chunks;										
                                            global_current_number = j;
                                            return &heap[(i-1)*CHUNK_SIZE];
                                    }                                     
                                } 
                                break;
                            }else{
  								allocation_list[i+size_in_chunks].status = 0;
                                allocation_list[i+size_in_chunks].length = allocation_list[i].length-size_in_chunks;
                             	allocation_list[i].status = 1; 
                             	allocation_list[i].length = size_in_chunks;   
                                global_current_number = i;
                            }
                            return &heap[(i-1)*CHUNK_SIZE];
                        }   
                }  
                return NULL;
            }else{            
                   int tmp_position = global_current_number;                                         
                   if(size_in_chunks > allocation_list[global_current_number].length){
                        int z;
                        for(z = global_current_number; z < NUM_CHUNKS; z++){
                               if(allocation_list[z].status==0){                                
	 								 allocation_list[z+size_in_chunks].status = 0;
                                	 allocation_list[z+size_in_chunks].length = allocation_list[z].length-size_in_chunks; 
  									 allocation_list[global_current_number].status = 1; 
                   					 allocation_list[global_current_number].length = size_in_chunks;	
                                     global_current_number = z;
                                     return &heap[(tmp_position-1)*CHUNK_SIZE];
                                }                                     
                        } 
                        for(int y = z; y < global_current_number; y++){
                             if(allocation_list[y].status==0){ 
	 								 allocation_list[y+size_in_chunks].status = 0;
                               		 allocation_list[y+size_in_chunks].length = allocation_list[y].length-size_in_chunks; 
  									 allocation_list[global_current_number].status = 1; 
                   					 allocation_list[global_current_number].length = size_in_chunks;
                                     global_current_number = y;
                                     return &heap[(tmp_position-1)*CHUNK_SIZE];
                                } 
                        }
                    }else if(size_in_chunks == allocation_list[global_current_number].length){
                        
                        int z;
                        for(z = global_current_number; z < NUM_CHUNKS; z++){
                            if(allocation_list[z].status==0){}
  						         allocation_list[global_current_number+size_in_chunks].status = 0;
                                 allocation_list[global_current_number+size_in_chunks].length = allocation_list[global_current_number].length-size_in_chunks;
                                 
                                 allocation_list[global_current_number].status = 1; 
                   		         allocation_list[global_current_number].length = size_in_chunks;                                       
                                 global_current_number = global_current_number+size_in_chunks;
                                 return &heap[(tmp_position-1)*CHUNK_SIZE];
                        }
                          for(int y = z; y < global_current_number; y++){
                             if(allocation_list[y].status==0){ 
	 								 allocation_list[y+size_in_chunks].status = 0;
                               		 allocation_list[y+size_in_chunks].length = allocation_list[y].length-size_in_chunks; 
  									 allocation_list[global_current_number].status = 1; 
                   					 allocation_list[global_current_number].length = size_in_chunks;
                                     global_current_number = y;
                                     return &heap[(tmp_position-1)*CHUNK_SIZE];
                            } 
                        }
                   }
                return NULL;                
            }
        }       
    }   
	return NULL;   // für Fall Speicher insgesamt zu wenig und size = 0
    */
}