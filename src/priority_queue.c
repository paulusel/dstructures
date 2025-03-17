#include "priority_queue.h"

#include <stdlib.h>
#include <string.h>

static void heapify(priority_queue *que, size_t indx){
    void* temp = malloc(que->elem_size);
    char *data = que->data, *end = data + que->elem_size*que->size, *i = data+indx*que->elem_size;
    for(char* l, *r, *t = i; ; i = t) {

        l = data + 2*(i-data) + que->elem_size, r = l + que->elem_size;

        if(l < end && que->cmp(l, i) == 1) t = l;
        if(l < end && que->cmp(r, t) == 1) t = r;

        if(t == i) break;

        // swap target and i
        memcpy(temp, t, que->elem_size);
        memcpy(t, i, que->elem_size);
        memcpy(i, temp, que->elem_size);
    }
    free(temp);
}

void priority_queue_push(priority_queue *que, void *val) {
    if(!que->data) return;

    if(que->size == que->capacity) {
        que->capacity *= 2;
        que->data =realloc(que->data, que->capacity*que->elem_size);
    }

    void *temp = malloc(que->elem_size);
    char *root = que->data, *curr = root + que->size*que->elem_size, *prnt; // incomplete

    // copy the value to the end
    memcpy(curr, val, que->elem_size);

    for(int i = que->size; i > 0; curr = prnt){
        i = (i-1)/2;
        prnt = root + i*que->elem_size;

        if(que->cmp(prnt, curr) > 0) break;

        // swap curr and prnt
        memcpy(temp, prnt, que->elem_size);
        memcpy(prnt, curr, que->elem_size);
        memcpy(curr, temp, que->elem_size);
    }

    ++que->size;
    free(temp);
}

void priority_queue_pop(priority_queue *que){
    if(!que->data || que->size == 0) return;

    --que->size;

    if(que->capacity > 20 && que->size * 2 < que->capacity) {
        que->capacity /= 2;
        que->data = realloc(que->data, que->capacity*que->elem_size);
    }

    void *end = (char*)que->data + (que->size)*que->elem_size;
    memcpy(que->data, end, que->elem_size);

    if(que->size > 1) heapify(que, 0);
}

priority_queue* priority_queue_create(size_t elem_size, int (*cmp)(const void*, const void*), size_t count) {
    priority_queue *q = malloc(sizeof(priority_queue));

    q->size = 0;
    q->capacity = count;
    q->elem_size = elem_size;
    q->cmp = cmp;

    q->data = malloc(q->capacity*q->elem_size);

    return q;
}

void priority_queue_destroy(priority_queue *q){
    if(q->data) {
        free(q->data);
        q->data = NULL;
    }
    free(q);
}
