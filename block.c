#include "block.h"
#include "delay.h"
#include "init.h"

extern volatile timer_t current_time;

void LLInsert(Linkedlist* ll, long start, int length, int track){
    Linkedlist tmp = *ll;
    Linkedlist new = (Linkedlist)malloc(sizeof(struct linkedlist));
	new->start = start;
  new->length = length;
  new->track = track;
  new->next = NULL;
    if (tmp == NULL) {
        *ll = new;
    } else {
        while (tmp->next != NULL) {tmp = tmp->next;}
        tmp->next = new;
    }
	
}

void LLRemoveHead(Linkedlist* ll) {
    Linkedlist tmp = *ll;
    if (tmp != NULL){
        *ll = tmp->next;
        tmp->next = NULL;
    }
    
    free(tmp);
}

#define INITIAL_SIZE (14)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

Hashtable internalHTCreate(int size)
{
	Hashtable ht;
	int i;
	
	ht = malloc(sizeof(*ht));
	
	ht->size = size;
	ht->n = 0;
	ht->table = malloc(sizeof(struct entry*) * ht->size);
	
	for (i=0; i < ht->size; i++) ht->table[i] = 0;
	
	return ht;
}

Hashtable HTCreate(void){
	return internalHTCreate(INITIAL_SIZE);
}

void HTDestroy(Hashtable ht){
	int i;
	struct entry *e;
	struct entry *next;
	
	for (i=0; i < ht->size; i++) {
		for (e = ht->table[i]; e != 0; e = next) {
			next = e->next;
			
			free(e);
		}
	}
	
	free(ht->table);
	free(ht);
}

void grow(Hashtable ht){
	Hashtable ht2;
	struct hashtable swap;
	int i;
	struct entry *e;
	ht2 = internalHTCreate(ht->size*GROWTH_FACTOR);
	
	for(i=0; i < ht->size; i++){
		for(e = ht->table[i]; e != 0; e=e->next) {
			if (e->key > val(&current_time)){
				HTInsert(ht2, e->key, e->length, e->track);
			}
		}
	}
	
	swap = *ht;
	*ht = *ht2;
	*ht2 = swap;
	
	HTDestroy(ht2);
}

void HTInsert(Hashtable ht, long key, int length, int track)
{
	int probe;
	struct entry* e;
	
	
	probe = key % ht->size;
	
	while(1){
		
		if (ht->table[probe] == 0){
			break;
		} else if (ht->table[probe]->key == key) {
			break;
		}
		probe = (probe+1) % ht->size;
	}
	e = malloc(sizeof(*e));
	e->key = key;
	e->length = length;
    e->track = track;
	
	e->next = ht->table[probe];
	ht->table[probe] = e;
	
	ht->n++;
	
	if (ht->n >= ht->size * MAX_LOAD_FACTOR) {grow(ht);}
}

struct entry* HTSearch(Hashtable ht, long key){
	int probe;
	probe = key % ht->size;
	while(1){
		if (ht->table[probe] == 0){
			return 0;
		} else if (ht->table[probe]->key == key) {
			break;
		}
		probe = (probe+1) % ht->size;
	}
	
	return ht->table[probe];
}

void HTDelete(Hashtable ht, long key){
	struct entry *e;
	struct entry *next;
	int probe;
	
	probe = key % ht->size;
	while(1){
		if (ht->table[probe] == 0){
			return;
		} else if (ht->table[probe]->key == key) {
			break;
		}
		probe = (probe+1) % ht->size;
	}
	e = ht->table[probe];
    
	while(e){
		next = e->next;
		free(e);
		e = next;
	}
    
    //ht->table[probe] = 0;
	ht->n--;
}

void block_queue_release_start(Hashtable src, long key, Linkedlist* tgt){
	struct entry *tmp;
	tmp = HTSearch(src, key);
	if (tmp == 0) return;
	// else
	while (tmp != 0) {
    LLInsert(tgt, key, tmp->length, tmp->track);
		tmp = tmp->next;
	}
	//HTDelete(src, key);
}

void block_queue_move_block(uint16_t buf[8]){
	buf_move_left(buf);
}

void block_queue_update_display(Linkedlist ll, uint16_t buf[8]){
	Linkedlist tmp = ll;
    while(tmp){
        if (tmp->length > 0) {
            //buf[tmp->track*2] |= 0x1;
            //buf[tmp->track*2+1] |= 0x1;
          if (tmp->track == 1) {
						buf[1] |= 0x1;
						buf[2] |= 0x1;
					} else {
						buf[5] |= 0x1;
						buf[6] |= 0x1;
					}						
					tmp->length--;
        }
        tmp = tmp->next;
    }
}

void block_queue_recycle(Linkedlist* ll){
    if(ll){
        while((*ll)->length == 0){
            LLRemoveHead(ll);
        }
    }
}

void HTRehash(Hashtable ht, long threshold){
	Hashtable ht2;
	struct hashtable swap;
	int i;
	struct entry *e;
	ht2 = internalHTCreate(ht->size);
	
	for(i=0; i < ht->size; i++){
		for(e = ht->table[i]; e != 0; e=e->next) {
			if (e->key >= threshold){
				HTInsert(ht2, e->key, e->length, e->track);
			}
		}
	}
	
	swap = *ht;
	*ht = *ht2;
	*ht2 = swap;
	
	HTDestroy(ht2);
}









