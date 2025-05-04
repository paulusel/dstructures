#include <stdlib.h>

#include "dstruct/dlinked_list.h"

dlist* dlist_create() {
    dlist* list = (dlist*)malloc(sizeof(dlist));

    node* head = (node*)malloc(sizeof(node));
    node* tail = (node*)malloc(sizeof(node));

    if(!list || !head || !tail) {
        free(list);
        free(head);
        free(tail);
        return nullptr;
    }

    head->next = tail;
    tail->prev = head;
    head->prev = tail->next = nullptr;

    list->head = head;
    list->tail = tail;

    return list;
}

// insert before nd
bool dlist_insert(dlist* list, node* nd, void* val) {
    if(!nd || nd == list->head) return false;

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
    if(!nd || nd == list->head || nd == list->tail) return false;
    nd->prev->next = nd->next;
    nd->next->prev = nd->prev;
    free(nd);

    --list->size;
    return true;
}

void dlist_destrory(dlist* list) {
    node *tmp, *tip = list->head;
    while(tip) {
        tmp = tip;
        tip = tip->next;
        free(tmp);
    }
    free(list);
}
