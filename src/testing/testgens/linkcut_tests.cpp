#include <vector> // std::vector
#include <string> // std::string
#include <random> // random()
#include <stdlib.h> // srand
#include <assert.h> // assert()
#include <fstream> // std::ofstream, std::ifstream
#include "./../../linkcut/link_cut_functions.h"
#include "./../../linkcut/link_cut_tree.h"
#include "./../test_utils.h"
#include "linkcut_tests.h" // Implement
#include <iostream> // TODO: remove

// Empty namespace to hide variable names.
namespace {
	// *************** Functions for generating tests ***************

	/// Generates a test of random link cut operations.
	/// After each operation that prints a value, print its answer.
	bool genRandomLinkCutOperations(int nodes, int queries, int maxval, bool deep, int seed, const std::string& path) {
		std::ofstream fout;
		fout.open(path);

		// Print node count and query count.
		fout << nodes << ' ' << queries << '\n';
		
		srand(seed);
		std::vector<int> parent (nodes);
		std::vector<int> value (nodes);
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
		// Print parents and values.
		for (int i = 0; i < nodes; ++i) fout << parent[i] << ' '; fout << '\n';
		for (int i = 0; i < nodes; ++i) fout << value[i] << ' '; fout << '\n';

		for (int j = 0; j < queries; ++j) {
			int t = rand() % 7;
			if (t == 0) {
				// insert
				int val = rand() % maxval;
				parent.push_back(-1);
				value.push_back(val);
				++nodes;
				fout << t << ' ' << val << '\n';
			} else if (t == 1) {
				// setVal
				int ind = rand() % nodes;
				int val = rand() % maxval;
				value[ind] = val;
				fout << t << ' ' << ind << ' ' << val << '\n';
			} else if (t == 2) {
				// link
				// Be careful not to create a loop
				int kid = rand() % nodes;
				int par = rand() % nodes;
				if ((par != kid) && (parent[kid] == -1)) {
					int tmp = par;
					while(parent[tmp] != -1) tmp = parent[tmp];
					if (tmp != kid) {
						parent[kid] = par;
						fout << t << ' ' << kid << ' ' << par << '\n';
						continue;
					}
				}
				// Choose some other operation!
				--j;
				continue;
			} else if (t == 3) {
				// cut
				int ind = rand() % nodes;
				parent[ind] = -1;
				fout << t << ' ' << ind << '\n';
			} else if (t == 4) {
				// findRoot
				int ind = rand() % nodes;
				int ans = ind;
				while(parent[ans] != -1) ans = parent[ans];
				fout << t << ' ' << ind << ' ' << ans << '\n';
			} else if (t == 5) {
				// pathMinVal
				int ind = rand() % nodes;
				int cur = ind;
				int ans = value[cur];
				while(parent[cur] != -1) {
					cur = parent[cur];
					ans = std::min(ans, value[cur]);
				}
				fout << t << ' ' << ind << ' ' << ans << '\n';
			} else if (t == 6) {
				// pathMinInd
				// Tiebreaker: Take highest up one
				int ind = rand() % nodes;
				int cur = ind;
				int ans = cur;
				while(parent[cur] != -1) {
					cur = parent[cur];
					if (value[cur] <= value[ans]) ans = cur;
				}
				fout << t << ' ' << ind << ' ' << ans << '\n';
			}
		}
		fout.close();
	}

	/// Interweaves the given two vectors.
	std::vector<int> interveawe(std::vector<int> a, std::vector<int> b) {
		std::vector<int> res (a.size() + b.size());
		for (int i = 0; i < a.size(); ++i) {
			res[2*i] = a[i];
			if (i < b.size()) res[2*i+1] = b[i];
		}
		return res;
	}
	/// Returns worst accessing order possible for the link/cut tree.
	std::vector<int> getBadOrder(int s, int e) {
		if (s == e) return {s};
		int mid = (s + e) / 2;
		return interveawe(getBadOrder(s, mid), getBadOrder(mid + 1, e));
	}

	/// Generate a test case where the preferred child changes log_2(total_nodes) times each operation.
	/// Total_nodes must be even
	void genWideLinkCutOperations(int total_nodes, int queries, const std::string& path) {
		assert(total_nodes % 2 == 0);

		std::ofstream fout;
		fout.open(path);
		// Print node count and operation count.
		fout << total_nodes << ' ' << queries << '\n';

		int nodes = total_nodes / 2;
		std::vector<int> order = getBadOrder(nodes, 2*nodes - 1);
		
		std::vector<int> parent (2*nodes);
		std::vector<int> value (2*nodes);
		for (int i = 0; i < 2*nodes; ++i) {
			parent[i] = ((i == 0 || i == 1) ? -1 : i / 2);
			value[i] = rand() % 100;
		}
		// Print parents and values
		for (int i = 0; i < total_nodes; ++i) fout << parent[i] << ' '; fout << '\n';
		for (int i = 0; i < total_nodes; ++i) fout << value[i] << ' '; fout << '\n';
		
		// Print operations
		for (int j = 0; j < queries;) {
			for (auto ind : order) {
				int cur = ind;
				int ans = cur;
				while(parent[cur] != -1) {
					cur = parent[cur];
					if (value[cur] <= value[ans]) ans = cur;
				}
				fout << 6 << ' ' << ind << ' ' << ans << '\n';
				
				++j;
				if (j == queries) break;
			}
		}
		fout.close();
	}

	// *************** Functions for testing ***************

