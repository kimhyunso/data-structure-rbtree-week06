#include "rbtree.h"

rbtree* new_rbtree(void) {
  	rbtree *p = (rbtree*)calloc(1, sizeof(rbtree));
  	node_t* nil = calloc(1, sizeof(node_t));
	nil->color = RBTREE_BLACK;
	nil->parent = nil;
	nil->left = nil;
	nil->right = nil;

    p->root = nil;
    p->nil = nil;

  	return p;
}

static void delete_nodes(rbtree* t, node_t* n) {
	if (n == t->nil) return;
	delete_nodes(t, n->left);
	delete_nodes(t, n->right);
	free(n);
}

void delete_rbtree(rbtree *t) {
	delete_nodes(t, t->root);
 	free(t->nil);
	free(t);
}

static void left_rotate(rbtree* t, node_t* x) {
	if (x == t->nil || x->right == t->nil) return;
    node_t* y = x->right; 
    x->right = y->left; // #1

	if (y->left != t->nil) // #2
        y->left->parent = x;

    y->parent = x->parent; // #3
    if (x->parent == t->nil)	
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x; // #4
    x->parent = y; // #5
}

static void right_rotate(rbtree *t, node_t *x) {
	if (x == t->nil || x->left == t->nil) return; 
	node_t *y = x->left;
    x->left = y->right;
    if (y->right != t->nil)
        y->right->parent = x;

    y->parent = x->parent;
    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

static void rbtree_insert_fixup(rbtree *t, node_t *z) {
    while (z->parent->color == RBTREE_RED) {
        if (z->parent == z->parent->parent->left) {
            node_t *y = z->parent->parent->right;  // 삼촌 노드
            if (y->color == RBTREE_RED) {
                // Case 1: 삼촌도 빨강
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Case 2: 삼촌은 검정, z가 오른쪽 자식
                    z = z->parent;
                    left_rotate(t, z);
                }
                // Case 3: 삼촌은 검정, z가 왼쪽 자식
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        } else {
            // 대칭적인 경우 (z->parent == z->parent->parent->right)
            node_t *y = z->parent->parent->left;  // 삼촌 노드
            if (y->color == RBTREE_RED) {
                // Case 1
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // Case 2
                    z = z->parent;
                    right_rotate(t, z);
                }
                // Case 3
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t* rbtree_insert(rbtree* t, const key_t key) {
	node_t* z = calloc(1, sizeof(node_t));
    z->key = key;
    z->color = RBTREE_RED;
    z->left = z->right = z->parent = t->nil;

    node_t *y = t->nil;
    node_t *x = t->root;

    while (x != t->nil) {
        y = x;
        if (z->key <= x->key)
            x = x->left;
        else
            x = x->right;  // multiset을 허용하면 <= 로 넣어도 됨
    }

    z->parent = y;
    if (y == t->nil) {
        t->root = z;  
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    rbtree_insert_fixup(t, z);
    return z;
}

node_t* rbtree_find(const rbtree* t , const key_t key) {
	if(t == NULL) return NULL;
	node_t* cur = t->root;
	while (cur != t->nil) {
		if (cur->key == key) {
			return cur;
		}
		cur = key < cur->key ? cur->left : cur->right;
	}
	return NULL;
}

node_t* rbtree_min(const rbtree *t) {
	if (t->root == t->nil) {
        return NULL;
    }

	node_t* head = t->root;
	while (head->left != t->nil) {
		head = head->left;
	}
  	return head;
}

node_t* rbtree_max(const rbtree *t) {
	if (t->root == t->nil) {
        return NULL;
    }

	node_t* head = t->root;
	while (head->right != t->nil) {
		head = head->right;
	}
  	return head;
}

static void transplant(rbtree* t, node_t* u, node_t* v) {
	if (u->parent == t->nil) {
		t->root = v;
	} else if (u == u->parent->left) {
		u->parent->left = v;
	} else {
		u->parent->right = v;
	}
    v->parent = u->parent;
}


static void delete_fixup(rbtree* t, node_t* x) {
	while (x != t->root && x->color == RBTREE_BLACK) {
		if (x == x->parent->left) {
			node_t* w = x->parent->right;
			if (w->color == RBTREE_RED) {
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				left_rotate(t, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
				w->color = RBTREE_RED;
				x = x->parent;
			} else {
				if (w->right->color == RBTREE_BLACK) {
					w->left->color = RBTREE_BLACK;
					w->color = RBTREE_RED;
					right_rotate(t, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = RBTREE_BLACK;
				w->right->color = RBTREE_BLACK;
				left_rotate(t, x->parent);
				x = t->root;
			}
		} else {
			node_t* w = x->parent->left;
			if (w->color == RBTREE_RED) {
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				right_rotate(t, x->parent);
				w = x->parent->left;
			}
			if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
			} else {
				if (w->left->color == RBTREE_BLACK) {
					w->right->color = RBTREE_BLACK;
					w->color = RBTREE_RED;
					left_rotate(t, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = RBTREE_BLACK;
				w->left->color = RBTREE_BLACK;
				right_rotate(t, x->parent);
				x = t->root;
			}
		}
	}
	x->color = RBTREE_BLACK;
}

static node_t* tree_minimum(rbtree* t, node_t* subtree) {
	node_t* head = subtree;
	while (head->left != t->nil) {
		head = head->left;
	}
  	return head;
}

int rbtree_erase(rbtree *t, node_t *z) {
    if (!z) return -1;
	node_t *y = z;
    node_t *x;
    color_t y_original_color = y->color;

    if (z->left == t->nil) {
        x = z->right;
        transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        transplant(t, z, z->left);
    } else {
        // 오른쪽 서브트리에서 최소값 찾기
        y = tree_minimum(t, z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;  // y가 바로 z의 오른쪽 자식일 경우
        } else {
            transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;  // 항상 z 색깔 복사
    }

    if (y_original_color == RBTREE_BLACK) {
        delete_fixup(t, x);
    }

    free(z);
    return 1;
}

static void inorder(node_t* node, node_t* nil, key_t* arr, size_t n, size_t* index) {
	if (node == nil || *index >= n) {
		return;
	}

	inorder(node->left, nil, arr, n, index);
	if ((*index) < n) {
        arr[(*index)] = node->key;
        (*index)++;
    }
	inorder(node->right, nil, arr, n, index);
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
	if (t == NULL || t->root == t->nil) return 0;
	size_t index = 0;
	inorder(t->root, t->nil, arr, n, &index);
	return 1;
}