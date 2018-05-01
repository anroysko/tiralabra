#ifndef __SPLAYTREE_SPLAY_FUNCTIONS_H_
#define __SPLAYTREE_SPLAY_FUNCTIONS_H_
#include <assert.h>	// assert
#include <utility>	// std::pair
#include <iostream>	// std::cout

// Contains templates, so defined in header
// For all functions, T needs to fulfill the following:
// 1. Contains T* left, right, parent. They should be the pointers to related elements in the binary tree
// 2. Has function update. It can update information such as subtree size, sum or maximum.
// 3. Has function push. Push pushes down operations to be applied to the whole subtree. With this, you can for example have a value "dec_value", that tells you how much must be decremented from all values in a subtree. 
// 4. Has function isRoot. This can be used so that the actual root's parent is path-parent, and the value is conveniently updated

// returns true if x is its parents left child
template<class T>
inline bool isLeftChild(T* x) {
	assert(x != nullptr);
	assert(x->parent != nullptr);
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
template<class T>
void zig(T* x) {
	assert(x != nullptr);
	assert(x->left != nullptr);

	T* y = x->left;
	x->push();
	y->push();
	
	if (x->isRoot()) {
		// no need to set p->child
		y->parent = x->parent;		// set y->parent
	} else {
		if (isLeftChild(x)) {
			x->parent->left = y;	// set p->child
			y->parent = x->parent;	// set y->parent
		} else {
			x->parent->right = y;	// set p->child
			y->parent = x->parent;	// set y->parent
		}
	}
	x->parent = y;				// set x->parent

	x->left = y->right;			// set x->left
	if (x->left) x->left->parent = x;	// set b->parent
	y->right = x;				// set y->right

	// Update all affected nodes, up from leaves
	x->update();
	y->update();
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
template<class T>
void zag(T* x) {
	assert(x != nullptr);
	assert(x->right != nullptr);

	T* z = x->right;
	x->push();
	z->push();

	if (x->isRoot()) {
		// no need to set p->child
		z->parent = x->parent;		// set z->parent
	} else {
		if (isLeftChild(x)) {
			x->parent->left = z;	// set p->child
			z->parent = x->parent;	// set z->parent
		} else {
			x->parent->right = z;	// set p->child
			z->parent = x->parent;	// set z->parent
		}
	}
	x->parent = z;				// set x->parent

	x->right = z->left;			// set x->right
	if (x->right) x->right->parent = x;	// set c->parent
	z->left = x;				// set z->left

	// Update all affected nodes, up from leaves
	x->update();
	z->update();
}

// Performs a zig/zag to bring x up one step in the tree
template<class T>
void rotateUp(T* x) {
	assert(x != nullptr);
	assert(! x->isRoot());
	if (isLeftChild(x)) {
		zig(x->parent);
	} else {
		zag(x->parent);
	}
}

// Perform a splay tree splay step on x
template<class T>
void splayStep(T* x) {
	assert(x != nullptr);
	assert(! x->isRoot());

	T* y = x->parent;
	if (y->isRoot()) {
		// case 1: x's parent is the root
		rotateUp(x);
	} else {
		T* z = y->parent;
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
template<class T>
void splay(T* x) {
	assert(x != nullptr);
	while(! x->isRoot()) splayStep(x);
}

// Find first element (leftmost) in binary tree
template<class T>
T* findFirst(T* root) {
	assert(root != nullptr);
	T* x = root;
	while(x->left != nullptr) x = x->left;
	splay(x);
	return x;
}

// Find last element (rightmost) in binary tree
template<class T>
T* findLast(T* root) {
	assert(root != nullptr);
	T* x = root;
	while(x->right != nullptr) x = x->right;
	splay(x);
	return x;
}

// Find next element in binary tree (nullptr if there is none)
template<class T>
T* findNext(T* node) {
	assert(node != nullptr);
	splay(node);
	if (node->right == nullptr) return nullptr;
	T* next = findFirst(node->right);
	return next;
}
// Find prev element in binary tree (nullptr if there is none)
template<class T>
T* findPrev(T* node) {
	assert(node != nullptr);
	splay(node);
	if (node->left == nullptr) return nullptr;
	T* prev = findLast(node->left);
	return prev;
}

// Joins two splay trees A and B into a single splay tree.
// a and b must be the roots of their respective trees
template<class T>
T* joinTrees(T* a, T* b) {
	if (a == nullptr) return b;
	if (b == nullptr) return a;
	assert((a->isRoot()) && (a->parent == nullptr));
	assert((b->isRoot()) && (b->parent == nullptr));
	a = findLast(a);

	a->push();
	a->right = b; // Not overriding anything, since the rightmost node in A can't have a right child
	b->parent = a;
	a->update(); // Update a since it gained a child

	return a;
}

// Splits the tree into two subtrees A and B, with split_point being A's last (rightmost) node.
template<class T>
std::pair<T*, T*> splitTree(T* split_point) {
	assert(split_point != nullptr);
	splay(split_point);
	split_point->push();

	T* a = split_point;
	T* b = split_point->right;

	a->right = nullptr;
	if (b != nullptr) b->parent = nullptr;
	a->update(); // Update a since it lost its right child
	return {a, b};
}

#endif // __SPLAYTREE_SPLAY_FUNCTIONS_H_
