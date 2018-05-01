#include <vector>	// std::vector
#include <string>	// std::string
#include <random>	// random()
#include <algorithm>	// ???
#include <stdlib.h>	// random.h
#include <utility>	// ???
#include "./../../linkcut/link_cut_functions.h"
#include "./../../linkcut/link_cut_tree.h"
#include "./../test_util.h"
#include "linkcut_tests.h"

bool testRandomLinkCutOperations(int ini_nodes, int queries, int maxval, int seed) {
	// Store the represented tree
	int nodes = ini_nodes;
	std::vector<int> parent (nodes);
	std::vector<int> value (nodes);
	srand(seed);
	for (int i = 0; i < nodes; ++i) {
		// Initialize parent pointers to be a binary tree rooted at 0
		parent[i] = (i == 0 ? -1 : rand() % i);
		value[i] = rand() % maxval;
	}
	LinkCutTree tree (nodes, value);
	for (int i = 0; i < nodes; ++i) {
		if (parent[i] != -1) tree.link(i, parent[i]);
	}

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
			if (tree_ans != ans) return false;
		} else if (t == 5) {
			// pathMinVal
			int ind = rand() % nodes;
			int cur = ind;
			int ans = value[cur];
			while(parent[cur] != -1) {
				cur = parent[cur];
				ans = std::min(ans, value[cur]);
			}
			int tree_ans = tree.pathMinVal(ind);
			if (tree_ans != ans) return false;
		} else if (t == 6) {
			// pathMinVal
			int ind = rand() % nodes;
			int cur = ind;
			int ans = cur;
			while(parent[cur] != -1) {
				cur = parent[cur];
				if (value[cur] < value[ans]) ans = cur;
			}
			int tree_ans = tree.pathMinInd(ind);
			if (tree_ans != ans) return false;
		}
	}
	return true;
}

bool timeRandomLinkCutOperations(int ini_nodes, int queries, int maxval, int seed, bool deep) {
	// Store the represented tree
	int nodes = ini_nodes;
	std::vector<int> parent (nodes);
	std::vector<int> value (nodes);
	srand(seed);
	for (int i = 0; i < nodes; ++i) {
		// Initialize parent pointers to be a random binary tree rooted at 0
		// If deep, we always pick from five previous ones
		// This of course results in a deeper tree, as at least every five nodes the depth increases by one
		if (deep) {
			parent[i] = (i == 0 ? -1 : (i-1) - (rand() % std::min(5, i)));
		} else {
			parent[i] = (i == 0 ? -1 : rand() % i);
		}
		value[i] = rand() % maxval;
	}
	LinkCutTree tree (nodes, value);
	for (int i = 0; i < nodes; ++i) {
		if (parent[i] != -1) tree.link(i, parent[i]);
	}

	int answer = 0;

	for (int j = 0; j < queries; ++j) {
		int t = rand() % 6;
		if (t == 0) {
			// insert
			int val = rand() % maxval;
			tree.insert(val);
			++nodes;
		} else if (t == 1) {
			// setVal
			int ind = rand() % nodes;
			int val = rand() % maxval;
			tree.setVal(ind, val);
		} else if (t == 2) {
			// link
			// Be careful not to create a loop
			int kid = rand() % nodes;
			int par = rand() % nodes;
			int tmp1 = tree.findRoot(kid);
			int tmp2 = tree.findRoot(par);
			if (tmp1 != kid || tmp2 == kid) continue;
			tree.link(kid, par);
		} else if (t == 3) {
			// cut
			int ind = rand() % nodes;
			tree.cut(ind);
		} else if (t == 4) {
			// findRoot
			int ind = rand() % nodes;
			int tree_ans = tree.findRoot(ind);
			answer += tree_ans;
		} else if (t == 5) {
			// pathMinVal
			int ind = rand() % nodes;
			int tree_ans = tree.pathMinVal(ind);
			answer += tree_ans;
		} else if (t == 6) {
			// pathMinInd
			int ind = rand() % nodes;
			int tree_ans = tree.pathMinInd(ind);
			answer += tree_ans;
		}
	}
	// To force the compiler to actually calculate answer
	volatile int pls = answer;
	return true;
}

