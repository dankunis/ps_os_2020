#include "bstree.h"

#include <assert.h>

struct bstree {
	size_t size;
	struct treeNode* root;
};

typedef struct treeNode {
	int key;
	struct treeNode* left;
	struct treeNode* right;
	struct treeNode* parent;
} node;

bstree* bstree_create() {
	bstree* tree = malloc(sizeof(bstree));
	tree->root = NULL;
	tree->size = 0;
	return tree;
}

/*
 * Helping funciton for deletion
 */
static void postOrderFree(node* root) {
	if(root != NULL) {
		postOrderFree(root->left);
		postOrderFree(root->right);
		free(root);
	}
}

void bstree_destroy(bstree* t) {
    assert(t != NULL);
	postOrderFree(t->root);
	free(t);
}

void bstree_insert(bstree* t, int d) {
    assert(t != NULL);
	if(bstree_contains(t, d)) {
		return;
	}

	t->size += 1;

	if(t->size == 1) {
		node* root = malloc(sizeof(node));
		root->key = d;
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;

		t->root = root;
		return;
	}

	node* root2 = t->root;
	node* root3 = NULL;
	node* tmp = malloc(sizeof(node));
	tmp->key = d;

	while(root2 != NULL) {
		root3 = root2;
		if(d < root2->key) {
			root2 = root2->left;
		} else {
			root2 = root2->right;
		}
	}

	tmp->parent = root3;

	tmp->left = NULL;
	tmp->right = NULL;
	if(d < root3->key) {
		root3->left = tmp;
	} else {
		root3->right = tmp;
	}
}

static node* search(node* root, int key) {
	if((root == NULL) || (root->key == key)) return root;
	if(key < root->key) {
		return search(root->left, key);
	} else {
		return search(root->right, key);
	}
}

bool bstree_contains(const bstree* t, int d) {
    assert(t != NULL);
	return search(t->root, d) != NULL;
}

static node* min(node* root) {
    assert(root != NULL);
	node* l = root;
	while(l->left != NULL)
		l = l->left;
	return l;
}

static node* max(node* root) {
    assert(root != NULL);
	node* r = root;
	while(r->right != NULL)
		r = r->right;
	return r;
}

int bstree_minimum(const bstree* t) {
    assert(t != NULL);
	node* m = min(t->root);
	assert(m != NULL);
	return m->key;
}

int bstree_maximum(const bstree* t) {
    assert(t != NULL);
	node* m = max(t->root);
	assert(m != NULL);
	return m->key;
}

/*
 * Find the succeeding element
 */
node* succ(node* root) {
    assert(root != NULL);
	node *p = root, *l = NULL;
	if(p->right != NULL) return min(p->right);

	l = p->parent;
	while((l != NULL) && (p == l->right)) {
		p = l;
		l = l->parent;
	}
	return l;
}

void bstree_remove(bstree* t, int d) {
    assert(t != NULL);
	node* l = search(t->root, d);

	if(!bstree_contains(t, d)) {
		return;
	}
	t->size -= 1;

	node* m = NULL;

	// this node has no children
	if((l->left == NULL) && (l->right == NULL)) {
		m = l->parent;
		if(l == m->right) {
			m->right = NULL;
		} else {
			m->left = NULL;
		}
		free(l);
	}

	// if the node has only a *right* child
	if((l->left == NULL) && (l->right != NULL)) {
		m = l->parent;
		if(l == m->right)
			m->right = l->right;
		else
			m->left = l->right;
		free(l);
	}
	// if the node has only a *left* child
	if((l->left != NULL) && (l->right == NULL)) {
		m = l->parent;
		if(l == m->right)
			m->right = l->left;
		else
			m->left = l->left;
		free(l);
	}
	// if both children are present
	if((l->left != NULL) && (l->right != NULL)) {
		m = succ(l);
		l->key = m->key;
		if(m->parent->left == m) {
			m->parent->left = m->right;
		} else {
			m->parent->right = m->right;
		}
		if(m->right != NULL) {
			m->right->parent = m->parent;
		}
		free(m);
	}
}

static void inorderTraversal(node* root, FILE* out) {
    assert(root != NULL);
    assert(out != NULL);

	if(root == NULL) return;
	if(root->left != NULL) {
		fprintf(out, "(");
		inorderTraversal(root->left, out);
		fprintf(out, "), ");
	}

	if(root->key) fprintf(out, "%d", root->key);

	if(root->right != NULL) {
		fprintf(out, ", (");
		inorderTraversal(root->right, out);
		fprintf(out, ")");
	}
}

void bstree_print(const bstree* t, FILE* out) {
    assert(t != NULL);
    assert(out != NULL);
	fprintf(out, "(");

	if(t->size == 0) {
		fprintf(out, " NIL ");
	} else {
		inorderTraversal(t->root, out);
	}
	fprintf(out, ") : %li\n", t->size);
}

size_t bstree_size(const bstree* t) {
    assert(t != NULL);
	return t->size;
}
