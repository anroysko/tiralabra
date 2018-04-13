#ifndef __LINKCUTTREE_LINK_CUT_H_
#define __LINKCUTTREE_LINK_CUT_H_

#include <vector>

struct LinkCutNode {
	LinkCutNode* left;
	LinkCutNode* right;
	LinkCutNode* parent;
	int value;
	int min_value;
	int index;

	LinkCutNode(int val, int ind);
	~LinkCutNode() = default;
	inline void update();
	inline bool isRoot();
	static void print(LinkCutNode* node, int indent = 0);
};

class LinkCutTree {
	private:
		std::vector<LinkCutNode*> nodes;
	public:
		LinkCutTree() = default;
		LinkCutTree(int n, std::vector<int> vals);
		~LinkCutTree();
		void insert(int val);
		void setVal(int i, int val);
		void link(int child, int parent);
		void cut(int i);
		int findRoot(int i);
		int pathMin(int i);
		void print();
};

#endif // __LINKCUTTREE_LINK_CUT_H_
