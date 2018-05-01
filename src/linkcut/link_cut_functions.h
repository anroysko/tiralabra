#ifndef __LINKCUT_LINK_CUT_FUNCTIONS_H_
#define __LINKCUT_LINK_CUT_FUNCTIONS_H_

#include "./../splaytree/splay_functions.h"

// These are template functions so they must be in the header
// Functions for link/cut nodes
// Link/Cut nodes must have
// 1. left child, right child, parent-pointers
// 2. A update() function, possibly empty
// 3. isRoot() function, returning if this node is the root of its chain
// 4. A push() function, possibly empty

// Accesses node. After accessing node i, it is the parent of the tree representing a chain from its represented tree's root to i.
// Therefore you can query for path-aggregate(like minimum value in path) by accessing the node's values
template<class T>
void access(T* node) {
	assert(node != nullptr);
	T* x = node;

	splay(x);
	if (x->right != nullptr) {
		x->push();
		x->right = nullptr; // If node has a right child, cut it off
		x->update();
	}
	
	while(true) {
		// Here node is its chain's root, so its parent is the chain's path-parent
		assert(x->isRoot());
		T* next = x->parent;

		if (next == nullptr) break;
		splay(next);

		// Replace next's preferred child with node
		next->push();
		if (next->right != nullptr) next->right = nullptr;
		next->right = x;
		next->update();

		x = next;
	}

	// Splay node back to the top
	splay(node);
}

// Returns node that is the root of the tree arg is in
template<class T>
T* subtreeRoot(T* node) {
	assert(node != nullptr);
	access(node);
	T* root = findFirst(node);
	splay(root);
	return root;
}

// Links child to node
// WARNING: you have to be careful that "node" is not "child"'s child. This causes undefined behaviour
template<class T>
void linkChild(T* child, T* node) {
	assert(child != nullptr);
	assert(node != nullptr);
	access(child);
	access(node);
	assert(child->parent == nullptr); // Child must not have a parent in the represented tree

	// Link the nodes
	node->push();
	node->right = child;
	child->parent = node;
	node->update();
}

// Cut off parent of "node"
template<class T>
void cutParent(T* node) {
	assert(node != nullptr);
	access(node);
	if (node->left == nullptr) {
		// Nothing to cut
	} else {
		node->push();
		node->left->parent = nullptr;
		node->left = nullptr;
		node->parent = nullptr;
		node->update();
	}
}

// Returns node with minimum value on path from x to root, breaking ties by picking the one closest to root.
// Requires additional properties
// 1. getMinVal() function, returning minimum value below a node
// 2. min_value, with its value after pushing being the minimum value below a node
template<class T>
T* pathMinNode(T* x) {
	assert(x != nullptr);
	access(x);
	while(true) {
		x->push();
		// Find leftmost minimum value, so prefer left-this-right
		if (x->left != nullptr && x->left->getMinVal() == x->min_value) {
			x = x->left;
		} else if (x->value == x->min_value) {
			break;
		} else {
			x = x->right;
		}
	}
	splay(x);
	return x;
}

#endif // __LINKCUT_LINK_CUT_FUNCTIONS_H_
