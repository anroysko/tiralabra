#include <random> // random()
#include <stdlib.h> // srand(int)
#include <vector> // Vector
#include <utility> // std::move
#include <string> // std::string
#include <assert.h> // assert
#include <fstream> // std::ofstream, std::ifstream

#include "./../test_utils.h" // Test, TestGroup, ...
#include "./../../dinic/dinic.h" // dinic functions
#include "./../../util/vector.h" // vector
#include "./../../util/array.h" // array
#include "dinic_tests.h" // Implement functions for

/// Empty namespace to hide functions and variables
namespace {
	// *************** Functions for generating tests ***************

	/// Edges must be at least 2 * nodes
	/// Creates test in the given folder
	void genRandomGraph(int nodes, int edges, int maxcap, int seed, const std::string& path) {
		assert(edges >= 2 * nodes);

		srand(seed);
		int n = nodes;
		int m = 0;
		std::vector<int> edge_target (edges);
		std::vector<int> edge_source (edges);
		std::vector<int> capacity (edges);
		std::vector<int> flow(edges, 0);
		std::vector<int> out_count(nodes, 0);
		std::vector<int> in_count(nodes, 0);
		int source = 0;
		int sink = nodes - 1;
		// Make sure each node has at least one 
		for (int i = 0; i < nodes; ++i) {
			if ((in_count[i] == 0) && (i > 0)) {
				int rand_prev = rand() % i;
				edge_target[m] = i;
				edge_source[m] = rand_prev;
				capacity[m] = 1 + (rand() % maxcap);
				++m;
				++out_count[rand_prev];
				++in_count[i];
			}
			if ((out_count[i] == 0) && (i < nodes-1)) {
				int rand_next = (i+1) + (rand() % (nodes-1-i));
				edge_target[m] = rand_next;
				edge_source[m] = i;
				capacity[m] = 1 + (rand() % maxcap);
				++m;
				++out_count[i];
				++in_count[rand_next];
			}
		}
		for (; m < edges; ++m) {
			int a = rand() % nodes;
			int b = rand() % (nodes-1);
			if (b >= a) ++b;
			edge_target[m] = a;
			edge_source[m] = b;
			capacity[m] = 1 + (rand() % maxcap);
		}
		
		// Print to file.
		std::ofstream fout;
		fout.open(path);
		fout << n << ' ' << m << ' ' << source << ' ' << sink << '\n';
		for (int i = 0; i < m; ++i) {
			fout << edge_source[i] << ' ' << edge_target[i] << ' ' << capacity[i] << '\n';
		}
		fout.close();
	}


