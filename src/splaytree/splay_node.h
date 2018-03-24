#ifndef __SPLAY_TREE_SPLAY_NODE_H_
#define __SPLAY_TREE_SPLAY_NODE_H_
#include <utility> // std::pair
#include <vector>  // std::vector

struct SplayNode {
	SplayNode* left;
	SplayNode* right;
	SplayNode* parent;
	int key;

	SplayNode(int v);
	~SplayNode();

	static bool isLeftChild(SplayNode* x);
	static void zig(SplayNode* x);
	static void zag(SplayNode* x);
	static void rotateUp(SplayNode* x);
	static void splayStep(SplayNode* x);
	static void splay(SplayNode* x);
	static SplayNode* find(SplayNode* root, int val);
	static SplayNode* findMax(SplayNode* root);
	static SplayNode* findMin(SplayNode* root);
	static SplayNode* join(SplayNode* a_root, SplayNode* b_root);
	static std::pair<SplayNode*, SplayNode*> split(SplayNode* root, int val);
	static void print(SplayNode* x, int depth = 0);
	static bool getTraversal(SplayNode* x, std::vector<int>& vec);
};
#endif
