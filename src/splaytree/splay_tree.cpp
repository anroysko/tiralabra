#include <assert.h>	// assert()
#include <utility>	// std::pair
#include <iostream>	// TODO: remove
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

int SplayTree::findMax() {
	assert(size > 0);
	root = SplayNode::findMax(root);
	return root->key;
}

int SplayTree::findMin() {
	assert(size > 0);
	root = SplayNode::findMin(root);
	return root->key;
}

bool SplayTree::find(int val) {
	if (size == 0) return false;
	root = SplayNode::find(root, val);
	return (root->key == val);
}

void SplayTree::insert(int val) {
	++size;
	if (root == 0) {
		root = new SplayNode(val);
	} else {
		std::pair<SplayNode*, SplayNode*> halves = SplayNode::split(root, val);
		if (halves.first != 0 && halves.first->key == val) return; // element already exists
		root = new SplayNode(val);
		root->left = halves.first;
		root->right = halves.second;
		if (root->left != 0) root->left->parent = root;
		if (root->right != 0) root->right->parent = root;
	}
}

bool SplayTree::erase(int val) {
	if (root == 0) return false;
	root = SplayNode::find(root, val);
	if (root->key == val) {
		SplayNode* a = root->left;
		SplayNode* b = root->right;

		--size;
		if (a != 0) a->parent = 0;
		if (b != 0) b->parent = 0;
		root->left = 0;
		root->right = 0;
		delete root;

		root = SplayNode::join(a, b);
		return true;
	} else {
		return false;
	}
}

void SplayTree::print() {
	SplayNode::print(root);
	std::cout << '\n';
}
