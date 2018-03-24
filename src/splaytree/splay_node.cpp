#include <assert.h>	// assert
#include <utility>	// std::pair
#include <iostream>	// std::cout
#include <vector>	// std::vector
#include "splay_node.h"

SplayNode::SplayNode(int v) {
	left = 0;
	right = 0;
	parent = 0;
	key = v;
}

// Deleting a node deletes its subtree
SplayNode::~SplayNode() {
	if (left) delete left;
	if (right) delete right;
}

inline bool SplayNode::isLeftChild(SplayNode* x) {
	assert(x != 0);
	assert(x->parent != 0);
	return (x->parent->left == x);
}

// Performs a zig on x
// Before zig:
//        p       
//        x       
//    y       z   
//  a   b   c   d 
// _ _ _ _ _ _ _ _
// After a zig on x:
//        p       
//        y       
//    a       x   
//  _   _   b   z 
// _ _ _ _ _ _ c d
// what changes?
// p->child
// y->parent
// x->parent
// x->left
// b->parent
// y->right
void SplayNode::zig(SplayNode* x) {
	assert(x != 0);
	assert(x->left != 0);

	SplayNode* y = x->left;
	
	if (x->parent != 0) {
		if (isLeftChild(x)) {
			x->parent->left = y;	// set p->child
			y->parent = x->parent;	// set y->parent
		} else {
			x->parent->right = y;	// set p->child
			y->parent = x->parent;	// set y->parent
		}
	} else {
		// no need to set p->child
		y->parent = 0;			// set y->parent
	}
	x->parent = y;				// set x->parent

	x->left = y->right;			// set x->left
	if (x->left) x->left->parent = x;	// set b->parent
	y->right = x;				// set y->right
}

// Performs a zag on x
// Before zag:
//        p       
//        x       
//    y       z   
//  a   b   c   d 
// _ _ _ _ _ _ _ _
// After a zag on x:
//        p       
//        z       
//    x       d   
//  y   c   _   _ 
// a b _ _ _ _ _ _
// what changes?
// p->child
// z->parent
// x->parent
// x->right
// c->parent
// z->left
void SplayNode::zag(SplayNode* x) {
	assert(x != 0);
	assert(x->right != 0);

	SplayNode* z = x->right;
	
	if (x->parent != 0) {
		if (isLeftChild(x)) {
			x->parent->left = z;	// set p->child
			z->parent = x->parent;	// set z->parent
		} else {
			x->parent->right = z;	// set p->child
			z->parent = x->parent;	// set z->parent
		}
	} else {
		// no need to set p->child
		z->parent = 0;			// set z->parent
	}
	x->parent = z;				// set x->parent

	x->right = z->left;			// set x->right
	if (x->right) x->right->parent = x;	// set c->parent
	z->left = x;				// set z->left
}

// Performs a zig/zag to bring x up one step in the tree
void SplayNode::rotateUp(SplayNode* x) {
	assert(x != 0);
	assert(x->parent != 0);
	if (isLeftChild(x)) {
		zig(x->parent);
	} else {
		zag(x->parent);
	}
}

// Perform a splay tree splay step on x
void SplayNode::splayStep(SplayNode* x) {
	assert(x != 0);

	SplayNode* y = x->parent;
	if (y == 0) {
		// case 0: x is the root
		return;
	}
	SplayNode* z = y->parent;

	if (z == 0) {
		// case 1: x's parent is the root
		rotateUp(x);
	} else {
		if (isLeftChild(x) == isLeftChild(y)) {
			// case 2: Both x and y are on the same side of their parents
			rotateUp(y);
			rotateUp(x);
		} else {
			// case 3: x and y are on different sides of their parents
			rotateUp(x);
			rotateUp(x);
		}
	}
}

// Splay a node to the top of the tree
void SplayNode::splay(SplayNode* x) {
	assert(x != 0);
	while(x->parent != 0) splayStep(x);
}

// Finds node with key equal to "val" and splays it up
// returns pointer to that node
// In case no such node exists, finds and splays either node with largest smaller key, or smallest larger key
SplayNode* SplayNode::find(SplayNode* root, int val) {
	assert(root != 0);

	SplayNode* x = root;
	while(true) {
		if (x->key == val) break;
		if (x->key < val) {
			if (x->right == 0) break;
			x = x->right;
		} else {
			if (x->left == 0) break;
			x = x->left;
		}
	}
	splay(x);
	return x;
}

// Finds node with maximum key and splays it up
// returns pointer to that node
SplayNode* SplayNode::findMax(SplayNode* root) {
	assert(root != 0);
	
	SplayNode* x = root;
	while(x->right != 0) x = x->right;
	splay(x);
	return x;
}

// Finds node with minimum key and splays it up
// returns pointer to that node
SplayNode* SplayNode::findMin(SplayNode* root) {
	assert(root != 0);
	
	SplayNode* x = root;
	while(x->left != 0) x = x->left;
	splay(x);
	return x;
}

// Joins splay trees a and b into a single splay tree.
// For all nodes in "a" and "b" must hold (a_node->key < b_node->key)
SplayNode* SplayNode::join(SplayNode* a_root, SplayNode* b_root) {
	if (a_root == 0) return b_root;
	if (b_root == 0) return a_root;
	a_root = findMax(a_root);
	assert(a_root->right == 0); // finding maximum and splaying on it means that it has no right child
	// link a_root and b_root
	a_root->right = b_root;
	b_root->parent = a_root;
	return a_root;
}

// Splits a splay tree into two splay trees "first" and "second"
// "first" contains all nodes with keys less than or equal to val
std::pair<SplayNode*, SplayNode*> SplayNode::split(SplayNode* root, int val) {
	if (root == 0) return {0,0};
	root = find(root, val);
	if (root->key <= val) {
		// root is max element in "first"
		SplayNode* second = root->right;
		root->right = 0;
		if (second) second->parent = 0;
		return {root, second};
	} else {
		// root is min element in "second"
		SplayNode* first = root->left;
		root->left = 0;
		if (first) first->parent = 0;
		return {first, root};
	}
}

// :: for testing ::
void SplayNode::print(SplayNode* x, int depth) {
	for (int i = 0; i < depth; ++i) std::cout << "  ";
	if (x == 0) {
		std::cout << "*\n";
	} else {
		std::cout << x->key << ' ';
		if (x->parent == 0) {
			std::cout << "N";
		} else {
			std::cout << x->parent->key;
		}
		std::cout << '\n';
		print(x->left, depth + 1);
		print(x->right, depth + 1);
	}
}

// traverse tree, and every time we enter a node, print it
// returns true if all parent pointers were correct, and false otherwise
bool SplayNode::getTraversal(SplayNode* x, std::vector<int>& vec) {
	if (x == 0) {
		return true;
	} else {
		bool res = true;
		if ((x->left) && (x->left->parent != x)) res = false;
		if ((x->right) && (x->right->parent != x)) res = false;

		vec.push_back(x->key);
		if (!getTraversal(x->left, vec)) res = false;
		vec.push_back(x->key);
		if (!getTraversal(x->right, vec)) res = false;
		vec.push_back(x->key);

		return res;
	}
}
