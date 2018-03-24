#include <assert.h>	// assert()
#include <utility>	// std::pair
#include "splay_tree.h"	// Implement functions for
#include "splay_node.h"	// splayNode and its functions

SplayTree::SplayTree() {
	size = 0;
	root = 0;
}

SplayTree::~SplayTree() {
	if (root != 0) delete root;
}

int SplayTree::getSize() {
	return size;
}

// Returns max element in splay tree
int SplayTree::findMax() {
	assert(size > 0);
	root = SplayNode::findMax(root);
	return root->key;
}

// Returns min element in splay tree
int SplayTree::findMin() {
	assert(size > 0);
	root = SplayNode::findMin(root);
	return root->key;
}

// Returns whether splay tree contains a node with the given value as key
bool SplayTree::find(int val) {
	if (size == 0) return false;
	root = SplayNode::find(root, val);
	return (root->key == val);
}

// Inserts node with the given value as key
// Tree cannot contain duplicates. Adding a duplicate does nothing
void SplayTree::insert(int val) {
	++size;
	if (root == 0) {
		root = new SplayNode(val);
	} else {
		// Split the tree and join the halves as the new node's children
		std::pair<SplayNode*, SplayNode*> halves = SplayNode::split(root, val);
		if (halves.first != 0 && halves.first->key == val) {
			// Element already exists
			root = SplayNode::join(halves.first, halves.second);
			return;
		}
		root = new SplayNode(val);
		root->left = halves.first;
		root->right = halves.second;
		if (root->left != 0) root->left->parent = root;
		if (root->right != 0) root->right->parent = root;
	}
}

bool SplayTree::erase(int val) {
	if (root == 0) return false;
	root = SplayNode::find(root, val); // splay element we want to erase to the root
	if (root->key == val) {
		SplayNode* a = root->left;
		SplayNode* b = root->right;

		// remove root and create a new tree from the two children
		--size;
		if (a != 0) a->parent = 0;
		if (b != 0) b->parent = 0;
		root->left = 0;
		root->right = 0;
		delete root; // free memory from old root

		root = SplayNode::join(a, b);
		return true;
	} else {
		// element doesn't exist
		return false;
	}
}

void SplayTree::print() {
	SplayNode::print(root);
}
