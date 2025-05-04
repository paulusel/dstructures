#include <stddef.h>

struct node {
    struct node *prev;
    void *data;
    struct node *next;
};

typedef struct node node;

typedef struct {
    node* head;
    node* tail;
    size_t size;
} dlist;

dlist* dlist_create();
bool dlist_insert(dlist* list, node* nd, void* val);
bool dlist_remove(dlist* list, node* nd);
void dlist_destrory(dlist* list);
