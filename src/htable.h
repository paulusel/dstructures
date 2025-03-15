#include <stddef.h>

enum htable_bucket_status : unsigned char { HTABLE_SLOT_OCCUPPIED = 0xFFU };

typedef struct {
    int size;
    size_t elem_size;
    int table_size;

    unsigned int (*hash_function)(void* key);
    int (*compare)(const void* key1, const void* key2);

    unsigned char *data;
    unsigned char *status;// 0 - free, 1 = occuppied

    struct htable_private_data *p_data;
} htable;

void htable_init(htable *ht, size_t elem_size,
                 unsigned int (*hash_function)(void *key),
                 int (*compare)(const void*, const void*));

void* htable_insert(htable *ht, void *val);

void *htable_search(htable *ht, void *val);

void htable_remove(htable *ht, void *val);

void htable_destroy(htable *ht);
