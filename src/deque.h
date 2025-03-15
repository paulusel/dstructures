#pragma once

#include <stddef.h>

struct deque {
    size_t size;
    size_t elem_size;

    struct node* front;
    struct node* back;

    void* front_data;
    void* back_data;

    size_t front_pos;
    size_t back_pos;
};

typedef struct deque deque;

void deque_init(deque* dq, size_t elem_size);

void deque_push_back(deque* dq, void* val);
void deque_push_front(deque* dq, void* val);

void deque_pop_back(deque* dq);
void deque_pop_front(deque* dq);

void* deque_front(deque* dq);
void* deque_back(deque* dq);

void deque_destory(deque* dq);
