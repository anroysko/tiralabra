#include <assert.h>	// assert
#include "link_cut_functions.h"	// Link/Cut functions
#include "link_cut_tree.h"

LinkCutNode::LinkCutNode(int val, int ind) {
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	value = val;
	min_value = val;
	index = ind;
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

// Insert node with value val
void LinkCutTree::insert(int val) {
	nodes.push_back(new LinkCutNode(val, nodes.size()));
}

// Set value of node with index i to val
void LinkCutTree::setVal(int i, int val) {
	assert(0 <= i && i <= nodes.size());
	LinkCutNode* node = nodes[i];
	access(node);
	node->value = val;
	node->update();
}

// Set child's parent to be parent
// WARNING: causes undefined behavior if "parent" is "child"'s direct or indirect child
void LinkCutTree::link(int child, int parent) {
	assert(0 <= child && child < nodes.size());
	assert(0 <= parent && parent < nodes.size());
	linkChild(nodes[child], nodes[parent]);
}

// Cuts edge to parent of node with index i
void LinkCutTree::cut(int i) {
	cutParent(nodes[i]);
}

// Finds root of tree node with index i is in
int LinkCutTree::findRoot(int i) {
	assert(0 <= i && i < nodes.size());
	LinkCutNode* root = subtreeRoot(nodes[i]);
	return root->index;
}

// Returns minimum value on path from i to the root of its tree
int LinkCutTree::pathMinVal(int i) {
	assert(0 <= i && i < nodes.size());
	access(nodes[i]);
	return nodes[i]->getMinVal();
}

// Returns index of node that has minimum value amongst all nodes on path from i to its root.
// Breaks ties by returning one closest to the root
int LinkCutTree::pathMinInd(int i) {
	assert(0 <= i && i < nodes.size());
	LinkCutNode* x = nodes[i];
	access(x);
	x = pathMinNode(x);
	return x->index;
}
