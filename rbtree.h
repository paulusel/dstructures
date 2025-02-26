#include <stddef.h>

enum _rbtree_node_color { RBTREE_COLOR_RED, RBTREE_COLOR_BLACK };
typedef enum _rbtree_node_color rbtree_node_color;

struct _node {
    struct _node *left;
    struct _node *parent;
    struct _node *right;
    void* data;
    rbtree_node_color color;
};
typedef struct _node rbtree_node;

struct _rbtree {
    rbtree_node* root;
    rbtree_node* sentinel;
    int (*cmp)(const void* a, const void* b);

    size_t size;
    size_t elem_size;
};
typedef struct _rbtree rbtree;

void rbtree_init(rbtree *tree, size_t elem_size, int (*cmp)(const void* a, const void* b));

void* rbtree_insert(rbtree *tree, void *val);
rbtree_node* rbtree_search(rbtree *tree, void *val);

rbtree_node* rbtree_front(rbtree *tree);
rbtree_node* rbtree_back(rbtree *tree);

rbtree_node* rbtree_next(rbtree *tree, rbtree_node *nd);
rbtree_node* rbtree_prev(rbtree *tree, rbtree_node *nd);

void rbtree_remove(rbtree *tree, void *val);
void rbtree_delete(rbtree *tree, rbtree_node *nd);

void rbtree_destroy(rbtree *tree);
