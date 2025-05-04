#include <stddef.h>

typedef struct htable htable;

htable* htable_create(size_t elem_size, unsigned int (*hash_function)(const void *elem), int (*compare)(const void* elem1, const void* elem2));

void* htable_insert(htable *ht, const void *val);

void *htable_search(const htable *ht, const void *val);

void htable_remove(htable *ht, const void *val);

size_t htable_size(const htable *ht);

void htable_destroy(htable *ht);
