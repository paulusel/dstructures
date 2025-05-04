#pragma once

#include <stddef.h>

struct vector {
    void* data;

    size_t capacity;
    size_t elem_size;
    size_t size;
};

typedef struct vector vector;

vector* vector_create(size_t elem_size, size_t count);
vector* vector_clone(vector *v);

void vector_push_back(vector *v, void *val);
void vector_pop_back(vector *v);

void* vector_at(vector *v, size_t pos);
void* vector_back(vector *v);

void vector_remove_unordered(vector *v, size_t pos);

void vector_destroy(vector *v);
