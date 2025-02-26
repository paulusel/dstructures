#include "htable.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const unsigned int hash_table_primes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
    196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
    402653189, 805306457, 1610612741 };

inline static uint32_t index_of(htable *ht, void *val) {
    uint32_t hash = ht->hash_function(val);
    return hash % ht->table_size;
}

static void htable_grow(htable *ht){
    uint32_t old_table_size = ht->table_size;
    unsigned char *old_data = ht->data;
    unsigned char *old_status = ht->status;

    ++ht->prime_indx;
    ht->table_size = hash_table_primes[ht->prime_indx];
    ht->size = 0;

    ht->data = malloc(ht->table_size * ht->elem_size);
    ht->status  = calloc(ht->table_size, ht->elem_size);

    void *dest, *src;
    for(uint32_t i = 0; i<old_table_size; ++i) {
        if(old_status[i] == HTABLE_SLOT_OCCUPPIED){
            src = old_data + i * ht->elem_size;
            dest = htable_insert(ht, src);
            memcpy(dest, src, ht->elem_size);
        }
    }

    free(old_data);
    free(old_status);
}

//static void htable_shrink(htable *ht){ }

void htable_init(htable *ht, size_t elem_size, unsigned int (*hash_function)(void *key),
                 int (*compare)(const void*, const void*))
{
    ht->elem_size = elem_size;
    ht->hash_function = hash_function;
    ht->compare = compare;

    ht->size = 0;
    ht->prime_indx = 0;
    ht->table_size = hash_table_primes[ht->prime_indx];

    ht->data = malloc(ht->elem_size * ht->table_size);
    ht->status = calloc(ht->table_size, ht->elem_size);
    ht->temp = malloc(2*ht->elem_size);
}

void *htable_insert(htable *ht, void *val) {
    if((double)ht->size/ht->table_size > 0.7) htable_grow(ht);

    memcpy(ht->temp, val, ht->elem_size);

    unsigned char *target, *pos = NULL, *temp = ht->temp + ht->elem_size;
    uint32_t indx = index_of(ht, ht->temp), probe = 0, desired_indx, offset;

    for(;;){
        target = ht->data + indx * ht->elem_size;
        if(ht->status[indx] != HTABLE_SLOT_OCCUPPIED){
            ht->status[indx] = HTABLE_SLOT_OCCUPPIED;
            ++ht->size;

            if(pos) {
                memcpy(target, ht->temp, ht->elem_size);
                return pos;
            }

            return target;
        }

        if(ht->compare(ht->temp, target) == 0) return target;

        desired_indx = index_of(ht, target);
        offset = (indx - desired_indx + ht->table_size) % ht->table_size;

        if(probe > offset) {
            if(!pos) pos = target;
            memcpy(temp, target, ht->elem_size);
            memcpy(target, ht->temp, ht->elem_size);
            memcpy(ht->temp, temp, ht->elem_size);
            probe = offset;
        }

        ++probe;
        indx = (indx + 1) % ht->table_size;
    }
}

void *htable_search(htable *ht, void *val) {
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
    uint32_t desired_indx, next_indx;

    current = ht->data + indx * ht->elem_size;

    for(;;){
        next_indx = (indx + 1) % ht->table_size;
        if(ht->status[next_indx] != HTABLE_SLOT_OCCUPPIED) break;

        next = ht->data + next_indx * ht->elem_size;
        desired_indx = index_of(ht, next);
        if(desired_indx == next_indx) break;

        memcpy(current, next, ht->elem_size);

        indx = next_indx;
        current = next;
    }
    ht->status[indx] = 0; // free
}

void htable_remove(htable *ht, void *val) {
    unsigned char *pos = htable_search(ht, val);
    if(pos) {
        htable_delete(ht, (pos - ht->data)/ht->elem_size);
        --ht->size;
    }
}

void htable_destroy(htable *ht) {
    free(ht->data);
    free(ht->status);
    free(ht->temp);
}
