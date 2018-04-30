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

// Dinic's algorithm
// IN: flowgraph to do dinic on
// OUT: int, giving how much flow was pushed. the flow graph is modified to signify the pushed flow
// Complexity: O(E log V)
int dinic(FlowGraph* graph, bool use_dfs);

#endif // __DINIC_DINIC_H_
