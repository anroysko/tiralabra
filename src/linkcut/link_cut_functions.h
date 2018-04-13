#ifndef __LINKCUT_LINK_CUT_FUNCTIONS_H_
#define __LINKCUT_LINK_CUT_FUNCTIONS_H_

#include "./../splaytree/splay_functions.h"

// These are template functions so they must be in the header
// Functions for link/cut nodes
// Link/Cut nodes must have
// 1. left child, right child, parent-pointers
// 2. isRoot() method of the form in the example
// In a Link/Cut tree the nodes are divided into chains of preferred childs
// Example struct:
//	struct ExampleNode {
//		ExampleNode* left;
//		ExampleNode* right;
//		ExampleNode* parent;
//		int min_val;
//		int this_val;
//		
//		void update() {
//			min_val = this_val;
//			if (left != nullptr) min_val = std::min(min_val, left->min_val);
//			if (right != nullptr) min_val = std::min(min_val, right->min_val);
//		}
//		// A chain's parent is the chain's path-parent
//		// How to separate this from a usual parent-link? chain's root is not a child for path-parent.
//		// isRoot() has to be this exact or equivalent function
//		bool isRoot() {
//			return (parent == nullptr) || ((parent->left != this) && (parent->right != this));
//		}
//	};


// Accesses node. After accessing node i, it is the parent of the tree representing a chain from its represented tree's root to i.
// Therefore you can query for path-aggregate(like minimum value in path) by accessing the node's values
template<class T>
void access(T* node) {
	assert(node != nullptr);
	T* x = node;

	splay(x);
	if (x->right != nullptr) {
		x->right = nullptr; // If node has a right child, cut it off
		node->update();
	}
	
	while(true) {
		// Here node is its chain's root, so its parent is the chain's path-parent
		assert(x->isRoot());
		T* next = x->parent;

		if (next == nullptr) break;
		splay(next);

		// Replace next's preferred child with node
		if (next->right != nullptr) next->right = nullptr;
		next->right = x;
		next->update();

		x = next;
	}

	// Splay node back to the top
	splay(node);
}

template<class T>
T* subtreeRoot(T* node) {
	assert(node != nullptr);
	access(node);
	T* root = findFirst(node);
	splay(root);
	return root;
}

// Links child to node
template<class T>
void linkChild(T* child, T* node) {
	assert(child != nullptr);
	assert(node != nullptr);
	access(child);
	access(node);
	assert(child->parent == nullptr);

	// Link the nodes
	node->right = child;
	child->parent = node;
	node->update();
}

template<class T>
void cutParent(T* node) {
	assert(node != nullptr);
	access(node);
	if (node->left == nullptr) {
		// Nothing to cut
	} else {
		node->left->parent = node->parent;
		node->left = nullptr;
		node->parent = nullptr;
		node->update();
	}
}

#endif // __LINKCUT_LINK_CUT_FUNCTIONS_H_
