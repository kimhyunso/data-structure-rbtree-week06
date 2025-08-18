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

static node_t* create_node(rbtree *t, key_t key) {
	node_t* newNode = calloc(1, sizeof(node_t));
	newNode->color = RBTREE_RED;
	newNode->key = key;
	newNode->left = t->nil;
	newNode->right = t->nil;
	newNode->parent = t->nil;

	return newNode;
}

static node_t* find(rbtree* t, key_t key) {
	node_t* head = t->root;
	node_t* tail = t->nil;

	while (head != t->nil) {
		tail = head;
		if (key < head->key) {
			head = head->left;
		} else {
			head = head->right;
		}
	}
	return tail;
}


static void left_rotate(rbtree* t, node_t* cur) {
	node_t* temp = cur->right;
	cur->right = temp->left;

	if (temp->left != t->nil) {
		temp->left->parent = cur;
	}

	temp->parent = cur->parent;
	if (cur->parent == t->nil) {
		t->root = temp;
	} else if (cur == cur->parent->left) {
		cur->parent->left = temp;
	} else {
		cur->parent->right = temp;
	}
	temp->left = cur;
	cur->parent = temp;
}

static void right_rotate(rbtree* t, node_t* cur) {
	node_t* temp = cur->left;
	cur->left = temp->right;

	if (temp->right != t->nil) {
		temp->right->parent = cur;
	}

	temp->parent = cur->parent;
	if (cur->parent == t->nil) {
		t->root = temp;
	} else if (cur == cur->parent->left) {
		cur->parent->left = temp;
	} else {
		cur->parent->right = temp;
	}
	temp->right = cur;
	cur->parent = temp;
}

node_t* rbtree_insert(rbtree *t, const key_t key) {
	node_t* newNode = create_node(t, key);
	node_t* findNode = find(t, key);
	newNode->parent = findNode->parent;
	
	if (t->root == t->nil) {
		t->root = newNode;
	} else if (findNode->key > key){
		findNode->left = newNode;
	} else {
		findNode->right = newNode;
	}

	while (newNode->parent->color == RBTREE_RED) { 
		if (newNode->parent == newNode->parent->parent->left) { // 넣고 싶은 node가 왼쪽에 존재하는가
			node_t* uncle = newNode->parent->parent->right;

			if (uncle->color == RBTREE_RED && newNode->parent->color == RBTREE_RED) { // 부모와 삼촌이 모두 레드인가
				newNode->parent->color = RBTREE_BLACK;
				uncle->color = RBTREE_BLACK;
				newNode->parent->parent = RBTREE_RED;
				newNode = newNode->parent->parent;
			} else { // 부모와 삼촌이 둘 중 하나가 레드가 아닌 경우
				if (newNode == newNode->parent->right) {
					newNode = newNode->parent;
					left_rotate(t, newNode);
				}
				newNode->parent->color = RBTREE_BLACK;
				newNode->parent->parent->color = RBTREE_RED;
				right_rotate(t, newNode->parent->parent);
			}
			
		} else { // 넣고 싶은 노드가 오른쪽에 존재하는가
			node_t* uncle = newNode->parent->parent->left;

			if (uncle->color == RBTREE_RED && newNode->parent->color == RBTREE_RED) { // 부모와 삼촌이 모두 레드인가
				newNode->parent->color = RBTREE_BLACK;
				uncle->color = RBTREE_BLACK;
				newNode->parent->parent = RBTREE_RED;
				newNode = newNode->parent->parent;
			} else { // 부모와 삼촌이 둘 중 하나가 레드가 아닌 경우
				if (newNode == newNode->parent->right) {
					newNode = newNode->parent;
					right_rotate(t, newNode);
				}
				newNode->parent->color = RBTREE_BLACK;
				newNode->parent->parent->color = RBTREE_RED;
				left_rotate(t, newNode->parent->parent);
			}
		}		
	}
	
	t->root->color = RBTREE_BLACK;
	return t->root;
}

node_t* rbtree_find(const rbtree* t , const key_t key) {
	node_t* head = t->root;

	while (head != t->nil) {
		if (head->key == key) {
			return head;
		}

		if (head->key < key) {
			head = head->right;
		} else {
			head = head->left;
		}
	}
	return NULL;
}

node_t* rbtree_min(const rbtree *t) {
	if (t->root == t->nil) {
        return t->nil;
    }

	node_t* head = t->root;
	while (head->left != t->nil) {
		head = head->left;
	}
  	return head;
}

node_t* rbtree_max(const rbtree *t) {
	if (t->root == t->nil) {
        return t->nil;
    }

	node_t* head = t->root;
	while (head->right != t->nil) {
		head = head->right;
	}
  	return head;
}

int rbtree_erase(rbtree *t, node_t *p) {
	// node_t* findNode = rbtree_find(t, p->key);
	// free(findNode);

  	return 0;
}

static void inorder(node_t* node, node_t* nil, key_t* arr, size_t n, size_t* index) {
	if (node == nil || *index >= n) {
		return;
	}

	inorder(node->left, nil, arr, n, index);
	if (*index < n) {
        arr[*index] = node->key;
        (*index)++;
    }
	inorder(node->right, nil, arr, n, index);
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
	if (t->root == t->nil) return 0;
	size_t index = 0;
	inorder(t->root, t->nil, arr, n, &index);
	return 1;
}