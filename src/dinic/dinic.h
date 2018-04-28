#ifndef __DINIC_DINIC_H_
#define __DINIC_DINIC_H_

#include <vector>
#include "./../util/two_dim_array.h"

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

	FlowGraph(int n, int m, int source, int sink, std::vector<int>& edge_source, std::vector<int>& edge_target, std::vector<int>& flow, std::vector<int>& capacity) {
		this->n = n;
		this->m = m;
		this->source = source;
		this->sink = sink;
		this->edge_source = edge_source;
		this->edge_target = edge_source;
		this->flow = flow;
		this->capacity = capacity;

		// Init edge array
		std::vector<int> conns (n, 0);
		for (int i = 0; i < m; ++i) {
			++conns[edge_source[i]];
			++conns[edge_target[i]];
		}
		TwoDimArray<int> tmp (2*m, conns);
		std::vector<int> inds (n, 0);
		for (int i = 0; i < m; ++i) {
			tmp[edge_source[i]][inds[edge_source[i]]] = i;
			tmp[edge_target[i]][inds[edge_target[i]]] = i;
			std::cout << "edge: " << edge_source[i] << ' ' << edge_target[i] << '\n';
			++inds[edge_source[i]];
			++inds[edge_target[i]];
		}
		edges = tmp;
		tmp.data = nullptr;
		tmp.offset = nullptr;
		edges.print();
	}
};

// Dinic's algorithm
// IN: graph, flowgraph to do dinic on
// OUT: int, giving how much flow was pushed. the flow graph is modified to signify the pushed flow
// Complexity: O(E log V)
int dinic(FlowGraph* graph, bool use_dfs = true);

#endif // __DINIC_DINIC_H_
