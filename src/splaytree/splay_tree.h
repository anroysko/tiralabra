#ifndef __SPLAYTREE_SPLAY_TREE_H_
#define __SPLAYTREE_SPLAY_TREE_H_
#include "splay_node.h"

class SplayTree {
	public:
		// these two are public for testing, for now
		int size;
		SplayNode* root;

		SplayTree();
		~SplayTree();
		int getSize();
		int findMax();
		int findMin();
		bool find(int val);
		void insert(int val);
		bool erase(int val);
		void print();
};

#endif // __SPLAYTREE_SPLAY_TREE_H_
