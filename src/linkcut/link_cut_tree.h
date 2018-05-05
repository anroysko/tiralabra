#ifndef __LINKCUTTREE_LINK_CUT_H_
#define __LINKCUTTREE_LINK_CUT_H_

#include "./../util/vector.h" // Vector

struct LinkCutNode {
	LinkCutNode* left; // Left child
	LinkCutNode* right; // Right child
	LinkCutNode* parent; // Parent
	int value; // Value of this node
	int min_value; // Minimum value in this node's subtree in its chain
	int index; // Index of this node in our link/cut tree

	LinkCutNode(int val, int ind);
	~LinkCutNode() = default;

	// inline so defined in header
	inline int getMinVal() {
		return min_value;
	}
	inline void update() {
		min_value = value;
		if (left != nullptr && left->getMinVal() < min_value) min_value = left->getMinVal();
		if (right != nullptr && right->getMinVal() < min_value) min_value = right->getMinVal();
	}
	inline void push() {
		// Do nothing
	}
	inline bool isRoot() {
		return (parent == nullptr || ((parent->left != this) && (parent->right != this)));
	}
};

class LinkCutTree {
	private:
		Vector<LinkCutNode*> nodes; // Vector containing all nodes in this tree
	public:
		LinkCutTree() = default;
		LinkCutTree(int size);
		~LinkCutTree();
		void insert(int val); // Insert node with value val
		void setVal(int i, int val); // Set value of node with index i to val
		// Set "child"'s parent to be "parent"
		// Warning: calling this function when "parent" is child's direct or indirect child causes undefined behavior
		void link(int child, int parent);
		void cut(int i); // Cuts edge to parent of node with index i
		int findRoot(int i); // Returns index of root of tree node i is in
		int pathMinVal(int i); // Returns minimum value on path from node i to its tree's root
		int pathMinInd(int i); // Returns node that has minimum value on path from node i to its tree's root, breaking ties by picking upmost one
};

#endif // __LINKCUTTREE_LINK_CUT_H_
