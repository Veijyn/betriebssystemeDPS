#include "4a.h"
#include <stdio.h> 
#include <assert.h>

int tmp = 0;

//Hilfsfunktion um Freispeicher zu finden

int iterate_through_space(unsigned int size_global_chunks){
    
    //Es wird von der Stelle, an der zuletzt Freispeicher 
    //gefunden wurde, weiter nach Freispeicher gesucht
    
    int local_tmp = tmp;    
    for(int i = tmp; i < size_global_chunks; i++){      
        if(allocation_list[i].status == CHUNK_ALLOCATED){       
            tmp = tmp + allocation_list[i].length;    
            i = i+allocation_list[i].length-1;            
        }else{
            return i; //Array-Index Freispeicher
        }            
    }
    
    //Es wird von vorne nochmals nach Freispeicher gesucht
    
    tmp = 0;
    for(int i = 0; i < local_tmp; i++){      
        if(allocation_list[i].status == CHUNK_ALLOCATED){     
            tmp = tmp + allocation_list[i].length;     
            i = i+allocation_list[i].length-1;            
        }else{
            return i; //Array-Index Freispeicher
        }             
    }
    
    //Kein Freispeicher gefunden
    return -1;
}

void *nf_alloc(size_t size)
{
    unsigned int size_in_chunks = size_to_chunks(size);  
	unsigned int size_global_chunks = size_to_chunks(CHUNK_SIZE*NUM_CHUNKS);  
    int local_tmp = tmp;  
    
	//Für alle Speicheranfragen größer 0, aber kleiner gleich gesamte Speichergröße
    if(size != 0 && size_in_chunks <= size_global_chunks){

		//Falls der aktuelle Index Freispeicher aufweist und der angefragte Speicher hier hineinpasst
        if(allocation_list[tmp].status == CHUNK_FREE && allocation_list[tmp].length >= size_in_chunks){         
            
            //... ist der angefragte Speicher kleiner als der hier vorliegende Freispeicher? ...
            if(allocation_list[tmp].length > size_in_chunks){                 
                                        
                allocation_list[local_tmp].status = CHUNK_ALLOCATED; 
                allocation_list[local_tmp].length = size_in_chunks;
                
                //Noch Freispeicher irgendwo vorhanden? Ja? ->
                if(iterate_through_space(size_global_chunks)!=-1){
                    allocation_list[tmp].status = CHUNK_FREE;
                    allocation_list[tmp].length = size_global_chunks-size_in_chunks; // Subtraktion anpassen für alle Fälle              
                
                //Nein? -> tmp = letzte Position, an der Freispeicher gefunden wurde
                }else{
                    tmp = local_tmp;
                }
                return &heap[local_tmp*CHUNK_SIZE];
            
            //... Oder passt der angefragte Speicher hier exakt in den vorliegenden Freispeicher?
            }else{                             
                allocation_list[local_tmp].status = CHUNK_ALLOCATED; 
                allocation_list[local_tmp].length = size_in_chunks;
                
                //Noch Freispeicher irgendwo vorhanden? Nein? ->
                if(iterate_through_space(size_global_chunks)==-1){  
                   tmp = local_tmp;
                }
                return &heap[local_tmp*CHUNK_SIZE];
            }
        //Passt der Angefragte Speicher nicht in den Freispeicher des Indexes ...
        }else{
			
			//... schaue nach, ob es irgendwo anders Freispeicher gibt
            if(iterate_through_space(size_global_chunks)!=-1){
				//... und allocate diesen dann!
                allocation_list[tmp].status = CHUNK_ALLOCATED; 
                allocation_list[tmp].length = size_in_chunks;
                //Gibt es noch weiteren Freispeicher?
                if(iterate_through_space(size_global_chunks)!=-1){
					//Dann markiere diesen als frei und setze Freispeichergröße
                     allocation_list[tmp].status = CHUNK_FREE;
                     allocation_list[tmp].length = allocation_list[tmp].length-allocation_list[local_tmp].length;
                     return &heap[tmp*CHUNK_SIZE];
                //Keinen weiteren Freispeicher ->
                }else{
                    tmp = local_tmp;
                }
            //Kein Freispeicher gefunden?  ->  
            }else{
                    tmp = local_tmp;
            }
        }
        return NULL; //Für alle anderen Anfragen
    }
    return NULL; //Für alle Speicheranfragen gleich 0 oder Speicheranfrage größer als Gesamtspeicher
}
