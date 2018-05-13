#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include "./../../splaytree/splay_functions.h"
#include "./../../splaytree/splay_tree.h"
#include "./../test_utils.h"
#include "splay_tests.h"

// Build SplayTree from its traversal
// Only works with unique values
SplayNode* buildTree(std::vector<int> traversal) {
	SplayNode* root = new SplayNode(traversal[0]);
	SplayNode* x = root;
	bool re_enter = false;
	for (int i = 1; i < traversal.size(); ++i) {
		if ((x->parent != 0) && ((x->parent->val) == traversal[i])) {
			x = x->parent;
			re_enter = true;
		} else if (traversal[i] == x->val) {
			// no left/right child
			re_enter = true;
		} else {
			if (re_enter) {
				// Make right child
				SplayNode* child = new SplayNode(traversal[i]);
				child->parent = x;
				x->right = child;
				x = child;
			} else {
				// Make left child
				SplayNode* child = new SplayNode(traversal[i]);
				child->parent = x;
				x->left = child;
				x = child;
			}
			re_enter = false;
		}
		x->update();
	}
	return root;
}

// Checks that parent pointers work correctly in tree rooted at "node"
// Returns true if they work
bool checkParentPointers(SplayNode* node) {
	if (node == nullptr) return true;
	if (node->left != nullptr && node->left->parent != node) return false;
	if (node->right != nullptr && node->right->parent != node) return false;
	if (! checkParentPointers(node->left)) return false;
	if (! checkParentPointers(node->right)) return false;
	return true;
}

// Checks that sizes work correctly in tree rooted at "node"
// Returns -1 if they are incorrect
int checkSizes(SplayNode* node) {
	if (node == nullptr) return 0;
	int sum = 1;
	if (node->left != nullptr) {
		int sub = checkSizes(node->left);
		if (sub == -1) return -1;
		sum += sub;
	}
	if (node->right != nullptr) {
		int sub = checkSizes(node->right);
		if (sub == -1) return -1;
		sum += sub;
	}
	if (node->size != sum) return -1;
	return sum;
}

bool compare(std::vector<int>& ans, SplayNode* root) {
	std::vector<int> res;
	SplayNode::getTraversal(root, res);
	bool match = (ans == res);
	bool pps = checkParentPointers(root);
	bool sizes = (checkSizes(root) != -1);
	bool success = pps && sizes && match;
	return success;
}

// SPLAY NODE TESTS
bool testZig() {
	// Before zig:
	//        8       
	//        4       
	//    2       6   
	//  1   3   5   7 
	// _ _ _ _ _ _ _ _
	std::vector<int> orig = {8,4,2,1,1,1,2,3,3,3,2,4,6,5,5,5,6,7,7,7,6,4,8,8};
	// (Hopefully) After a zig on x:
	//        8       
	//        2       
	//    1       4   
	//  _   _   3   6 
	// _ _ _ _ _ _ 5 7
	std::vector<int> ans  = {8,2,1,1,1,2,4,3,3,3,4,6,5,5,5,6,7,7,7,6,4,2,8,8};

	SplayNode* root = buildTree(orig);
	SplayNode* x = root->left;
	zig(x);

	return compare(ans, root);
}

bool testZag() {
	// Before zag:
	//        8       
	//        4       
	//    2       6   
	//  1   3   5   7 
	// _ _ _ _ _ _ _ _
	std::vector<int> orig = {8,4,2,1,1,1,2,3,3,3,2,4,6,5,5,5,6,7,7,7,6,4,8,8};
	// (Hopefully) After a zag on x:
	//        8       
	//        6       
	//    4       7   
	//  2   5   _   _ 
	// 1 3 _ _ _ _ _ _
	std::vector<int> ans  = {8,6,4,2,1,1,1,2,3,3,3,2,4,5,5,5,4,6,7,7,7,6,8,8};

	SplayNode* root = buildTree(orig);
	SplayNode* x = root->left;
	zag(x);

	return compare(ans, root);
}