	/// Tests the given file.
	/// Compares the correct answers to queries given in the input to the tree's answers.
	/// Additionally writes answers to all queries to the out-file.
	bool testFile(const std::string& file_in, const std::string& file_out) {
		// Read the input
		std::ifstream fin;
		std::ofstream fout;
		fin.open(file_in);
		if (!fin.is_open()) return false;
		fout.open(file_out);

		int nodes, queries; fin >> nodes >> queries;
		
		LinkCutTree tree (nodes);
		for (int i = 0; i < nodes; ++i) {
			int parent; fin >> parent;
			if (parent != -1) tree.link(i, parent);
		}
		for (int i = 0; i < nodes; ++i) {
			int value; fin >> value;
			tree.setVal(i, value);
		}

		for (int j = 0; j < queries; ++j) {
			int t; fin >> t;
			if (t == 0) {
				// insert
				int val; fin >> val;
				tree.insert(val);
			} else if (t == 1) {
				// setVal
				int ind; fin >> ind;
				int val; fin >> val;
				tree.setVal(ind, val);
			} else if (t == 2) {
				// link
				// Be careful not to create a loop
				int kid; fin >> kid;
				int par; fin >> par;
				tree.link(kid, par);
			} else if (t == 3) {
				// cut
				int ind; fin >> ind;
				tree.cut(ind);
			} else if (t == 4) {
				// findRoot
				int ind; fin >> ind;
				int ans; fin >> ans;
				int tree_ans = tree.findRoot(ind);
				fout << tree_ans << ' ';
				if (ans != tree_ans) {
					fout << '\n';
					fout.close();
					fin.close();
					return false;
				}
			} else if (t == 5) {
				// pathMinVal
				int ind; fin >> ind;
				int ans; fin >> ans;
				int tree_ans = tree.pathMinVal(ind);
				fout << tree_ans << ' ';
				if (ans != tree_ans) {
					fout << '\n';
					fout.close();
					fin.close();
					return false;
				}
			} else if (t == 6) {
				// pathMinInd
				int ind; fin >> ind;
				int ans; fin >> ans;
				int tree_ans = tree.pathMinInd(ind);
				fout << tree_ans << ' ';
				if (ans != tree_ans) {
					fout << '\n';
					fout.close();
					fin.close();	
					return false;
				}
			}
		}
		fout << '\n';
		fout.close();
		fin.close();
		return true;
	}

	// *************** Functions for generating tests ***************

	// in-file has name <test>.in
	// out-file has name <test>.out

	/// Count of test files of all sizes.
	const int TESTS_PER_TYPE = 20;
	const int TEST_TYPES = 3;
	/// Names of test files.
	std::vector<std::string> test_files;
} // Empty namespace

// Generate tests.
// If create_tests is true, the tests are also created.
// Otherwise, this just fills performance_test_names and test_test_names.
void genLinkCutTests(bool create_tests, const std::string& base_path) {
	// bool genRandomLinkCutOperations(int nodes, int queries, int maxval, bool deep, int seed, const std::string& path)
	// void genWideLinkCutOperations(int total_nodes, int queries, const std::string& path)
	if (! test_files.empty()) return;
	int maxval = 1e9;
	int test_seed = 0;
	// Random tests.
	for (int i = 1; i <= TESTS_PER_TYPE; ++i) {
		int nodes = 1<<(1 + i);
		int queries = 1<<(4 + i);
		std::string test_name = (std::string)"link_cut_random_" + std::to_string(i);
		test_files.push_back(test_name);
		if (create_tests) genRandomLinkCutOperations(nodes, queries, maxval, test_seed, false, base_path + test_name + ".in");
	}
	// Deep tests
	for (int i = 1; i <= TESTS_PER_TYPE; ++i) {
		int nodes = 1<<(1 + i);
		int queries = 1<<(4 + i);
		std::string test_name = (std::string)"link_cut_random_deep_" + std::to_string(i);
		test_files.push_back(test_name);
		if (create_tests) genRandomLinkCutOperations(nodes, queries, maxval, test_seed, true, base_path + test_name + ".in");
	}
	// Wide tests
	for (int i = 1; i <= TESTS_PER_TYPE; ++i) {
		int nodes = 1<<(1 + i);
		int queries = 1<<(4 + i);
		std::string test_name = (std::string)"link_cut_wide_" + std::to_string(i);
		test_files.push_back(test_name);
		if (create_tests) genWideLinkCutOperations(nodes, queries, base_path + test_name + ".in");
	}
}

// Get TestGroup containing tests.
TestGroup getLinkCutTests(bool perf_tests, const std::string& base_path) {
	// Make sure that we know the names of the tests.
	genLinkCutTests(false, base_path);
	std::vector<Test> tests;
	// How many tests we want to take per test group
	int high = (perf_tests ? TESTS_PER_TYPE : TESTS_PER_TYPE / 2);
	int low = (perf_tests ? TESTS_PER_TYPE / 2 : 0);
	for (int j = 0; j < TEST_TYPES; ++j) {
		int base = j * TESTS_PER_TYPE;
		for (int i = base + low; i < high + base; ++i) {
			// File for the test to take input from
			std::string in_file = base_path + test_files[i] + ".in";
			std::string out_file = base_path + test_files[i] + ".out";
			if (perf_tests) {
				tests.push_back(makeTest(testFile, in_file, out_file, out_file, true));
			} else {
				tests.push_back(makeTest(testFile, in_file, out_file, out_file, false));
			}
		}
	}
	if (perf_tests) {
		return makeTestGroup(tests, "link/cut performance tests", true);
	} else {
		return makeTestGroup(tests, "link/cut tests", true);
	}
}

