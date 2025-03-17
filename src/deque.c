#include "deque.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define NODE_CAPACITY 10

struct node {
    void* data;
    struct node* next;
    struct node* prev;
};

typedef struct node node;

static node* new_node(size_t elem_size){
    node* nd = (node*)malloc(sizeof(node));

    nd->data = (void*)malloc(elem_size*NODE_CAPACITY);
    nd->prev = nd->next = NULL;

    return nd;
}

deque* deque_create(size_t elem_size) {
    deque* dq = malloc(sizeof(deque));

    dq->elem_size = elem_size;
    dq->size = 0;

    dq->front = dq->back = new_node(elem_size);
    dq->front_data = dq->back_data = dq->front->data;

    dq->front_pos = NODE_CAPACITY/2, dq->back_pos = dq->front_pos - 1;

    return dq;
}

void deque_push_back(deque* dq, void* val) {
    if(dq->back_pos + 1 == NODE_CAPACITY){
        // add new node
        node* nd = new_node(dq->elem_size);

        dq->back->next = nd;
        nd->prev = dq->back;
        dq->back = nd;

        dq->back_data = nd->data;
        dq->back_pos = -1;
    }

    ++dq->back_pos, ++dq->size;
    void* target = (char*)dq->back_data + dq->back_pos*dq->elem_size;
    memcpy(target, val, dq->elem_size);
}

void deque_push_front(deque* dq, void* val) {
    if(dq->front_pos == 0){
        // new node
        node* nd = new_node(dq->elem_size);

        dq->front->prev = nd;
        nd->next = dq->front;
        dq->back = nd;

        dq->front_data = nd->data;
        dq->front_pos = NODE_CAPACITY;
    }

    --dq->front_pos, ++dq->size;
    void* target = (char*)dq->front_data + dq->front_pos*dq->elem_size;
    memcpy(target, val, dq->elem_size);
}

void deque_pop_back(deque* dq) {
    if(dq->size < 1) return;

    if(dq->back_pos == 0) {
        // free back node and shrink back
        if(dq->back == dq->front) {
            dq->front_pos = NODE_CAPACITY/2, dq->back_pos = dq->front_pos;
        }
        else {
            node* nd = dq->back;

            dq->back = dq->back->prev;
            dq->back->next = NULL;
            dq->back_data = dq->back->data;
            dq->back_pos = NODE_CAPACITY;

            free(nd->data);
            free(nd);
        }
    }

    --dq->back_pos, --dq->size;
}

void deque_pop_front(deque* dq) {
    if(dq->size < 1) return;

    if(dq->front_pos + 1 == NODE_CAPACITY) {
        if(dq->back == dq->front){
            dq->front_pos = NODE_CAPACITY/2, dq->back_pos = dq->front_pos;
        }
        else {
            node* nd = dq->front;

            dq->front = dq->front->next;
            dq->front->prev = NULL;
            dq->front_data = dq->front->data;
            dq->front_pos = -1;

            free(nd->data);
            free(nd);
        }
    }

    ++dq->front_pos, --dq->size;
}

void* deque_front(deque* dq) {
    if(dq->size == 0) return NULL;
    return (char*)dq->front_data + dq->front_pos*dq->elem_size;
}

void* deque_back(deque* dq) {
    if(dq->size == 0) return NULL;
    return (char*)dq->back_data + dq->back_pos*dq->elem_size;
}

void deque_destory(deque* dq) {
    while(dq->back) {
        node* nd = dq->back;
        dq->back = dq->back->prev;

        free(nd->data);
        free(nd);
    }
    dq->front = dq->front_data = dq->back = dq->back_data = NULL;

    free(dq);
}
