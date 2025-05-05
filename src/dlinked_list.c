#include <stdlib.h>

#include "dstruct/dlinked_list.h"

dlist* dlist_create() {
    dlist* list = (dlist*)malloc(sizeof(dlist));

    node* end = (node*)malloc(sizeof(node));

    if(!list || !end) {
        free(list);
        free(end);
        return nullptr;
    }

    end->next = end;
    end->prev = end;
    end->data = nullptr;

    *list = (dlist){
        .end = end,
        .size = 0
    };

    return list;
}

// insert before nd
bool dlist_insert(dlist* list, node* nd, void* val) {
    if(!nd) return false;

    node* new_node = (node*)malloc(sizeof(node));

    new_node->data = val;
    new_node->next = nd;
    new_node->prev = nd->prev;

    nd->prev->next = new_node;
    nd->prev = new_node;

    ++list->size;
    return true;
}

bool dlist_remove(dlist* list, node* nd) {
    if(!nd || nd == list->end) return false;
    nd->prev->next = nd->next;
    nd->next->prev = nd->prev;
    free(nd);
    --list->size;
    return true;
}

void dlist_destrory(dlist* list) {
    node *tmp, *tip = list->end->next;
    while(tip != list->end) {
        tmp = tip;
        tip = tip->next;
        free(tmp);
    }
    free(list->end);
    free(list);
}
