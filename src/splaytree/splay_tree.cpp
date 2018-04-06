#include <assert.h>	// assert()
#include <utility>	// std::pair
#include "splay_functions.h"	// splay tree functions

#include "splay_tree.h"

// Splay node functions
SplayNode::SplayNode(int v) {
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	val = v;
	size = 1;
}

SplayNode::~SplayNode() {
	if (left != nullptr) delete left;
	if (right != nullptr) delete right;
}

inline void SplayNode::update() {
	size = 1;
	if (left != nullptr) size += left->size;
	if (right != nullptr) size += right->size;
}

// Returns x's index, e.g. how many smaller elements than x exist in the tree
int SplayNode::getIndex(SplayNode* x) {
        assert(x != nullptr);
        splay(x);
        if (x->left == nullptr) return 0;
        return x->left->size;
}

// Find and splay on k'th smallest node
SplayNode* SplayNode::findKth(SplayNode* root, int k) {
	assert(root != nullptr);
	assert(k < root->size);
	int left_size = 0;
	if (root->left != nullptr) left_size = root->left->size;
	if (k < left_size) {
		return findKth(root->left, k);
	} else if (k == left_size) {
		splay(root);
		return root;
	} else {
		return findKth(root->right, k - (left_size + 1));
	}
}

// :: for debugging ::
// Prints tree, with indentation marking depth
void SplayNode::print(SplayNode* x, int depth) {
	for (int i = 0; i < depth; ++i) std::cout << "  ";
	if (x == 0) {
		std::cout << "*\n";
	} else {
		std::cout << x->val << ' ';
		std::cout << '\n';
		print(x->left, depth + 1);
		print(x->right, depth + 1);
	}
}

// traverse tree, and every time we enter a node, add it to the list.
// Every node appears exactly 3 times.
// returns true if all parent pointers were correct, and false otherwise
bool SplayNode::getTraversal(SplayNode* x, std::vector<int>& vec) {
        if (x == 0) {
                return true;
        } else {
                bool res = true;
                if ((x->left) && (x->left->parent != x)) res = false;
                if ((x->right) && (x->right->parent != x)) res = false;

                vec.push_back(x->val);
                if (!getTraversal(x->left, vec)) res = false;
                vec.push_back(x->val);
                if (!getTraversal(x->right, vec)) res = false;
                vec.push_back(x->val);

                return res;
        }
}

// Splay tree functions
SplayTree::SplayTree() {
	root = nullptr;
}

SplayTree::~SplayTree() {
	if (root != nullptr) delete root;
}

int SplayTree::size() {
	if (root == nullptr) return 0;
	return root->size;
}

// Returns max element in splay tree
int SplayTree::getMax() {
	assert(root != nullptr);
	root = findLast(root);
	return root->val;
}

// Returns min element in splay tree
int SplayTree::getMin() {
	assert(root != nullptr);
	root = findFirst(root);
	return root->val;
}

int SplayTree::getKth(int k) {
	assert(root != nullptr);
	assert(size() > k);
	root = SplayNode::findKth(root, k);
	return root->val;
}

// Splays rightmost node with val <= value, or minimum val if no value is <= value
// Returns its value
int SplayTree::lowerBound(int value) {
	assert(root != nullptr);
	SplayNode* x = root;
	SplayNode* best = root;
	while(true) {
		if (x->val <= value) {
			if (x->right == nullptr) break;
			x = x->right;
		} else {
			// x->val > value
			if (x->left == nullptr) break;
			x = x->left;
		}
		if (x->val <= value) best = x;
	}
	splay(x); // To keep correct complexity
	splay(best); // To get it to the root
	root = best;
	return best->val;
}
// Splays leftmost node with val >= value, or maximum val if no value is >= value
// Returns its value
int SplayTree::upperBound(int value) {
	assert(root != nullptr);
	SplayNode* x = root;
	SplayNode* best = root;
	while(true) {
		if (x->val < value) {
			if (x->right == nullptr) break;
			x = x->right;
		} else {
			// x->val >= value
			if (x->left == nullptr) break;
			x = x->left;
		}
		if (x->val >= value) best = x;
	}
	splay(x); // To keep correct complexity
	splay(best); // To get it to the root
	root = best;
	return best->val;
}

// Returns whether splay tree contains a node with the given value as key
bool SplayTree::find(int value) {
	if (root == nullptr) return false;
	int ans = lowerBound(value);
	return (ans == value);
}

// How many vals in the tree are smaller than value
int SplayTree::getInd(int value) {
	if (root == nullptr) return 0;
	upperBound(value); // After this, root's left subtree contains all nodes with val < value
	if (root->left == nullptr) return 0;
	return root->left->size;
}

// Inserts node with the given value as key
void SplayTree::insert(int value) {
	if (root == nullptr) {
		root = new SplayNode(value);
	} else {
		// Split the tree and join the halves as the new node's children
		lowerBound(value); // Splays rightmost, val <= value to root
		std::pair<SplayNode*, SplayNode*> halves = split(root);
		root = new SplayNode(value);
		root->left = halves.first; // Contains <= values
		root->right = halves.second; // Contains > values
		if (root->left != 0) root->left->parent = root;
		if (root->right != 0) root->right->parent = root;
		root->update();
	}
}

bool SplayTree::erase(int value) {
	if (root == nullptr) return false;
	int mx = lowerBound(value); // splay element we want to erase to the root

	if (mx == value) {
		SplayNode* a = root->left;
		SplayNode* b = root->right;

		// remove root and create a new tree from the two children
		if (a != nullptr) a->parent = nullptr;
		if (b != nullptr) b->parent = nullptr;
		// Remove root's children so they won't get deleted with root
		root->left = nullptr;
		root->right = nullptr;
		delete root; // free memory from old root

		root = join(a, b);
		return true;
	} else {
		// element doesn't exist
		return false;
	}
}


void SplayTree::print() {
	SplayNode::print(root);
}
