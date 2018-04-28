#include <iostream>
#include "splaytree/splay_tree.h"
#include "linkcut/link_cut_tree.h"
using namespace std;

void testSplayTree() {
	SplayTree tree;
	cout << "Test SplayTree\n";
	cout << "Apply operations (t \\not\\in [0,11] for help)\n";
	while(true) {
		int t;
		cin >> t;
		if (t == 0) {
			// int size()
			cout << tree.size() << '\n';
		} else if (t == 1) {
			// int getMax()
			cout << tree.getMax() << '\n';
		} else if (t == 2) {
			// int getMin()
			cout << tree.getMin() << '\n';
		} else if (t == 3) {
			// int getKth()
			int k;
			cin >> k;
			cout << tree.getKth(k) << '\n';
		} else if (t == 4) {
			// int lowerBound(int value)
			int value;
			cin >> value;
			cout << tree.lowerBound(value) << '\n';
		} else if (t == 5) {
			// int upperBound(int value)
			int value;
			cin >> value;
			cout << tree.upperBound(value) << '\n';
		} else if (t == 6) {
			// bool find(int value)
			int value;
			cin >> value;
			cout << tree.find(value) << '\n';
		} else if (t == 7) {
			// int getInd(int value)
			int value;
			cin >> value;
			cout << tree.getInd(value) << '\n';
		} else if (t == 8) {
			// void insert(int value);
			int value;
			cin >> value;
			tree.insert(value);
		} else if (t == 9) {
			// bool erase(int value);
			int value;
			cin >> value;
			cout << tree.erase(value) << '\n';
		} else if (t == 10) {
			tree.print();
		} else if (t == 11) {
			break;
		} else {
			cout << "Operations:\n"
				"	0:	int size();\n"
				"	1:	int getMax();\n"
				"	2:	int getMin();\n"
				"	3:	int getKth(int k);\n"
				"	4:	int lowerBound(int value);\n"
				"	5:	int upperBound(int value);\n"
				"	6:	bool find(int value);\n"
				"	7:	int getInd(int value);\n"
				"	8:	void insert(int value);\n"
				"	9:	bool erase(int value);\n"
				"	10:	void print();\n"
				"	11:	Quit\n"
				"	other:	help\n";
		}
	}
}
void testLinkCutTree() {
	LinkCutTree tree;
	cout << "Test LinkCutTree\n";
	cout << "Apply operations (t \\not\\in [0,8] for help)\n";
	while(true) {
		int t;
		cin >> t;
		if (t == 0) {
			// void insert(int val);
			int val;
			cin >> val;
			tree.insert(val);
		} else if (t == 1) {
			// void setVal(int i, int val);
			int i, val;
			cin >> i >> val;
			tree.setVal(i, val);
		} else if (t == 2) {
			// void link(int child, int parent);
			int child, parent;
			cin >> child >> parent;
			tree.link(child, parent);
		} else if (t == 3) {
			// void cut(int i);
			int i;
			cin >> i;
			tree.cut(i);
		} else if (t == 4) {
			// int findRoot(int i)
			int i;
			cin >> i;
			cout << tree.findRoot(i) << '\n';
		} else if (t == 5) {
			// int pathMinVal(int i)
			int i;
			cin >> i;
			cout << tree.pathMinVal(i) << '\n';
		} else if (t == 6) {
			// int pathMinInd(int i)
			int i;
			cin >> i;
			cout << tree.pathMinInd(i) << '\n';
		} else if (t == 7) {
			// void print()
			tree.print();
		} else if (t == 8) {
			break;
		} else {
			cout << "Operations:\n"
				"	0:	int insert(int val);\n"
				"	1:	void setVal(int i, int val);\n"
				"	2:	void link(int child, int parent);\n"
				"	3:	void cut(int i);\n"
				"	4:	int findRoot(int i);\n"
				"	5:	int pathMinVal(int value);\n"
				"	6:	int pathMinInd(int value);\n"
				"	7:	void print();\n"
				"	8:	quit\n"
				"	other:	help\n";
		}
	}
}

int main() {
	testLinkCutTree();
	testSplayTree();
}
