#include <iostream>
#include "splaytree/splay_tree.h"
using namespace std;

int main() {
	splay_tree::SplayTree tree;
	
	cout << "Amount of operations: ";
	int m;	// amount of operations
	cin >> m;
	cout << "Apply operations (t \\not\\in [0,6] for help)\n";
	for (int j = 0; j < m; ++j) {
		int t;
		cin >> t;
		if (t == 0) {
			// Insert
			int x;
			cin >> x;
			tree.insert(x);
		} else if (t == 1) {
			int x;
			cin >> x;
			cout << tree.erase(x) << '\n';
		} else if (t == 2) {
			int x;
			cin >> x;
			cout << tree.find(x) << '\n';
		} else if (t == 3) {
			cout << tree.findMax() << '\n';
		} else if (t == 4) {
			cout << tree.findMin() << '\n';
		} else if (t == 5) {
			tree.print();
		} else if (t == 6) {
			cout << tree.getSize() << '\n';
		} else {
			cout << "Operations:\n";
			cout << "	0 x: insert node with key x\n";
			cout << "	1 x: delete node with key x\n";
			cout << "	2 x: find node with key x\n";
			cout << "	3: find maximum\n";
			cout << "	4: find minimum\n";
			cout << "	5: print tree\n";
		}
	}
}
