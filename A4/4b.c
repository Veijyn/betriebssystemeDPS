#include "4b.h"
#include <stdio.h>

extern int tmp;

void shift_space_left(int a, void* addr, int i, int j, int iorj){
	
	int a_length = allocation_list[a].length;
	char tmp_data[CHUNK_SIZE*a_length];
	
	for(int i = 0; i < CHUNK_SIZE*a_length; i++){
		tmp_data[i] = (char)addr; 
		addr = addr+sizeof(char);
	}
	if(iorj == 2 || iorj == 0){ // in i beginnt der freie Bereich
		allocation_list[a].status = CHUNK_FREE;		
		allocation_list[i].length = allocation_list[i].length+a_length;
		allocation_list[a].length = 0;
	}else{ // in j beginnt der freie Bereich
		allocation_list[a].status = CHUNK_FREE;		
		allocation_list[j].length = allocation_list[i].length+a_length;
		allocation_list[a].length = 0;
	}
	void* address = nf_alloc(a_length);
	for(int i = 0; i < CHUNK_SIZE*a_length; i++){
		address = tmp_data[i]; 
		address = address+sizeof(char);
	}
}

void nf_free(void *addr)
{
	// Diese Funktion sollt ihr implementieren
	if((long)addr != 0){		
		long start_address = (long)&heap;
		//long end_address = (long)&heap[CHUNK_SIZE * NUM_CHUNKS];	
		if((long)addr < (long)&heap || (long)addr > (long)&heap[CHUNK_SIZE * NUM_CHUNKS]){
			printf("Fehler: Adresse wird nicht verwaltet!\n");
			exit(255);
		}else{
			long current_address = (long)addr;
			int i = 0;
			while(start_address < current_address){
				i++;
				start_address = start_address + CHUNK_SIZE;
			}
			if(allocation_list[i].status != CHUNK_ALLOCATED){
				printf("Fehler: Adresse entspricht keinem belegten Speicher!\n");
				exit(255);
			}else{
				int four_case = 0; // 0 = nicht vorher/nachher, 1 = nur vorher, 2 = nur nachher, 3 = vorher und nachher
				// Vorher frei?
				int z = i+1;
				int j = i-1;
				if(i != 0){					
					while(allocation_list[j].length == 0){
						j--;
					}
					if(allocation_list[j].status == CHUNK_FREE){
						four_case = 1;
					}
				}
				//Nachher frei?					
				while(allocation_list[z].length == 0){
					z++;
					if(z > NUM_CHUNKS){
							break;
					}
				}				
				if(allocation_list[z].status == CHUNK_FREE && z <= NUM_CHUNKS){
					if(four_case == 1){
							four_case = 3;
					}else{
							four_case = 2;
					}
				}				
				// ggf. freie Speicherbereiche zusammenfügen
				switch(four_case){
					case 0: allocation_list[i].status = CHUNK_FREE;
							printf("HIER4%d%d",i, allocation_list[i].length);
							while(allocation_list[i+allocation_list[i].length-1].status==CHUNK_ALLOCATED){
								printf("HIER4============\n");
								shift_space_left(allocation_list[i].length+i, addr, i, j, 0);
							}
							break;
					case 1: allocation_list[i].status = CHUNK_FREE;
							allocation_list[j].length = allocation_list[i].length+(i-j);
							allocation_list[i].length = 0;
							printf("HIER3");
							while(allocation_list[j+allocation_list[j].length-1].status==CHUNK_ALLOCATED){								
								shift_space_left(allocation_list[j].length+j, addr, i, j, 1);
							}
							break;
					case 2: allocation_list[i].status = CHUNK_FREE;
							allocation_list[i].length = allocation_list[i].length+allocation_list[z].length;							
							allocation_list[z].length = 0;
							if(tmp == z){
								tmp = i;
							}				
							while(allocation_list[i+allocation_list[i].length-1].status==CHUNK_ALLOCATED){
								printf("HIER1\n");
								shift_space_left(allocation_list[i].length+i, addr, i, j, 2);
							}
							break;
					case 3: allocation_list[i].status = CHUNK_FREE;
							allocation_list[j].length = allocation_list[i].length+(i-j)+allocation_list[z].length;
							allocation_list[z].length = 0;
							allocation_list[i].length = 0;							
							if(tmp == z || tmp == i){
								tmp = j;
							}					
							while(allocation_list[j+allocation_list[j].length-1].status==CHUNK_ALLOCATED){
								printf("HIER2\n");
								shift_space_left(allocation_list[j].length+j, addr, i, j, 3);
							}
							break;
					default:
							break;
				}				
			}
		}
	}
}
