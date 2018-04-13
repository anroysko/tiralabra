#include <iostream>	// std::cout
#include "link_cut_functions.h"	// Link/Cut functions
#include "link_cut_tree.h"


LinkCutNode::LinkCutNode(int val, int ind) {
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	value = val;
	index = ind;
	min_value = val;
}

inline void LinkCutNode::update() {
	min_value = value;
	if (left != nullptr) min_value = std::min(min_value, left->min_value);
	if (right != nullptr) min_value = std::min(min_value, right->min_value);
}
inline bool LinkCutNode::isRoot() {
	return (parent == nullptr) || (parent->left != this && parent->right != this);
}

void LinkCutNode::print(LinkCutNode* node, int indent) {
	for (int i = 0; i < indent; ++i) {
		std::cout << "  ";
	}
	if (node == nullptr) {
		std::cout << "*\n";
	} else {
		std::cout << "val: " << node->value << ", ind: " << node->index << ",  min_val: " << node->min_value << " parent ind: ";
		if (node->parent == nullptr) {
			std::cout << "none\n";
		} else {
			std::cout << node->parent->index << '\n';
		}
		print(node->left, indent + 1);
		print(node->right, indent + 1);
	}
}

LinkCutTree::LinkCutTree(int n, std::vector<int> vals) {
	nodes.resize(n);
	for (int i = 0; i < n; ++i) {
		nodes[i] = new LinkCutNode(vals[i], i);
	}
}

LinkCutTree::~LinkCutTree() {
	for (int i = 0; i < nodes.size(); ++i) {
		delete nodes[i];
	}
	nodes.resize(0);
	nodes.shrink_to_fit();
}
void LinkCutTree::insert(int val) {
	nodes.push_back(new LinkCutNode(val, nodes.size()));
}

void LinkCutTree::setVal(int i, int val) {
	assert(0 <= i && i <= nodes.size());
	LinkCutNode* node = nodes[i];
	access(node);
	node->value = val;
	node->update();
}

void LinkCutTree::link(int child, int parent) {
	assert(0 <= child && child < nodes.size());
	assert(0 <= parent && parent < nodes.size());
	linkChild(nodes[child], nodes[parent]);
}

void LinkCutTree::cut(int i) {
	cutParent(nodes[i]);
}

int LinkCutTree::findRoot(int i) {
	assert(0 <= i && i < nodes.size());
	LinkCutNode* root = subtreeRoot(nodes[i]);
	return root->index;
}

int LinkCutTree::pathMin(int i) {
	assert(0 <= i && i < nodes.size());
	access(nodes[i]);
	return nodes[i]->min_value;
}

void LinkCutTree::print() {
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i]->isRoot()) {
			LinkCutNode::print(nodes[i]);
		}
	}
}




