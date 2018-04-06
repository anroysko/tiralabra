#include <iostream>
#include "splaytree/splay_tree.h"
using namespace std;

int main() {
	SplayTree tree;
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
