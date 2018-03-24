#ifndef __SPLAYTREE_SPLAY_TREE_H_
#define __SPLAYTREE_SPLAY_TREE_H_
#include "splay_node.h"

class SplayTree {
	private:
		int size;
		SplayNode* root;
	public:
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
