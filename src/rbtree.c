#include "rbtree.h"

#include <stdlib.h>
#include <string.h>

typedef void(*rotation)(rbtree*, rbtree_node*);

static void left_rotate(rbtree *tree, rbtree_node *nd) {
    rbtree_node *right = nd->right;

    nd->right = right->left;
    if(right->left != tree->sentinel) right->left->parent = nd;
    right->parent = nd->parent;

    if(nd->parent == tree->sentinel) tree->root = right;
    else if(nd == nd->parent->left) nd->parent->left = right;
    else nd->parent->right = right;

    right->left = nd;
    nd->parent = right;
}

static void right_rotate(rbtree *tree, rbtree_node *nd) {
    rbtree_node *left = nd->left;

    nd->left = left->right;
    if(left->right != tree->sentinel) left->right->parent = nd;
    left->parent = nd->parent;

    if(nd->parent == tree->sentinel) tree->root = left;
    else if(nd == nd->parent->right) nd->parent->right = left;
    else nd->parent->left = left;

    left->right = nd;
    nd->parent = left;
}

static void rbtree_insert_fixup_case_one(rbtree_node *prnt, rbtree_node *uncle){
    prnt->color = RBTREE_COLOR_BLACK;
    uncle->color = RBTREE_COLOR_BLACK;
    prnt->parent->color = RBTREE_COLOR_RED;
}

static void rbtree_insert_fixup_case_three(rbtree *tree, rbtree_node *prnt, rotation rotate){
    prnt->color = RBTREE_COLOR_BLACK;
    prnt->parent->color = RBTREE_COLOR_RED;
    rotate(tree, prnt->parent);
}

static void rbtree_insert_fixup(rbtree *tree, rbtree_node *nd) {
    for(rbtree_node *uncle, *prnt = nd->parent; prnt->color == RBTREE_COLOR_RED; prnt = nd->parent) {
        if(prnt == prnt->parent->left) { // parent is on the left of grandparent
            uncle = prnt->parent->right;
            if(uncle->color == RBTREE_COLOR_RED) { // CASE 1: both parent and uncle are black
                rbtree_insert_fixup_case_one(prnt, uncle);
                nd = prnt->parent;
            }
            else {
                if(nd == prnt->right) { // CASE 2: current node is located on the right of parent
                    nd = prnt;
                    left_rotate(tree, nd);
                    prnt = nd->parent;
                }
                rbtree_insert_fixup_case_three(tree, prnt, right_rotate); // CASE 3
            }
        }
        else { // parent is located on the right of grandparent
            uncle = prnt->parent->left;
            if(uncle->color == RBTREE_COLOR_RED) {
                rbtree_insert_fixup_case_one(prnt, uncle);
                nd = prnt->parent;
            }
            else {
                if(nd == prnt->left) { // current node is located on the left of the parent
                    nd = prnt;
                    right_rotate(tree, nd);
                    prnt = nd->parent;
                }
                rbtree_insert_fixup_case_three(tree, prnt, left_rotate);
            }
        }
    }
    tree->root->color = RBTREE_COLOR_BLACK;
}

static void rbtree_transplant_node(rbtree *tree, rbtree_node *a, rbtree_node *b){ // node a is replaced with b
    rbtree_node *prnt = a->parent;

    if(prnt == tree->sentinel) tree->root = b;
    else if(prnt->left == a) prnt->left = b;
    else prnt->right = b;

    b->parent = prnt;
}

static void rbtree_delete_fixup_case_one(rbtree *tree, rbtree_node *nd, rbtree_node *sibling, rotation rotate){
    sibling->color = RBTREE_COLOR_BLACK;
    nd->parent->color = RBTREE_COLOR_RED;
    rotate(tree, nd->parent);
}

static void rbtree_delete_fixup_case_three(rbtree *tree, rbtree_node *sibling, char side, rotation rotate) {
    if(side) sibling->right->color = RBTREE_COLOR_BLACK;
    else sibling->left->color = RBTREE_COLOR_BLACK;
    sibling->color = RBTREE_COLOR_RED;
    rotate(tree, sibling);
}

