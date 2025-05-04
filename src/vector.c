#include "dstruct/vector.h"

#include <stdlib.h>
#include <string.h>

vector* vector_create(size_t elem_size, size_t count) {
    vector* v = malloc(sizeof(vector));

    v->size = 0;
    v->elem_size = elem_size;
    v->capacity = count;

    v->data = malloc(elem_size*v->capacity);
    return v;
}

vector* vector_duplicate(vector *v) {
    vector *new_vector = malloc(sizeof(vector));

    new_vector->size = v->size;
    new_vector->capacity = v->size;
    new_vector->elem_size = v->elem_size;

    new_vector->data = realloc(new_vector->data, new_vector->size*new_vector->elem_size);
    memcpy(new_vector->data, v->data, new_vector->size*new_vector->elem_size);

    return new_vector;
}

void vector_push_back(vector *v, void *val) {
    if(!v->data) return;

    if(v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity*v->elem_size);
    }

    void* target = (char*)v->data + v->elem_size*v->size;
    memcpy(target, val, v->elem_size);
    ++v->size;
}

void vector_append(vector *v, void *val, size_t count){
    int new_size = v->size + count;
    if(v->capacity < new_size) {
        v->capacity = new_size * 2;
        v->data = realloc(v->data, v->capacity*v->elem_size);
    }

    void* target = (char*)v->data + v->size*v->elem_size;
    memcpy(target, val, v->elem_size*count);
    v->size += count;
}

void vector_pop_back(vector *v) {
    if(!v->data) return;

    if(v->size * 2 < v->capacity) {
        v->capacity /= 2;
        v->data = realloc(v->data, v->elem_size*v->capacity);
    }

    --v->size;
}

void* vector_at(vector *v, size_t pos) {
    if(!v->data && pos >= v->size) return NULL;

    return (char*)v->data + pos*v->elem_size;
}

void* vector_back(vector* v) {
    return vector_at(v, v->size - 1);
}

void vector_destroy(vector *v) {
    free(v->data);
    free(v);
}
