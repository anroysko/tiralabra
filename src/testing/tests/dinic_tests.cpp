#include <random> // random()
#include <stdlib.h> // srand(int)
#include <vector> // Vector
#include <utility> // std::move
#include <iostream> // TODO: remove

#include "./../../dinic/dinic.h"
#include "dinic_tests.h"

// Test if flow is valid
bool checkFlowGraph(const FlowGraph& graph, long long ans) {
	Vector<long long> diff(graph.n, 0);
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
// Test if more flow can be pushed
bool tryDfsFlow(const FlowGraph& graph) {
	Vector<int> visited (graph.n, false);

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

// Edges must be at least 2 * nodes
bool testRandomDinicGraph(int nodes, int edges, int maxcap, int seed, int cutoff) {
	if (edges < 2 * nodes) return false;
	srand(seed);
	int m = 0;
	Vector<int> edge_target (edges);
	Vector<int> edge_source (edges);
	Vector<int> capacity (edges);
	Vector<int> flow(edges, 0);
	Vector<int> out_count(nodes);
	Vector<int> in_count(nodes);
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

	FlowGraph graph (nodes, edges, source, sink, std::move(edge_source), std::move(edge_target), std::move(flow), std::move(capacity));
	long long res;
	if (cutoff == -1) {
		res = basicDinic(&graph);
	} else {
		res = dinic(&graph, cutoff);
	}
	if (!checkFlowGraph(graph, res)) return false;
	if (!tryDfsFlow(graph)) return false;
	return true;
}

// Edges must be at least 2 * nodes
bool timeRandomDinicGraph(int nodes, int edges, int maxcap, int seed, int cutoff) {
	if (edges < 2 * nodes) return false;
	srand(seed);
	int m = 0;
	Vector<int> edge_target (edges);
	Vector<int> edge_source (edges);
	Vector<int> capacity (edges);
	Vector<int> flow(edges, 0);
	Vector<int> out_count(nodes);
	Vector<int> in_count(nodes);
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
			++out_count[i];
			++in_count[rand_prev];
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

	FlowGraph graph (nodes, edges, source, sink, std::move(edge_source), std::move(edge_target), std::move(flow), std::move(capacity));
	int res;
	if (cutoff == -1) {
		res = basicDinic(&graph);
	} else {
		res = dinic(&graph, cutoff);
	}
	volatile int pls = res;
	return true;
}

// Construction to make dinic reach its max complexity:
//         .. - so - ..
// bb                       bb
// |                        |
// ..                       ..
// |                        |
// b2        l1 - r1        b2
// |       /    x    \      |
// b1 - al - .. - .. - ar - b1
//      |  \    x    / |
//      ..   la - ra   ..
// 
//      bb - .. - b2 - b1
//                     |
//      si - cc - .. - c1

// Different node groups:
// so = source
// si = sink
// a-nodes: Dinic has to take all a^2 paths trough these nodes each level
// b-nodes: These nodes let us set a length for a edge. node bi is i away from the node b1 is connected to.
// c-nodes: Just a long path that Dinic has to traverse for every path in part a

// Edge weights:
// edges betwen l's and r's: 1
// edges from al and ar to l's and r's: a
// edges from so, al, ar: a^2
// all other edges: infinity

bool timeHardDinicGraph(int a, int b, int c, int cutoff) {
	// Generate graph to run on
	int nodes = 2 + 2*(a+1) + 3*b + c;
	int edges = (a*(a+2)) + (3*b + 2*((b+3)/4)) + c;
	const int inf = 1e9;

	Vector<int> edge_target (edges);
	Vector<int> edge_source (edges);
	Vector<int> capacity (edges);
	Vector<int> flow(edges, 0);
	Vector<int> out_count(nodes);
	Vector<int> in_count(nodes);

	// NODES
	int n = 0;
	// 2 special nodes
	int source = n; ++n;
	int sink = n; ++n;

	// 2*(a+1) type a nodes
	int aleft = n; ++n;
	int aright = n; ++n;
	Vector<int> left_as(a);
	Vector<int> right_as(a);
	for (int i = 0; i < a; ++i) {
		left_as[i] = n; ++n;
		right_as[i] = n; ++n;
	}
	
	// 3*b type b nodes
	Vector<int> left_bs(b);
	Vector<int> right_bs(b);
	Vector<int> down_bs(b);
	for (int i = 0; i < b; ++i) {
		left_bs[i] = n; ++n;
		right_bs[i] = n; ++n;
		down_bs[i] = n; ++n;
	}
	
	// c type c nodes
	Vector<int> cs(c);
	for (int i = 0; i < c; ++i) {
		cs[i] = n; ++n;
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
	edge_target[m] = cs[c-1];
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

	// c edges in group c
	for (int i = 0; i < c-1; ++i) {
		edge_target[m] = cs[i];
		edge_source[m] = cs[i+1];
		capacity[m] = inf;
		++m;
	}
	edge_target[m] = sink;
	edge_source[m] = cs[0];
	capacity[m] = inf;
	++m;

	// Calculate flow
	FlowGraph graph (nodes, edges, source, sink, std::move(edge_source), std::move(edge_target), std::move(flow), std::move(capacity));
	int res;
	if (cutoff == -1) {
		res = basicDinic(&graph);
	} else {
		res = dinic(&graph, cutoff);
	}

	/*
	if (nodes < 100) {
		std::cout << "Graph:\n";
		std::cout << n << ' ' << m << '\n';
		for (int i = 0; i < m; ++i) {
			std::cout << graph.edge_source[i] << ' ' << graph.edge_target[i]  << ' ' << graph.capacity[i] << ' ' << graph.flow[i] << '\n';
		}
	}
	*/

	volatile int pls = res;
	if (!checkFlowGraph(graph, res)) return false;
	if (!tryDfsFlow(graph)) return false;
	return true;
}

// Tests
bool testRandomDinicSmall() {
	return testRandomDinicGraph(10, 25, 1000000000, 0, -1);
}
bool testRandomDinicMedium() {
	return testRandomDinicGraph(10000, 50000, 1000000000, 0, -1);
}
bool testRandomLinkCutDinicSmall() {
	return testRandomDinicGraph(10, 25, 1000000000, 0, 0);
}
bool testRandomLinkCutDinicMedium() {
	return testRandomDinicGraph(10000, 50000, 1000000000, 0, 0);
}
bool testRandomMixedDinicMedium() {
	return testRandomDinicGraph(10000, 50000, 1000000000, 0, 100);
}
// Hard graph
bool testHardDinicSmall() {
	return timeHardDinicGraph(10, 10, 10, -1);
}

// Performance tests
bool timeRandomDinicMedium() {
	return timeRandomDinicGraph(10000, 50000, 1000000000, 0, -1);
}
bool timeRandomDinicLarge() {
	return timeRandomDinicGraph(1000000, 5000000, 1000000000, 0, -1);
}
bool timeRandomLinkCutDinicMedium() {
	return timeRandomDinicGraph(10000, 50000, 1000000000, 0, 0);
}
bool timeRandomLinkCutDinicLarge() {
	return timeRandomDinicGraph(1000000, 5000000, 1000000000, 0, 0);
}
bool timeRandomMixedDinicMedium() {
	return timeRandomDinicGraph(10000, 50000, 1000000000, 0, 100);
}
bool timeRandomMixedDinicLarge() {
	return timeRandomDinicGraph(1000000, 5000000, 1000000000, 0, 100);
}
// Hard graph
bool timeHardDinicMedium() {
	return timeHardDinicGraph(200, 200, 200, -1);
}
bool timeHardLinkCutDinicMedium() {
	return timeHardDinicGraph(200, 200, 200, 0);
}
bool timeHardLinkCutDinicLarge() {
	return timeHardDinicGraph(600, 600, 600, 0);
}
bool timeHardFairDinicMedium() {
	return timeHardDinicGraph(200, 200, 1, -1);
}
bool timeHardFairLinkCutDinicMedium() {
	return timeHardDinicGraph(200, 200, 1, 0);
}

TestGroup getDinicTests() {
	std::vector<Test> tests (5);
	// On random graphs
	tests[0] = makeTest(testRandomDinicSmall, "test Dinic RandomSmall", false);
	tests[1] = makeTest(testRandomDinicMedium, "test Dinic RandomMedium", false);
	tests[2] = makeTest(testRandomLinkCutDinicSmall, "test LinkCutDinic RandomSmall", false);
	tests[3] = makeTest(testRandomLinkCutDinicMedium, "test LinkCutDinic RandomMedium", false);
	tests[4] = makeTest(testRandomMixedDinicMedium, "test MixedDinic RandomMedium", false);
	return makeTestGroup(tests, "dinic tests", true);
}
TestGroup getDinicTimeTests() {
	std::vector<Test> tests (11);
	// On random graphs
	tests[0] = makeTest(timeRandomDinicMedium, "time Dinic RandomMedium", true);
	tests[1] = makeTest(timeRandomDinicLarge, "time Dinic RandomLarge", true);
	tests[2] = makeTest(timeRandomLinkCutDinicMedium, "time LinkCutDinic RandomMedium", true);
	tests[3] = makeTest(timeRandomLinkCutDinicLarge, "time LinkCutDinic RandomLarge", true);
	tests[4] = makeTest(timeRandomMixedDinicMedium, "time MixedDinic RandomMedium", true);
	tests[5] = makeTest(timeRandomMixedDinicLarge, "time MixedDinic RandomLarge", true);
	// On worst-case graph
	tests[6] = makeTest(timeHardDinicMedium, "time Dinic HardMedium", true);
	tests[7] = makeTest(timeHardLinkCutDinicMedium, "time LinkCutDinic HardMedium", true);
	tests[8] = makeTest(timeHardLinkCutDinicLarge, "time LinkCutDinic HardLarge", true);
	tests[9] = makeTest(timeHardFairDinicMedium, "time Dinic FairHardMedium", true);
	tests[10] = makeTest(timeHardFairLinkCutDinicMedium, "time LinkCutDinic FairHardMedium", true);
	return makeTestGroup(tests, "dinic performance tests", true);
}