static void rbtree_delete_fixup_case_four(rbtree *tree, rbtree_node *nd, rbtree_node *sibling, char side, rotation rotate) {
    sibling->color = nd->parent->color;
    nd->parent->color = RBTREE_COLOR_BLACK;
    if(side) sibling->right->color = RBTREE_COLOR_BLACK;
    else sibling->left->color = RBTREE_COLOR_BLACK;
    rotate(tree, nd->parent);
}

static void rbtree_delete_fixup(rbtree *tree, rbtree_node *nd){
    rbtree_node *sibling;
    while(nd != tree->root && nd->color == RBTREE_COLOR_BLACK){
        if(nd == nd->parent->left){ // node is one the left
            sibling = nd->parent->right;
            if(sibling->color == RBTREE_COLOR_RED) { // case 1
                rbtree_delete_fixup_case_one(tree, nd, sibling, left_rotate); // case 1
                sibling = nd->parent->right;
            }

            if(sibling->left->color == RBTREE_COLOR_BLACK && sibling->right->color == RBTREE_COLOR_BLACK){ // case 2
                sibling->color = RBTREE_COLOR_RED;
                nd = nd->parent;
            }
            else {
                if(sibling->right->color == RBTREE_COLOR_BLACK) { // case 3
                    rbtree_delete_fixup_case_three(tree, sibling, 0, right_rotate);
                    sibling = nd->parent->right;
                }
                rbtree_delete_fixup_case_four(tree, nd, sibling, 1, left_rotate); // case 4
                nd = tree->root;
            }
        }
        else { // node is on the right
            sibling = nd->parent->left;
            if(sibling->color == RBTREE_COLOR_RED) { // case 1
                rbtree_delete_fixup_case_one(tree, nd, sibling, right_rotate); // case 1
                sibling = nd->parent->left;
            }

            if(sibling->right->color == RBTREE_COLOR_BLACK && sibling->left->color == RBTREE_COLOR_BLACK){ // case 2
                sibling->color = RBTREE_COLOR_RED;
                nd = nd->parent;
            }
            else {
                if(sibling->left->color == RBTREE_COLOR_BLACK) { // case 3
                    rbtree_delete_fixup_case_three(tree, sibling, 1, left_rotate);
                    sibling = nd->parent->left;
                }
                rbtree_delete_fixup_case_four(tree, nd, sibling, 0, right_rotate); // case 4
                nd = tree->root;
            }
        }
    }
    nd->color = RBTREE_COLOR_BLACK;
}

static rbtree_node* rbtree_min(rbtree *tree, rbtree_node *nd) {
    if(nd == tree->sentinel) return NULL;
    while(nd->left != tree->sentinel) nd = nd->left;
    return nd;
}

static rbtree_node* rbtree_max(rbtree *tree, rbtree_node *nd) {
    if(nd == tree->sentinel) return NULL;
    while(nd->right != tree->sentinel) nd = nd->right;
    return nd;
}

//void rbtree_init( size_t elem_size, int (*cmp)(const void* a, const void* b)){
rbtree* rbtree_create(size_t elem_size, int (*cmp)(const void* a, const void* b)){
    rbtree *tree = malloc(sizeof(rbtree));

    tree->sentinel = (rbtree_node*)malloc(sizeof(rbtree_node));
    tree->sentinel->color = RBTREE_COLOR_BLACK;

    tree->cmp = cmp;
    tree->root = tree->sentinel;
    tree->elem_size = elem_size;
    tree->size = 0;

    return tree;
}

rbtree_node* rbtree_search(rbtree *tree, void *val) {
    rbtree_node *nd = tree->root;
    while(nd != tree->sentinel) {
        int res = tree->cmp(val, nd->data);
        if(res > 0) nd = nd->right;
        else if(res < 0) nd = nd->left;
        else return nd;
    }

    return NULL;
}

rbtree_node *rbtree_front(rbtree *tree) {
    return rbtree_min(tree, tree->root);
}

rbtree_node *rbtree_back(rbtree *tree) {
    return rbtree_max(tree, tree->root);
}

