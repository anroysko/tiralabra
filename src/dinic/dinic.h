#ifndef __DINIC_DINIC_H_
#define __DINIC_DINIC_H_

#include <vector>
#include "./../util/two_dim_array.h"

// Graph representing a max-flow problem in a directed graph
struct FlowGraph {
	int n;
	int m;
	int source;
	int sink;

	std::vector<int> edge_source; // Source of edge i
	std::vector<int> edge_target; // Target of edge i
	std::vector<int> flow; // Current flow in edge i
	std::vector<int> capacity; // Max flow edge i can sustain
	TwoDimArray<int> edges; // What edges leave and come to each node?
	
	FlowGraph(int n, int m, int source, int sink, std::vector<int>&& edge_source, std::vector<int>&& edge_target, std::vector<int>&& flow, std::vector<int>&& capacity);
	FlowGraph(const FlowGraph& graph);
};

// Dinic's algorithm with link/cut trees
// IN: flowgraph: Graph to do dinic on; swap_const: how deep does the level graph need to be to use link/cut tree?
// OUT: int, giving how much flow was pushed. the flow graph is modified to signify the pushed flow
// Complexity: O(EV log V) if swap_const is small enough
long long dinic(FlowGraph* graph, int swap_const);
// basic Dinic's algorithm. Compared to dinic(graph, inf) doesn't have the overhead of initializing the link/cut tree
// IN: flowgraph to do dinic on
// OUT: int, giving how much flow was pushed. the flow graph is modified to signify the pushed flow
// Complexity: O(EV^2)
long long basicDinic(FlowGraph* graph);

#endif // __DINIC_DINIC_H_