	/// Construction to make dinic reach its max complexity:
	///         .. - so - ..
	/// bb                       bb
	/// |                        |
	/// ..                       ..
	/// |                        |
	/// b2        l1 - r1        b2
	/// |       /    x    \      |
	/// b1 - al - .. - .. - ar - b1
	///      |  \    x    / |
	///      ..   la - ra   ..
	/// 
	///      bb - .. - b2 - b1
	///                     |
	///                     si
	/// Different node groups:
	///     so = source
	///     si = sink
	///     a-nodes: Dinic has to take all a^2 paths trough these nodes each level
	///     b-nodes: These nodes let us set a length for a edge. node bi is i away from the node b1 is connected to.
	/// Edge weights:
	///     edges betwen l's and r's: 1
	///     edges from al and ar to l's and r's: a
	///     edges from so, al, ar: a^2
	///     all other edges: infinity
	void genHardGraph(int a, int b, const std::string& path) {
		// Generate graph to run on
		int nodes = 2 + 2*(a+1) + 3*b;
		int edges = (a*(a+2)) + (3*b + 2*((b+3)/4));
		const int inf = 1e9;

		Array<int> edge_target (edges);
		Array<int> edge_source (edges);
		Array<int> capacity (edges);
		Array<int> flow(edges, 0);
		Array<int> out_count(nodes, 0);
		Array<int> in_count(nodes, 0);

		// NODES
		int n = 0;
		// 2 special nodes
		int source = n; ++n;
		int sink = n; ++n;

		// 2*(a+1) type a nodes
		int aleft = n; ++n;
		int aright = n; ++n;
		Array<int> left_as(a);
		Array<int> right_as(a);
		for (int i = 0; i < a; ++i) {
			left_as[i] = n; ++n;
			right_as[i] = n; ++n;
		}
		
		// 3*b type b nodes
		Array<int> left_bs(b);
		Array<int> right_bs(b);
		Array<int> down_bs(b);
		for (int i = 0; i < b; ++i) {
			left_bs[i] = n; ++n;
			right_bs[i] = n; ++n;
			down_bs[i] = n; ++n;
		}

		// EDGES
		int m = 0;
		// a*(a+2) edges in group a
		for (int i = 0; i < a; ++i) {
			// from aright
			edge_source[m] = aright;
			edge_target[m] = right_as[i];
			capacity[m] = a;
			++m;
			// to aleft
			edge_source[m] = left_as[i];
			edge_target[m] = aleft;
			capacity[m] = a;
			++m;
			// Between a nodes
			for (int j = 0; j < a; ++j) {
				edge_source[m] = right_as[i];
				edge_target[m] = left_as[i];
				capacity[m] = 1;
				++m;
			}
		}
		
		// 3*b + 2*(b/4) (rounded up) edges in group b
		// 3*(b-1) edges inside group b
		for (int i = 0; i < b-1; ++i) {
			edge_target[m] = left_bs[i];
			edge_source[m] = left_bs[i+1];
			capacity[m] = inf;
			++m;
			edge_target[m] = right_bs[i];
			edge_source[m] = right_bs[i+1];
			capacity[m] = inf;
			++m;
			edge_target[m] = down_bs[i];
			edge_source[m] = down_bs[i+1];
			capacity[m] = inf;
			++m;
		}
		// 3 edges that lead out of b-groups
		edge_target[m] = aleft;
		edge_source[m] = left_bs[0];
		capacity[m] = inf;
		++m;
		edge_target[m] = aright;
		edge_source[m] = right_bs[0];
		capacity[m] = inf;
		++m;
		edge_target[m] = sink;
		edge_source[m] = down_bs[0];
		capacity[m] = inf;
		++m;
		// 2*(b/4) (rounded up) edges to groups b, with capacity a^2
		for (int i = 0; i * 4 < b; ++i) {
			// Edge from source
			if (i & 1) {
				edge_target[m] = left_bs[4*i];
			} else {
				edge_target[m] = right_bs[4*i];
			}
			edge_source[m] = source;
			capacity[m] = a*a;
			++m;
			// Edge to down-bs
			if (i & 1) {
				edge_source[m] = aright;
			} else {
				edge_source[m] = aleft;
			}
			edge_target[m] = down_bs[4*i];
			capacity[m] = a*a;
			++m;
		}

		/// Print to file.
		std::ofstream fout;
		fout.open(path);
		fout << n << ' ' << m << ' ' << source << ' ' << sink << '\n';
		for (int i = 0; i < m; ++i) {
			fout << edge_source[i] << ' ' << edge_target[i] << ' ' << capacity[i] << '\n';
		}
		fout.close();
	}

	// *************** Functions for testing ***************

	/// Test if flow is valid
	bool checkFlowGraph(const FlowGraph& graph, long long ans) {
		Array<long long> diff(graph.n, 0);
		for (int j = 0; j < graph.m; ++j) {
			int a = graph.edge_source[j];
			int b = graph.edge_target[j];
			diff[a] -= graph.flow[j];
			diff[b] += graph.flow[j];
		}
		for (int j = 0; j < graph.n; ++j) {
			if (j == graph.source) {
				if (diff[j] != -ans) return false;
			} else if (j == graph.sink) {
				if (diff[j] != ans) return false;
			} else {
				if (diff[j] != 0) return false;
			}
		}
		return true;
	}