rbtree_node* rbtree_next(rbtree *tree, rbtree_node *nd) {
    if(nd->right == tree->sentinel) {
        rbtree_node *prnt = nd->parent;
        while(prnt != tree->sentinel && nd == prnt->right) {
            nd = prnt;
            prnt = nd->parent;
        }
        return prnt == tree->sentinel ? NULL : prnt;
    }
    else {
        for(nd = nd->right; nd->left != tree->sentinel; ) nd = nd->left;
        return nd;
    }
}

rbtree_node* rbtree_prev(rbtree *tree, rbtree_node *nd) {
    if(nd->left == tree->sentinel) {
        rbtree_node *prnt = nd->parent;
        while(prnt != tree->sentinel && nd == prnt->left) {
            nd = prnt;
            prnt = nd->parent;
        }
        return prnt == tree->sentinel ? NULL : prnt;
    }
    else {
        for(nd = nd->left; nd->right != tree->sentinel; ) nd = nd->right;
        return nd;
    }
}

// @return NULL if new value is inserted. Otherwise pointer to existing node.
void* rbtree_insert(rbtree *tree, void *val) {
    rbtree_node *nd = tree->root, *prnt = tree->sentinel;
    int cmp_result = 1;
    while(nd != tree->sentinel) {
        prnt = nd;

        cmp_result = tree->cmp(val, nd->data);
        if(cmp_result > 0) nd = nd->right;
        else if(cmp_result < 0) nd = nd->left;
        else break;
    }

    if(cmp_result == 0) return prnt->data; // no duplicates

    rbtree_node *new_node = (rbtree_node*)malloc(sizeof(rbtree_node));
    new_node->right = new_node->left = tree->sentinel;
    new_node->color = RBTREE_COLOR_RED;

    new_node->data = malloc(tree->elem_size);
    memcpy(new_node->data, val, tree->elem_size);

    new_node->parent = prnt;

    if(prnt == tree->sentinel) tree->root = new_node;
    else if(cmp_result > 0) prnt->right = new_node;
    else prnt->left = new_node;

    ++tree->size;

    rbtree_insert_fixup(tree, new_node);
    return NULL;
}

void rbtree_delete(rbtree *tree, rbtree_node *nd) {
    rbtree_node *node = nd;
    rbtree_node_color original_color = nd->color;

    if(nd->left == tree->sentinel) {
        node = nd->right;
        rbtree_transplant_node(tree, nd, nd->right);
    }
    else if(nd->right == tree->sentinel) {
        node = nd->left;
        rbtree_transplant_node(tree, nd, nd->left);
    }
    else {
        rbtree_node *nxt = rbtree_min(tree, nd->right);
        original_color = nxt->color;
        node = nxt->right;

        if(nxt != nd->right) {
            rbtree_transplant_node(tree, nxt, nxt->right);
            nxt->right = nd->right;
            nd->right->parent = nxt;
        }
        else node->parent = nxt;

        rbtree_transplant_node(tree, nd, nxt);
        nxt->left = nd->left;
        nd->left->parent = nxt;
        nxt->color = nd->color;
    }

    if(original_color == RBTREE_COLOR_BLACK) rbtree_delete_fixup(tree, node);

    free(nd->data);
    free(nd);

    --tree->size;
}

void rbtree_remove(rbtree *tree, void *val) {
    rbtree_node *nd = rbtree_search(tree, val);
    if(nd) rbtree_delete(tree, nd);
}

void rbtree_destroy(rbtree *tree) {
    for(rbtree_node *node, *nd = tree->root; nd != tree->sentinel;) {
        node = nd;
        if(nd->left != tree->sentinel) {
            nd = nd->left;
            node->left = tree->sentinel;
            continue;
        }

        if(nd->right != tree->sentinel) {
            nd = nd->right;
            node->right = tree->sentinel;
            continue;
        }

        nd = nd->parent;

        free(node->data);
        free(node);
    }

    free(tree->sentinel);

    tree->sentinel = NULL;
    tree->root = NULL;
    free(tree);
}
