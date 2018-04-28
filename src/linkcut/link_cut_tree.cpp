#include <iostream>	// std::cout
#include <assert.h>	// assert
#include "link_cut_functions.h"	// Link/Cut functions
#include "link_cut_tree.h"

LinkCutNode::LinkCutNode(int val, int ind) {
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	value = val;
	min_value = val;
	dec_value = 0;
	index = ind;
}

void LinkCutNode::print(LinkCutNode* node, int indent) {
	for (int i = 0; i < indent; ++i) {
		std::cout << "  ";
	}
	if (node == nullptr) {
		std::cout << "*\n";
	} else {
		std::cout << "ind: " << node->index << ", value: " << node->value << ",  min_value: " << node->min_value << ", dec_value: " << node->dec_value << ", parent ind: ";
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

int LinkCutTree::pathMinVal(int i) {
	assert(0 <= i && i < nodes.size());
	access(nodes[i]);
	return nodes[i]->getMinVal();
}
int LinkCutTree::pathChild(int i) {
	assert(0 <= i && i < nodes.size());
	LinkCutNode* x = nodes[i];
	x = findNext(x);
	if (x == nullptr) return -1;
	return x->index;
}

int LinkCutTree::pathMinInd(int i) {
	assert(0 <= i && i < nodes.size());
	LinkCutNode* x = nodes[i];
	access(x);
	x = pathMinNode(x);
	return x->index;
}

void LinkCutTree::print() {
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i]->isRoot()) {
			LinkCutNode::print(nodes[i]);
		}
	}
}