bool testSplay() {
	// Before splaying
	//                                10
	//                7               
	//        2               9       
	//    1       5       8       _   
	//  _   _   3   6   _   _   _   _ 
	// _ _ _ _ _ 4 _ _ _ _ _ _ _ _ _ _
	std::vector<int> orig = {10,7,2,1,1,1,2,5,3,3,4,4,4,3,5,6,6,6,5,2,7,9,8,8,8,9,9,7,10,10};
	// Splaying on 3:
	// Case 3: x and y are not the same type of children
	//                                10
	//                7               
	//        2               9       
	//    1       3       8       _   
	//  _   _   _   5   _   _   _   _ 
	// _ _ _ _ _ _ 4 6 _ _ _ _ _ _ _ _
	//                                10
	//                7               
	//        3               9       
	//    2       5       8       _   
	//  1   _   4   6   _   _   _   _ 
	// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
	// Case 2: x and y are the same type of children             
	//                7               
	//        3               10       
	//    2       5       9       _   
	//  1   _   4   6   8   _   _   _ 
	// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
	//                3               
	//        2               7       
	//    1       _       5       10   
	//  _   _   _   _   4   6   9   _ 
	// _ _ _ _ _ _ _ _ _ _ _ _ 8 _ _ _
	std::vector<int> ans  = {3,2,1,1,1,2,2,3,7,5,4,4,4,5,6,6,6,5,7,10,9,8,8,8,9,9,10,10,7,3};
	
	SplayNode* root = buildTree(orig);
	root = SplayNode::findKth(root, 2); // Node 3 is index 2 in tree

	return compare(ans, root);
}

bool testJoin() {
	// A before join   
	//    2
	//  1   4
	// _ _ 3 5
	std::vector<int> a_orig = {2,1,1,1,2,4,3,3,3,4,5,5,5,4,2};
	// A after splaying on max (5)
	//        5
	//    4       _
	//  2   _   _   _
	// 1 3 _ _ _ _ _ _
	// B before join
	//    6
	//  7   _
	// _ _ _ _
	std::vector<int> b_orig = {6,7,7,7,6,6};
	// A and B joined
	//        5
	//    4       6
	//  2   _   7   _
	// 1 3 _ _ _ _ _ _
	std::vector<int> ans = {5,4,2,1,1,1,2,3,3,3,2,4,4,5,6,7,7,7,6,6,5};

	SplayNode* a_root = buildTree(a_orig);
	SplayNode* b_root = buildTree(b_orig);
	SplayNode* root = joinTrees(a_root, b_root);

	return compare(ans, root);
}

bool testSplit() {
	// Before splaying on 4
	//        5
	//    4       6
	//  2   _   7   _
	// 1 3 _ _ _ _ _ _
	std::vector<int> orig = {5,4,2,1,1,1,2,3,3,3,2,4,4,5,6,7,7,7,6,6,5};
	// Before splitting
	//        4
	//    2       5
	//  1   3   _   6
	// _ _ _ _ _ _ 7 _
	// After splitting
	// first:
	//        4
	//    2       _
	//  1   3   _   _
	// _ _ _ _ _ _ _ _
	// second:
	//    5
	//  _   6
	// _ _ 7 _
	std::vector<int> first_ans = {4,2,1,1,1,2,3,3,3,2,4,4};
	std::vector<int> second_ans = {5,5,6,7,7,7,6,6,5};
	
	SplayNode* root = buildTree(orig);
	std::pair<SplayNode*, SplayNode*> halves = splitTree(SplayNode::findKth(root, 3));
	bool success = true;
	if (!compare(first_ans, halves.first)) success = false;
	if (!compare(second_ans, halves.second)) success = false;
	return success;
}

// SPLAY TREE TESTS
bool testInsert() {
	// Splay Tree before inserting
	// size == 6
	//        3
	//    2       7
	//  1   _   5   _
	// _ _ _ _ _ 6 _ _
	std::vector<int> orig = {3,2,1,1,1,2,2,3,7,5,5,6,6,6,5,7,7,3};
	// Splay tree after splitting on 4:
	// first:
	//    3 
	//  2   _
	// 1 _ _ _
	// second:
	//    5
	//  _   7
	// _ _ 6 _
	// After joining:
	// size == 7
	//        4
	//    3       5
	//  2   _   _   7
	// 1 _ _ _ _ _ 6 _
	std::vector<int> ans = {4,3,2,1,1,1,2,2,3,3,4,5,5,7,6,6,6,7,7,5,4};

	SplayTree tree;
	tree.root = buildTree(orig);

	tree.insert(4);
	bool success = true;
	if (!compare(ans, tree.root)) success = false;
	tree.root = 0; // so we won't delete the elements again
	return success;
}

