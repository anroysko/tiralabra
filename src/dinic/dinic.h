#ifndef __DINIC_DINIC_H_
#define __DINIC_DINIC_H_

#include <vector>
#include "./../util/two_dim_array.h"

// Graph representing a max-flow problem.
struct FlowGraph {
	int n;
	int m;
	int source;
	int sink;

	std::vector<int> edge_source;
	std::vector<int> edge_target;
	std::vector<int> flow;
	std::vector<int> capacity;
	TwoDimArray<int> edges;
	
	FlowGraph(int n, int m, int source, int sink, std::vector<int>&& edge_source, std::vector<int>&& edge_target, std::vector<int>&& flow, std::vector<int>&& capacity);
	FlowGraph(const FlowGraph& graph);
};

// Dinic's algorithm with link/cut trees
// IN: flowgraph to do dinic on, levelgraph dist to use link/cut trees
// OUT: int, giving how much flow was pushed. the flow graph is modified to signify the pushed flow
// Complexity: O(EV log V) if swap_const is small enough
long long dinic(FlowGraph* graph, int swap_const);
// Dinic's algorithm
// IN: flowgraph to do dinic on
// OUT: int, giving how much flow was pushed. the flow graph is modified to signify the pushed flow
// Complexity: O(EV^2)
long long basicDinic(FlowGraph* graph);

#endif // __DINIC_DINIC_H_