	/// Test if more flow can be pushed
	bool tryDfsFlow(const FlowGraph& graph) {
		Array<int> visited (graph.n, false);

		Vector<int> que;
		que.reserve(graph.n);

		visited[graph.source] = true;
		que.push(graph.source);
		for (int j = 0; j < que.size(); ++j) {
			int i = que[j];
			if (i == graph.sink) return false;
			for (int s = 0; s < graph.edges.getArraySize(i); ++s) {
				int edge = graph.edges[i][s];
				if (graph.edge_source[edge] == i) {
					if (graph.capacity[edge] > graph.flow[edge]) {
						int t = graph.edge_target[edge];
						if (! visited[t]) {
							visited[t] = true;
							que.push(t);
						}
					}
				} else {
					if (graph.flow[edge] > 0) {
						int t = graph.edge_source[edge];
						if (! visited[t]) {
							visited[t] = true;
							que.push(t);
						}
					}
				}
			}
		}
		return true;
	}
	/// Tests the given file. Prints output to "out".
	/// cutoff: Cutoff in dinic. If -1, uses basic dinic.
	/// check: Should you check that the resulting flow graph is correct.
	bool testFile(const std::string& file_in, const std::string& file_out, int cutoff, bool check) {
		// 1. Read input
		std::ifstream fin;
		fin.open(file_in);
		if (!fin.is_open()) {
			std::cout << "Failed to open file " << file_in << "\n";
			return false;
		}
		
		int nodes;
		int edges;
		int source;
		int sink;
		fin >> nodes >> edges >> source >> sink;

		Array<int> edge_target (edges);
		Array<int> edge_source (edges);
		Array<int> capacity (edges);
		Array<int> flow(edges, 0);

		for (int i = 0; i < edges; ++i) {
			fin >> edge_source[i] >> edge_target[i] >> capacity[i];
		}

		fin.close();

		// 2. Calculate result
		FlowGraph graph (nodes, edges, source, sink, std::move(edge_source), std::move(edge_target), std::move(flow), std::move(capacity));

		long long res;
		if (cutoff == -1) {
			res = basicDinic(&graph);
		} else {
			res = dinic(&graph, cutoff);
		}
		/// 3. (maybe) Test that the result is valid
		if (check) {
			if (! checkFlowGraph(graph, res)) return false;
			if (! tryDfsFlow(graph)) return false;
		}
		/// 4. Output the result into a file, to make sure that it absolutely is calculated.
		std::ofstream fout;
		fout.open(file_out);
		fout << res << '\n';
		for (int i = 0; i < edges; ++i) {
			fout << graph.flow[i] << ' ';
		}
		fout << '\n';
		fout.close();
		return true;
	}

	// Wrapper functions
	/// DFS dinic with result checking on.
	bool dfsDinicTestFile(const std::string& file_in, const std::string& file_out) {
		return testFile(file_in, file_out, -1, true);
	}
	/// Link/Cut dinic with result checking on.
	bool linkCutDinicTestFile(const std::string& file_in, const std::string& file_out) {
		return testFile(file_in, file_out, 0, true);
	}
	/// Mixed dinic with result checking on.
	bool mixedDinicTestFile(const std::string& file_in, const std::string& file_out) {
		return testFile(file_in, file_out, 100, true);
	}
	/// DFS dinic without result checking.
	bool dfsDinicTimeFile(const std::string& file_in, const std::string& file_out) {
		return testFile(file_in, file_out, -1, false);
	}
	/// Link/Cut dinic without result checking.
	bool linkCutDinicTimeFile(const std::string& file_in, const std::string& file_out) {
		return testFile(file_in, file_out, 0, false);
	}
	/// Mixed dinic without result checking.
	bool mixedDinicTimeFile(const std::string& file_in, const std::string& file_out) {
		return testFile(file_in, file_out, 100, false);
	}

