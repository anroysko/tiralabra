#ifndef __LINKCUTTREE_LINK_CUT_H_
#define __LINKCUTTREE_LINK_CUT_H_

#include <vector>

struct LinkCutNode {
	LinkCutNode* left;
	LinkCutNode* right;
	LinkCutNode* parent;
	int value;
	int min_value;
	int dec_value; // Value to decrement all nodes in this node's subtree by
	int index;

	LinkCutNode(int val, int ind);
	~LinkCutNode() = default;
	// inline so defined in header
	inline int getMinVal() {
		return min_value - dec_value;
	}
	// Assumes that this node has been pushed. 
	inline void update() {
		min_value = value;
		if (left != nullptr && left->getMinVal() < min_value) min_value = left->getMinVal();
		if (right != nullptr && right->getMinVal() < min_value) min_value = right->getMinVal();
	}
	inline void push() {
		value -= dec_value;
		min_value -= dec_value;
		if (left != nullptr) left->dec_value += dec_value;
		if (right != nullptr) right->dec_value += dec_value;
		dec_value = 0;
	}
	inline bool isRoot() {
		return (parent == nullptr || ((parent->left != this) && (parent->right != this)));
	}
	static void print(LinkCutNode* node, int indent = 0);
};

class LinkCutTree {
	private:
		std::vector<LinkCutNode*> nodes;
	public:
		LinkCutTree() = default;
		LinkCutTree(int n, std::vector<int> vals);
		~LinkCutTree();
		void insert(int val);
		void setVal(int i, int val);
		void link(int child, int parent);
		void cut(int i);
		int findRoot(int i);
		int pathMinVal(int i);
		int pathMinInd(int i);
		int pathChild(int i);
		void print();
};

#endif // __LINKCUTTREE_LINK_CUT_H_
