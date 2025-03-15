#pragma once

#include <stddef.h>

struct vector {
    void* data;

    size_t capacity;
    size_t elem_size;
    size_t size;
};

typedef struct vector vector;

void vector_init(vector *v, size_t elem_size, size_t count);
void vector_copy(vector *src, vector *dst);

void vector_push_back(vector *v, void *val);
void vector_pop_back(vector *v);
void vector_append(vector *v, void *val, size_t count);

void* vector_at(vector *v, size_t pos);
void* vector_back(vector *v);

void vector_destroy(vector *v);