	// *************** Functions for generating tests ***************

	// in-file has name <test>.in
	// out-file has name <test>.out


	/// Count of test files of all sizes.
	/// TODO: increase
	const int TESTS_PER_TYPE = 20;
	const int TEST_TYPES = 2;
	/// Names of test files.
	std::vector<std::string> test_files;
} // Empty namespace

// Generate tests.
// If create_tests is true, the tests are also created.
// Otherwise, this just fills performance_test_names and test_test_names.
void genDinicTests(bool create_tests, const std::string& base_path) {
	if (! test_files.empty()) return;
	int weight_cap = 1e9;
	int test_seed = 0;
	// Random tests.
	for (int i = 1; i <= TESTS_PER_TYPE; ++i) {
		int nodes = 1<<(i);
		int edges = 1<<(4 + i);
		std::string test_name = (std::string)"dinic_random_" + std::to_string(i);
		test_files.push_back(test_name);
		if (create_tests) genRandomGraph(nodes, edges, weight_cap, test_seed, base_path + test_name + ".in");
	}
	// Hard tests
	// Large ones should be close to TLE. Time complexity on these tests is O(a^2 b^2) or O(a^2 b log_2(b))
	for (int i = 1; i <= TESTS_PER_TYPE; ++i) {
		int a = 1<<(1 + i / 2);
		int b = 1<<((i+1) / 2);
		std::string test_name = (std::string)"dinic_hard_" + std::to_string(i);
		test_files.push_back(test_name);
		if (create_tests) genHardGraph(a, b, base_path + test_name + ".in");
	}
}

// Get TestGroup containing tests.
TestGroup getDinicTests(bool perf_tests, const std::string& base_path) {
	const int max_is[6] = {TESTS_PER_TYPE, TESTS_PER_TYPE, TESTS_PER_TYPE, 17, TESTS_PER_TYPE, 0};
	// Make sure that we know the names of the tests.
	genDinicTests(false, base_path);
	std::vector<Test> tests;
	// How many tests we want to take per test group
	int low = (perf_tests ? TESTS_PER_TYPE / 2 : 0);
	int high = (perf_tests ? TESTS_PER_TYPE : TESTS_PER_TYPE / 2);
	for (int j = 0; j < TEST_TYPES; ++j) {
		int base = j * TESTS_PER_TYPE;
		for (int i = base + low; i < high + base; ++i) {
			// File for the test to take input from
			std::string in_file = base_path + test_files[i] + ".in";
			// File for the test to output to
			std::string out_dfs = base_path + test_files[i] + "_dfs.out";
			std::string out_link_cut = base_path + test_files[i] + "_link_cut.out";
			std::string out_mixed = base_path + test_files[i] + "_mixed.out";

			if (perf_tests) {
				if (i-base < max_is[j * 3 + 0]) tests.push_back(makeTest(dfsDinicTimeFile, in_file, out_dfs, out_dfs, true));
				if (i-base < max_is[j * 3 + 1]) tests.push_back(makeTest(linkCutDinicTimeFile, in_file, out_link_cut, out_link_cut, true));
				if (i-base < max_is[j * 3 + 2]) tests.push_back(makeTest(mixedDinicTimeFile, in_file, out_mixed, out_mixed, true));
			} else {
				if (i-base < max_is[j * 3 + 0]) tests.push_back(makeTest(dfsDinicTestFile, in_file, out_dfs, out_dfs, false));
				if (i-base < max_is[j * 3 + 1]) tests.push_back(makeTest(linkCutDinicTestFile, in_file, out_link_cut, out_link_cut, false));
				if (i-base < max_is[j * 3 + 2]) tests.push_back(makeTest(mixedDinicTestFile, in_file, out_mixed, out_mixed, false));
			}
		}
	}
	if (perf_tests) {
		return makeTestGroup(tests, "dinic performance tests", true);
	} else {
		return makeTestGroup(tests, "dinic tests", true);
	}
}
