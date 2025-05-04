#include "dstruct/htable.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char HTABLE_SLOT_OCCUPPIED = 0xFFU;

static const unsigned int hash_table_primes[] = {
    53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739,
    6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741
};

struct htable {
    int size;
    size_t elem_size;
    int table_size;

    unsigned int (*hash_function)(const void* elem);
    int (*compare)(const void* elem1, const void* elem2);

    unsigned char *data;
    unsigned char *status;// 0 - free, 1 = occuppied

    unsigned char *temp1, *temp2;
    int prime_indx;
};

inline static uint32_t index_of(const htable *ht, const void *val) {
    uint32_t hash = ht->hash_function(val);
    return hash % ht->table_size;
}

static void htable_grow(htable *ht){
    uint32_t old_table_size = ht->table_size;
    unsigned char *old_data = ht->data;
    unsigned char *old_status = ht->status;

    ++ht->prime_indx;
    ht->table_size = hash_table_primes[ht->prime_indx];
    ht->data = malloc(ht->table_size * ht->elem_size);
    ht->status  = calloc(ht->table_size, ht->elem_size);
    ht->size = 0;

    for(uint32_t i = 0; i<old_table_size; ++i) {
        if(old_status[i] == HTABLE_SLOT_OCCUPPIED){
            htable_insert(ht, old_data + i*ht->elem_size);
        }
    }

    free(old_data);
    free(old_status);
}

size_t htable_size(const htable *ht) {
    return ht->size;
}

//static void htable_shrink(htable *ht){ }

htable* htable_create(size_t elem_size, unsigned int (*hash_function)(const void*), int (*compare)(const void*, const void*)) {
    htable *ht = malloc(sizeof(htable));

    ht->elem_size = elem_size;
    ht->hash_function = hash_function;
    ht->compare = compare;

    ht->size = 0;
    ht->prime_indx = 0;
    ht->table_size = hash_table_primes[ht->prime_indx];

    ht->data = malloc(ht->elem_size * ht->table_size);
    ht->status = calloc(ht->table_size, ht->elem_size);
    ht->temp1 = malloc(ht->elem_size);
    ht->temp2 = malloc(ht->elem_size);

    return ht;
}

void* htable_insert(htable *ht, const void *val) {
    if((double)ht->size/ht->table_size > 0.7) htable_grow(ht);

    memcpy(ht->temp1, val, ht->elem_size);

    unsigned char *target;
    uint32_t indx = index_of(ht, ht->temp1), probe = 0, desired_indx, offset;

    for(;;){
        target = ht->data + indx * ht->elem_size;
        if(ht->status[indx] != HTABLE_SLOT_OCCUPPIED ) {
            memcpy(target, ht->temp1, ht->elem_size);
            ht->status[indx] = HTABLE_SLOT_OCCUPPIED;
            ++ht->size;
            return NULL;
        }

        if(ht->compare(ht->temp1, target) == 0) {
            memcpy(ht->temp2, target, ht->elem_size);
        }

        desired_indx = index_of(ht, target);
        offset = (indx - desired_indx + ht->table_size) % ht->table_size;

        if(probe > offset) {
            memcpy(ht->temp2, target, ht->elem_size);
            memcpy(target, ht->temp1, ht->elem_size);
            memcpy(ht->temp1, ht->temp2, ht->elem_size);
            probe = offset;
        }

        ++probe;
        indx = (indx + 1) % ht->table_size;
    }

}

void *htable_search(const htable *ht, const void *val) {
    uint32_t indx = index_of(ht, val), probe = 0;

    for(;;){
        if(ht->status[indx] != HTABLE_SLOT_OCCUPPIED) return NULL;

        unsigned char *current = ht->data + indx * ht->elem_size;
        if(ht->compare(current, val) == 0) return current;

        uint32_t desired_indx = index_of(ht, current);
        uint32_t offset = (indx - desired_indx + ht->table_size) % ht->table_size;

        if(offset < probe) return NULL;

        ++probe;
        indx = (indx + 1) % ht->table_size;
    }
}

static void htable_delete(htable *ht, int indx){
    unsigned char *current, *next;
    uint32_t desired_next_indx, next_indx;

    current = ht->data + indx * ht->elem_size;

    for(;;){
        next_indx = (indx + 1) % ht->table_size;
        if(ht->status[next_indx] != HTABLE_SLOT_OCCUPPIED) break;

        next = ht->data + next_indx * ht->elem_size;
        desired_next_indx = index_of(ht, next);
        if(desired_next_indx == next_indx) break;

        memcpy(current, next, ht->elem_size);

        indx = next_indx;
        current = next;
    }
    ht->status[indx] = 0; // free
}

void htable_remove(htable *ht, const void *val) {
    unsigned char *pos = htable_search(ht, val);
    if(pos) {
        htable_delete(ht, (pos - ht->data)/ht->elem_size);
        --ht->size;
    }
}

void htable_destroy(htable *ht) {
    free(ht->data);
    free(ht->status);
    free(ht->temp1);
    free(ht->temp2);
    free(ht);
}