bool testErase() {
	// Splay Tree before erasing
	// size == 7
	//        4
	//    3       5
	//  2   _   _   7
	// 1 _ _ _ _ _ 6 _
	std::vector<int> orig = {4,3,2,1,1,1,2,2,3,3,4,5,5,7,6,6,6,7,7,5,4};
	// Splay Tree after erasing
	//        3
	//    2       5
	//  1   _   _   7
	// _ _ _ _ _ _ 6 _
	std::vector<int> ans = {3,2,1,1,1,2,2,3,5,5,7,6,6,6,7,7,5,3};

	SplayTree tree;
	tree.root = buildTree(orig);
	
	tree.erase(4);
	bool success = true;
	if (!compare(ans, tree.root)) success = false;
	tree.root = 0; // so we won't delete the elements again
	return success;
}

bool testUnitTests(const std::string& file_in, const std::string& file_out) {
	if (!testZig()) return false;
	if (!testZag()) return false;
	if (!testSplay()) return false;
	if (!testJoin()) return false;
	if (!testSplit()) return false;
	if (!testInsert()) return false;
	if (!testErase()) return false;
	return true;
}

bool testRandomSplayTreeOperations(int ini_nodes, int queries, int maxval, int seed) {
	std::vector<int> comp (ini_nodes);
	SplayTree tree;
	srand(seed);

	for (int i = 0; i < ini_nodes; ++i) {
		int val = rand() % maxval;
		tree.insert(val);
		comp[i] = val;
	}
	std::sort(comp.begin(), comp.end());

	for (int j = 0; j < queries; ++j) {
		int op = rand() % 10;
		if (op == 0) {
			int tree_ans = tree.size();
			int ans = comp.size();
			if (tree_ans != ans) return false;
		} else if (op == 1) {
			if (comp.empty()) continue;
			int tree_ans = tree.getMax();
			int ans = comp.back();
			if (tree_ans != ans) return false;
		} else if (op == 2) {
			if (comp.empty()) continue;
			int tree_ans = tree.getMin();
			int ans = comp[0];
			if (tree_ans != ans) return false;
		} else if (op == 3) {
			if (comp.empty()) continue;
			int k = rand() % comp.size();
			int tree_ans = tree.getKth(k);
			int ans = comp[k];
			if (tree_ans != ans) return false;
		} else if (op == 4) {
			if (comp.empty()) continue;
			int value = rand() % maxval;
			int tree_ans = tree.lowerBound(value);
			int ans = comp[0];
			for (int i = 0; i < comp.size(); ++i) {
				if (comp[i] <= value) ans = comp[i];
			}
			if (tree_ans != ans) return false;
		} else if (op == 5) {
			if (comp.empty()) continue;
			int value = rand() % maxval;
			int tree_ans = tree.upperBound(value);
			int ans = comp.back();
			for (int i = (int)comp.size() - 1; i >= 0; --i) {
				if (comp[i] >= value) ans = comp[i];
			}
			if (tree_ans != ans) return false;
		} else if (op == 6) {
			int value = rand() % maxval;
			bool tree_ans = tree.find(value);
			bool ans = 0;
			for (auto it : comp) ans |= (it == value);
			
			if (tree_ans != ans) return false;
		} else if (op == 7) {
			int value = rand() % maxval;
			int tree_ans = tree.getInd(value);
			int ans = 0;
			for (int i = 0; i < comp.size(); ++i) {
				if (comp[i] < value) ++ans;
			}
			if (tree_ans != ans) return false;
		} else if (op == 8) {
			int value = rand() % maxval;
			comp.push_back(value);
			sort(comp.begin(), comp.end());
			tree.insert(value);
		} else if (op == 9) {
			int value = rand() % maxval;
			bool tree_ans = tree.erase(value);
			bool ans = 0;
			for (int i = 0; i < comp.size(); ++i) {
				if (comp[i] == value) {
					ans = true;
					comp[i] = comp.back();
					comp.pop_back();
					break;
				}
			}
			std::sort(comp.begin(), comp.end());
			if (tree_ans != ans) return false;
		}
	}
	return true;
}
bool testRandomSplaySmall(const std::string& file_in, const std::string& file_out) {
	return testRandomSplayTreeOperations(10, 50, 10, 0);
}
bool testRandomSplayMedium(const std::string& file_in, const std::string& file_out) {
	return testRandomSplayTreeOperations(1000, 5000, 1000000000, 1);
}

