#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"

// dynamic hashtable storing blocks, use starting time as key
typedef struct hashtable {
	int size;
	int n;
	struct entry **table;
}*Hashtable;

struct entry {
	struct entry *next;
	long key;
	int length;
    int track;
};

typedef struct linkedlist{
    long start;
    int length;
    int track;
    struct linkedlist* next;
} *Linkedlist;

Linkedlist LLCreate(void);

void LLDestroy(Linkedlist);

void LLInsert(Linkedlist *ll, long start, int length, int track);

void LLRemoveHead(Linkedlist* ll);

// updates display for the block in the queue
void block_queue_update_display(Linkedlist ll, uint16_t buf[8]);

// frees used block in the queue
void block_queue_recycle(Linkedlist* ll);

// moves active block in queue
void block_queue_move_block(uint16_t buf[8]);

// activates block in queue
void block_queue_release_start(Hashtable src, long key, Linkedlist* tgt);

// Hashtable functions
Hashtable HTCreate(void);

void HTDestroy(Hashtable);

void HTInsert(Hashtable, long key, int length, int track);

struct entry* HTSearch(Hashtable, long key);

void HTRehash(Hashtable ht, long threshold);












