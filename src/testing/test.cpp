#include <iostream>
#include <vector>
#include "./../splaytree/splay_node.h"
#include "./../splaytree/splay_tree.h"

// Build SplayTree from its traversal
SplayNode* buildTree(std::vector<int> traversal) {
	SplayNode* root = new SplayNode(traversal[0]);
	SplayNode* x = root;
	bool re_enter = false;
	for (int i = 1; i < traversal.size(); ++i) {
		if ((x->parent != 0) && ((x->parent->key) == traversal[i])) {
			x = x->parent;
			re_enter = true;
		} else if (traversal[i] == x->key) {
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
	}
	return root;
}

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
	SplayNode::zig(x);

	std::vector<int> res;
	bool pps = SplayNode::getTraversal(root, res);
	bool match = (ans == res);
	bool success = pps && match;

	if (! success) {
		std::cout << "testZig failed: \n";
		if (pps == false) std::cout << "Parent pointers are wrong\n";
		if (match == false) {
			std::cout << "Traversal is wrong\n";
			for (auto it : ans) std::cout << it << ' '; std::cout << '\n';
			for (auto it : res) std::cout << it << ' '; std::cout << '\n';
		}
		SplayNode::print(root);
	}
	delete root;

	return success;
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
	SplayNode::zag(x);

	std::vector<int> res;
	bool pps = SplayNode::getTraversal(root, res);
	bool match = (ans == res);
	bool success = pps && match;

	if (! success) {
		std::cout << "testZag failed: \n";
		if (pps == false) std::cout << "Parent pointers are wrong\n";
		if (match == false) {
			std::cout << "Traversal is wrong\n";
			for (auto it : ans) std::cout << it << ' '; std::cout << '\n';
			for (auto it : res) std::cout << it << ' '; std::cout << '\n';
		}
		SplayNode::print(root);
	}
	delete root;

	return success;	
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
	root = SplayNode::find(root, 3);

	std::vector<int> res;
	bool pps = SplayNode::getTraversal(root, res);
	bool match = (ans == res);
	bool success = pps && match;

	if (! success) {
		std::cout << "testSplay failed: \n";
		if (pps == false) std::cout << "Parent pointers are wrong\n";
		if (match == false) {
			std::cout << "Traversal is wrong\n";
			for (auto it : ans) std::cout << it << ' '; std::cout << '\n';
			for (auto it : res) std::cout << it << ' '; std::cout << '\n';
		}
		SplayNode::print(root);
	}
	delete root;

	return success;
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
	SplayNode* root = SplayNode::join(a_root, b_root);

	std::vector<int> res;
	bool pps = SplayNode::getTraversal(root, res);
	bool match = (ans == res);
	bool success = pps && match;

	if (! success) {
		std::cout << "testSplay failed: \n";
		if (pps == false) std::cout << "Parent pointers are wrong\n";
		if (match == false) {
			std::cout << "Traversal is wrong\n";
			for (auto it : ans) std::cout << it << ' '; std::cout << '\n';
			for (auto it : res) std::cout << it << ' '; std::cout << '\n';
		}
		SplayNode::print(root);
	}
	delete root;

	return success;
}

// Do unit tests for splay tree / splay node
// returns: tests passed / total tests
std::pair<int, int> testSplayTree() {
	std::cout << "Testing Splay Tree\n";
	int correct = 0;
	int total = 4;
	
	if (testZig()) ++correct;
	if (testZag()) ++correct;
	if (testSplay()) ++correct;
	if (testJoin()) ++correct;

	std::cout << correct << "/" << total << " tests were successful\n";
	return {correct, total};
}

int main() {
	testSplayTree();
}
