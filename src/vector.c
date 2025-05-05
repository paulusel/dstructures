#include "dstruct/vector.h"

#include <stdlib.h>
#include <string.h>

static inline void consider_shrinking(vector* v) {
    if(v->size * 2 < v->capacity) {
        v->capacity /= 2;
        v->capacity = v->capacity < 2 ? 2 : v->capacity;
        v->data = realloc(v->data, v->elem_size*v->capacity);
    }
}

static inline void consider_expanding(vector *v) {
    if(v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity*v->elem_size);
    }
}

static inline void *pointer_to(vector *v, size_t pos) {
    return (char*)v->data + pos*v->elem_size;
}

vector* vector_create(size_t elem_size, size_t count) {
    if(elem_size < 1 || count < 0) return nullptr;

    vector* v = malloc(sizeof(vector));
    *v = (vector) {
        .size = 0,
        .elem_size = elem_size,
        .capacity = count > 2 ? count : 2,
    };
    v->data = malloc(elem_size*v->capacity);

    return v;
}

vector* vector_clone(vector *v) {
    vector *new_vector = malloc(sizeof(vector));

    *new_vector = (vector){
        .size = v->size,
        .capacity = v->size,
        .elem_size = v->elem_size
    };

    new_vector->data = realloc(new_vector->data, new_vector->size*new_vector->elem_size);
    memcpy(new_vector->data, v->data, new_vector->size*new_vector->elem_size);

    return new_vector;
}

void vector_push_back(vector *v, void *val) {
    consider_expanding(v);
    void* target = pointer_to(v, v->size);
    memcpy(target, val, v->elem_size);
    ++v->size;
}

void vector_pop_back(vector *v) {
    if(v->size == 0) return;
    --v->size;
    consider_shrinking(v);
}

void* vector_at(vector *v, size_t pos) {
    if(pos < 0 || pos >= v->size) return NULL;
    return pointer_to(v, pos);
}

void* vector_back(vector* v) {
    return pointer_to(v, v->size-1);
}

void vector_remove_unordered(vector *v, size_t pos) {
    if(pos < 0 || pos >= v->size) return;

    void *target = pointer_to(v, pos);
    void *end = pointer_to(v, v->size-1);

    if(target != end) memcpy(target, end, v->elem_size);
    --v->size;
    consider_shrinking(v);
}

void vector_destroy(vector *v) {
    free(v->data);
    free(v);
}
