#include "trade.h"

void free_tree(struct avl_tree *tree)
{
    struct avl_node *cur;
    struct kv_node *node;

    cur = avl_first(tree);
    while(cur) {
        node = _get_entry(cur, struct kv_node, avl);
        // printf("remove key %d, value %d\n", node->key, node->value);
        cur = avl_next(cur);
        avl_remove(tree, &node->avl);
        free(node->key);
        free(node->value->prici);
        free(node->value);
        free(node);
    }
}

void free_tree1(struct avl_tree *tree)
{
    struct avl_node *cur;
    struct kv_node1 *node;

    cur = avl_first(tree);
    while(cur) {
        node = _get_entry(cur, struct kv_node1, avl);
        // printf("remove key %d, value %d\n", node->key, node->value);
        cur = avl_next(cur);
        avl_remove(tree, &node->avl);
        free(node->value);
        free(node);
    }
}