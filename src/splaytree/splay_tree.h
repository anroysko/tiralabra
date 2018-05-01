#ifndef __SPLAYTREE_SPLAY_TREE_H_
#define __SPLAYTREE_SPLAY_TREE_H_

#include "splay_functions.h"
#include <vector>

struct SplayNode {
	SplayNode* left; // Left child
	SplayNode* right; // Right child
	SplayNode* parent; // Parent
	int val; // Value
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
	static int getIndex(SplayNode* x); // Get this node's index in its subtree
	static SplayNode* findKth(SplayNode* root, int k); // Find k'th element in this node's subtree
	static void getTraversal(SplayNode* x, std::vector<int>& vec); // Returns traversal (add to vec on each entry) of this subtree
};

class SplayTree {
	private:
	public:
		SplayNode* root; // Public so tests can access it
		SplayTree();
		~SplayTree();
		int size(); // Size of the splaytree
		int getMax(); // Returns maximum value in the tree
		int getMin(); // Returns minimum value in the tree
		int getKth(int k); // Returns k'th smallest value (0-indexed) in the tree
		int lowerBound(int value); // Returns largest value <= arg
		int upperBound(int value); // Returns smallest value >= arg
		bool find(int value); // Returns whether element with value = arg exists in tree
		int getInd(int value); // Returns how many elements are smaller than value
		void insert(int value); // Inserts node with value = arg to tree
		bool erase(int value); // Erases node with value = arg from the tree. Returns if it was found
};

#endif // __SPLAYTREE_SPLAY_TREE_H_