bool timeRandomSplayTreeOperations(int ini_nodes, int queries, int maxval, int seed, int recal = 1e9) {
	SplayTree tree;
	srand(seed);

	for (int i = 0; i < ini_nodes; ++i) {
		int val = rand() % maxval;
		tree.insert(val);
	}

	int total = 0;
	for (int j = 0; j < queries; ++j) {
		if ((recal != 1e9) && (j % recal == 0)) {
			// This makes the tree a chain
			for (int i = 0; i < tree.size(); ++i) {
				int tree_ans = tree.getKth(i);
				total += tree_ans;
			}
		}
		int op = rand() % 9;
		if (op == 0) {
			if (tree.size() == 0) continue;
			int tree_ans = tree.getMax();
			total += tree_ans;
		} else if (op == 1) {
			if (tree.size() == 0) continue;
			int tree_ans = tree.getMin();
			total += tree_ans;
		} else if (op == 2) {
			if (tree.size() == 0) continue;
			int k = rand() % tree.size();
			int tree_ans = tree.getKth(k);
			total += tree_ans;
		} else if (op == 3) {
			if (tree.size() == 0) continue;
			int value = rand() % maxval;
			int tree_ans = tree.lowerBound(value);
			total += tree_ans;
		} else if (op == 4) {
			if (tree.size() == 0) continue;
			int value = rand() % maxval;
			int tree_ans = tree.upperBound(value);
			total += tree_ans;
		} else if (op == 5) {
			int value = rand() % maxval;
			bool tree_ans = tree.find(value);
			total += tree_ans;
		} else if (op == 6) {
			int value = rand() % maxval;
			int tree_ans = tree.getInd(value);
			total += tree_ans;
		} else if (op == 7) {
			int value = rand() % maxval;
			tree.insert(value);
		} else if (op == 8) {
			int value = rand() % maxval;
			bool tree_ans = tree.erase(value);
			total += value;
		}
	}
	volatile int tmp = total; // This is to make sure that the compiler doesn't optimize out any operations
	return true;
}
bool timeRandomSplayMedium(const std::string& file_in, const std::string& file_out) {
	return timeRandomSplayTreeOperations(10000, 10000, 100, 4);
}
bool timeRandomSplayLarge(const std::string& file_in, const std::string& file_out) {
	return timeRandomSplayTreeOperations(1000000, 1000000, 1000, 5);
}
bool timeHardRandomSplayMedium(const std::string& file_in, const std::string& file_out) {
	return timeRandomSplayTreeOperations(10000, 10000, 100, 6, 1000);
}
bool timeHardRandomSplayLarge(const std::string& file_in, const std::string& file_out) {
	return timeRandomSplayTreeOperations(500000, 1000000, 1000, 7, 100000);
}

void genSplayTests(bool create_tests, const std::string& base_path) {
	// Do nothing
}

TestGroup getSplayTests(bool perf_tests, const std::string& base_path) {
	if (perf_tests) {
		std::vector<Test> tests (4);
		tests[0] = makeTest(timeRandomSplayMedium, "","","splay_random_medium", true);
		tests[1] = makeTest(timeRandomSplayLarge, "","","splay_random_large", true);
		tests[2] = makeTest(timeHardRandomSplayMedium, "","","splay_hard_random_medium", true);
		tests[3] = makeTest(timeHardRandomSplayLarge, "","","splay_hard_random_large", true);
		return makeTestGroup(tests,"splay tree performance tests", true);
	} else {
		std::vector<Test> tests(3);
		tests[0] = makeTest(testUnitTests, "","","Splay unit tests", false);
		tests[1] = makeTest(testRandomSplaySmall, "","","splay_random_small", false);
		tests[2] = makeTest(testRandomSplayMedium, "","","splay_random_medium", false);
		return makeTestGroup(tests,"splay tree tests", true);
	}
}