std::vector<int> interveawe(std::vector<int> a, std::vector<int> b) {
	std::vector<int> res (a.size() + b.size());
	for (int i = 0; i < a.size(); ++i) {
		res[2*i] = a[i];
		if (i < b.size()) res[2*i+1] = b[i];
	}
	return res;
}
std::vector<int> getBadOrder(int s, int e) {
	if (s == e) return {s};
	int mid = (s + e) / 2;
	return interveawe(getBadOrder(s, mid), getBadOrder(mid + 1, e));
}

bool timeLinkCutOperationsWide(int total_nodes, int queries) {
	int nodes = total_nodes / 2;
	std::vector<int> order = getBadOrder(nodes, 2*nodes - 1);
	
	std::vector<int> parent (2*nodes);
	std::vector<int> value (2*nodes);
	for (int i = 0; i < 2*nodes; ++i) {
		parent[i] = ((i == 0 || i == 1) ? -1 : i / 2);
		value[i] = rand() % 100;
	}
	LinkCutTree tree (2*nodes, value);
	for (int i = 0; i < 2*nodes; ++i) {
		if (parent[i] != -1) tree.link(i, parent[i]);
	}

	int answer = 0;
	for (int j = 0; j < queries; j += nodes) {
		for (auto ind : order) {
			int tree_ans = tree.pathMinInd(ind);
			answer += tree_ans;
		}
	}
	volatile int pls = answer;
	return true;
}

bool testRandomLinkCutOperationsMedium() {
	return testRandomLinkCutOperations(10000, 10000, 100, 0);
}
bool testRandomLinkCutOperationsLarge() {
	return testRandomLinkCutOperations(1000000, 1000000, 1000, 1);
}
bool timeRandomLinkCutOperationsMedium() {
	return timeRandomLinkCutOperations(10000, 10000, 100, 2, false);
}
bool timeRandomLinkCutOperationsLarge() {
	return timeRandomLinkCutOperations(1000000, 1000000, 1000, 3, false);
}
bool timeRandomLinkCutOperationsDeepMedium() {
	return timeRandomLinkCutOperations(10000, 10000, 1000, 4, true);
}
bool timeRandomLinkCutOperationsDeepLarge() {
	return timeRandomLinkCutOperations(1000000, 1000000, 1000, 5, true);
}
bool timeLinkCutOperationsWideMedium() {
	return timeLinkCutOperationsWide(1<<14, 1<<16);
}
bool timeLinkCutOperationsWideLarge() {
	return timeLinkCutOperationsWide(1<<18, 1<<20);
}
TestGroup getLinkCutTests() {
	std::vector<Test> res;
	res.push_back(makeTest(testRandomLinkCutOperationsMedium, "testRandomLinkCutOperationsMedium"));
	res.push_back(makeTest(testRandomLinkCutOperationsLarge, "testRandomLinkCutOperationsLarge"));
	return makeTestGroup(res, "link/cut tree tests", true);
}
TestGroup getLinkCutTimeTests() {
	std::vector<Test> res;
	res.push_back(makeTest(timeRandomLinkCutOperationsMedium, "timeRandomLinkCutOperationsMedium", true));
	res.push_back(makeTest(timeRandomLinkCutOperationsLarge, "timeRandomLinkCutOperationsLarge", true));
	res.push_back(makeTest(timeRandomLinkCutOperationsDeepMedium, "timeRandomLinkCutOperationsDeepMedium", true));
	res.push_back(makeTest(timeRandomLinkCutOperationsDeepLarge, "timeRandomLinkCutOperationsDeepLarge", true));
	res.push_back(makeTest(timeLinkCutOperationsWideMedium, "timeLinkCutOperationsWideMedium", true));
	res.push_back(makeTest(timeLinkCutOperationsWideLarge, "timeLinkCutOperationsWideLarge", true));
	return makeTestGroup(res, "link/cut tree performance tests", true);
}
