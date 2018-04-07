#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include "./../splaytree/splay_functions.h"
#include "./../splaytree/splay_tree.h"
#include "./../linkcut/link_cut_functions.h"
#include "./../linkcut/link_cut_tree.h"

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

bool checkParentPointers(SplayNode* node) {
	if (node == nullptr) return true;
	if (node->left != nullptr && node->left->parent != node) return false;
	if (node->right != nullptr && node->right->parent != node) return false;
	if (! checkParentPointers(node->left)) return false;
	if (! checkParentPointers(node->right)) return false;
	return true;
}

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

bool compare(std::vector<int>& ans, SplayNode* root, std::string test_name) {
	std::vector<int> res;
	SplayNode::getTraversal(root, res);
	bool match = (ans == res);
	bool pps = checkParentPointers(root);
	bool sizes = (checkSizes(root) != -1);
	bool success = pps && sizes && match;
	if (! success) {
		std::cout << test_name << " failed: \n";
		if (pps == false) std::cout << "Parent pointers are wrong\n";
		if (sizes == false) std::cout << "Subtree sizes are wrong\n";
		if (match == false) {
			std::cout << "Traversal is wrong\n";
			for (auto it : ans) std::cout << it << ' '; std::cout << '\n';
			for (auto it : res) std::cout << it << ' '; std::cout << '\n';
		}
	}
	delete root;

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

	return compare(ans, root, "testZig");
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

	return compare(ans, root, "testZag");
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

	return compare(ans, root, "testSplay");
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

	return compare(ans, root, "testJoin");
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
	if (!compare(first_ans, halves.first, "testSplit_first")) success = false;
	if (!compare(second_ans, halves.second, "testSplit_second")) success = false;
	return success;
}

// Do unit tests for splay node
// returns: tests passed / total tests
std::pair<int, int> testSplayNode() {
	std::cout << "Testing Splay Node\n";
	int correct = 0;
	int total = 5;
	
	if (testZig()) ++correct;
	if (testZag()) ++correct;
	if (testSplay()) ++correct;
	if (testJoin()) ++correct;
	if (testSplit()) ++correct;

	std::cout << correct << "/" << total << " tests were successful\n";
	return {correct, total};
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
	if (!compare(ans, tree.root, "testInsert")) success = false;
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
	if (!compare(ans, tree.root, "testErase")) success = false;
	tree.root = 0; // so we won't delete the elements again
	return success;
}

std::pair<int, int> testSplayTree() {
	std::cout << "Testing Splay Tree\n";
	int correct = 0;
	int total = 2;
	
	if (testInsert()) ++correct;
	if (testErase()) ++correct;

	std::cout << correct << "/" << total << " tests were successful\n";
	return {correct, total};
}

bool testRandomLinkCutOperations(int ini_nodes, int queries, int maxval, int seed) {
	// Store the represented tree
	int nodes = ini_nodes;
	std::vector<int> parent (nodes);
	std::vector<int> value (nodes);
	srand(seed);
	for (int i = 0; i < nodes; ++i) {
		parent[i] = -1;
		value[i] = rand() % maxval;
	}
	
	LinkCutTree tree (nodes, value);

	for (int j = 0; j < queries; ++j) {
		int t = rand() % 6;
		if (t == 0) {
			// insert
			int val = rand() % maxval;
			parent.push_back(-1);
			value.push_back(val);
			tree.insert(val);
			++nodes;
		} else if (t == 1) {
			// setVal
			int ind = rand() % nodes;
			int val = rand() % maxval;
			tree.setVal(ind, val);
			value[ind] = val;
		} else if (t == 2) {
			// link
			// Be careful not to create a loop
			int kid = rand() % nodes;
			int par = rand() % nodes;
			if ((par != kid) && (parent[kid] == -1)) {
				int tmp = par;
				while(parent[tmp] != -1) tmp = parent[tmp];
				if (tmp != kid) {
					tree.link(kid, par);
					parent[kid] = par;
				}
			}
		} else if (t == 3) {
			// cut
			int ind = rand() % nodes;
			parent[ind] = -1;
			tree.cut(ind);
		} else if (t == 4) {
			// findRoot
			int ind = rand() % nodes;
			int ans = ind;
			while(parent[ans] != -1) ans = parent[ans];
			int tree_ans = tree.findRoot(ind);
			if (tree_ans != ans) {
				std::cout << "testRandomLinkCutOperations failed, with nodes=" << ini_nodes << ", queries=" << queries << ", maxval=" << maxval << ", seed=" << seed << "\n";
				std::cout << "findRoot(" << ind << "): " << ans << " vs " << tree_ans << '\n';
				if (nodes < 10) {
					std::cout << "Parents, values:\n";
					for (int i = 0; i < nodes; ++i) std::cout << parent[i] << ' '; std::cout << '\n';
					for (int i = 0; i < nodes; ++i) std::cout << value[i] << ' '; std::cout << '\n';
					std::cout << "Link/Cut tree:\n";
					tree.print();
				}
				return false;
			}
		} else if (t == 5) {
			// pathMin
			int ind = rand() % nodes;
			int cur = ind;
			int ans = value[cur];
			while(parent[cur] != -1) {
				cur = parent[cur];
				ans = std::min(ans, value[cur]);
			}
			int tree_ans = tree.pathMin(ind);
			if (tree_ans != ans) {
				std::cout << "testRandomLinkCutOperations failed, with nodes=" << ini_nodes << ", queries=" << queries << ", maxval=" << maxval << ", seed=" << seed << "\n";
				std::cout << "pathMin(" << ind << "): " << ans << " vs " << tree_ans << '\n';
				if (nodes < 10) {
					std::cout << "Parents, values:\n";
					for (int i = 0; i < nodes; ++i) std::cout << parent[i] << ' '; std::cout << '\n';
					for (int i = 0; i < nodes; ++i) std::cout << value[i] << ' '; std::cout << '\n';
					std::cout << "Link/Cut tree:\n";
					tree.print();
				}
				return false;
			}
		}
	}
	return true;
}

// Do "unit" tests for splay tree
// "Kind" of cheating since this isn't really a unit test
// It's really hard to write tests for these >8(
// returns: tests passed / total tests
std::pair<int, int> testLinkCutTree() {
	std::cout << "Testing Link/Cut tree\n";
	int correct = 0;
	int total = 7;
	// initial nodes, queries, maximum value, seed
	if ((correct == 0) && (testRandomLinkCutOperations(5, 10, 10, 0))) ++correct;
	if ((correct == 1) && (testRandomLinkCutOperations(10, 10, 10, 1))) ++correct;
	if ((correct == 2) && (testRandomLinkCutOperations(100, 100, 100, 2))) ++correct;
	if ((correct == 3) && (testRandomLinkCutOperations(1000, 1000, 1000, 3))) ++correct;
	if ((correct == 4) && (testRandomLinkCutOperations(10000, 10000, 10000, 4))) ++correct;
	if ((correct == 5) && (testRandomLinkCutOperations(100000, 100000, 100000, 5))) ++correct;
	if ((correct == 6) && (testRandomLinkCutOperations(1000000, 1000000, 1000000, 6))) ++correct;
	
	std::cout << correct << "/" << total << " tests were successful\n";
	return {correct, total};
}

int main() {
	testSplayNode();
	testSplayTree();
	testLinkCutTree();
}
