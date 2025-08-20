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

    return 0;
}