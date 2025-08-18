#include "rbtree.h"

void printNode(rbtree* r, node_t* node) {
    if (node == r->nil) {
        return;
    }

    printNode(r, node->right);
    printf("%d\n", node->key);
    printNode(r, node->left);
}


int main(int argc, char *argv[]) {
    rbtree* tree = new_rbtree();
    rbtree_insert(tree, 11);
    rbtree_insert(tree, 2);
    rbtree_insert(tree, 14);
    rbtree_insert(tree, 15);
    rbtree_insert(tree, 1);
    rbtree_insert(tree, 7);
    rbtree_insert(tree, 5);
    rbtree_insert(tree, 4);
    rbtree_insert(tree, 8);
    rbtree_insert(tree, 5);

    // node_t* findNode = rbtree_find(tree, 8);
    // printNode(tree, tree->root);
    // printf("%d", findNode->key);

    // node_t* minNode = rbtree_min(tree);
    // node_t* maxNode = rbtree_max(tree);

    // printf("%d", minNode->key);
    // printf("%d", maxNode->key);

    key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
    const size_t n = sizeof(entries) / sizeof(entries[0]);
    rbtree_to_array(tree, entries, n);

    for (int i = 0; i < n; i++) {
        printf("%d\n", entries[i]);
    }

    return 0;
}