#ifndef __SPLAY_TREE_SPLAY_NODE_H_
#define __SPLAY_TREE_SPLAY_NODE_H_
#include <utility> // std::pair
#include <vector>  // std::vector

struct SplayNode {
	SplayNode* left;	// left child of node
	SplayNode* right;	// right child of node
	SplayNode* parent;	// parent of node
	int key;		// node's key, the value the binary tree is sorted on

	SplayNode(int v);
	~SplayNode();

	static bool isLeftChild(SplayNode* x);	// returns true if x is its parents left child
	static void zig(SplayNode* x);		// performs a "zig" on x (right rotation)
	static void zag(SplayNode* x);		// performs a "zag" on x (left rotation)
	static void rotateUp(SplayNode* x);	// performs a "zig" or a "zag" on x to move it up in the tree
	static void splayStep(SplayNode* x);	// Performs a splay step.
	static void splay(SplayNode* x);	// Performs splay steps until x is the root
	static SplayNode* find(SplayNode* root, int val);	// finds and splays node with given val
	static SplayNode* findMax(SplayNode* root);		// finds and splays node with max val
	static SplayNode* findMin(SplayNode* root);		// finds and splays node with min val
	static SplayNode* join(SplayNode* a_root, SplayNode* b_root);	// joins splay trees a and b
	static std::pair<SplayNode*, SplayNode*> split(SplayNode* root, int val);	// splits splay tree into two, with values <= val in the left one.
	static void print(SplayNode* x, int depth = 0);		// for debugging; prints the Splay tree
	static bool getTraversal(SplayNode* x, std::vector<int>& vec);	// Tree traversal for testing equality of trees for testing. Returns true if all parent pointers are correct.
};
#endif
