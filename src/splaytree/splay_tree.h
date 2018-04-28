#ifndef __SPLAYTREE_SPLAY_TREE_H_
#define __SPLAYTREE_SPLAY_TREE_H_

#include "splay_functions.h"
#include <vector>

struct SplayNode {
	SplayNode* left;
	SplayNode* right;
	SplayNode* parent;
	int val;
	int size; // subtree size
	
	SplayNode(int v);
	~SplayNode();
	// Inline so defined in header
	inline void update() {
		size = 1 + (left == nullptr ? 0 : left->size) + (right == nullptr ? 0 : right->size);
	}
	inline bool isRoot() {
		return parent == nullptr;
	}
	inline void push() {
		// Do nothing
	}
	static int getIndex(SplayNode* x);
	static SplayNode* findKth(SplayNode* root, int k);
	static void print(SplayNode* x, int depth = 0);
	static void getTraversal(SplayNode* x, std::vector<int>& vec);
};

class SplayTree {
	private:
	public:
		SplayNode* root;
		SplayTree();
		~SplayTree();
		int size();
		int getMax();
		int getMin();
		int getKth(int k);
		int lowerBound(int value);
		int upperBound(int value);
		bool find(int value);
		int getInd(int value);
		void insert(int value);
		bool erase(int value);
		void print();
};

#endif // __SPLAYTREE_SPLAY_TREE_H_
