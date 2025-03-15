#include <stdlib.h>

typedef struct {
    void* data;
    int (*cmp)(const void*, const void*);

    size_t elem_size;
    size_t size;
    size_t capacity;
} priority_queue;

void priority_queue_init(priority_queue *q, size_t eleme_size, int (*cmp)(const void*, const void*), size_t count);
void priority_queue_destroy(priority_queue *q);

void priority_queue_push(priority_queue *q, void *val);
void priority_queue_pop(priority_queue *q);
