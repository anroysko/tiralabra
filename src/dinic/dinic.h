#ifndef __DINIC_DINIC_H_
#define __DINIC_DINIC_H_

#include "./../util/vector.h" // Vector
#include "./../util/two_dim_array.h" // TwoDimArray

/// Graph representing a max-flow problem in a directed graph
struct FlowGraph {
	/// Amount of nodes
	int n;
	/// Amount of edges
	int m;
	/// Index of source
	int source;
	/// Index of sink
	int sink;

	/// Source of edge i.
	Vector<int> edge_source;
	/// Target of edge i.
	Vector<int> edge_target;
	/// Current flow over edge i.
	Vector<int> flow;
	/// Max flow over edge i.
	Vector<int> capacity;
	/// What edges leave and come to each node?
	TwoDimArray<int> edges;
	
	/// Build a flowgraph from the values.
	FlowGraph(int n, int m, int source, int sink, Vector<int>&& edge_source, Vector<int>&& edge_target, Vector<int>&& flow, Vector<int>&& capacity);
	/// Copy constructor.
	FlowGraph(const FlowGraph& graph);
};

/// Dinic's algorithm with link/cut trees.
/// In:
///     flowgraph: Graph to do dinic on;
///     swap_const: how deep does the level graph need to be to use link/cut tree?
/// Out:
///     int, giving how much flow was pushed.
/// Side effects:
///     the flow graph is modified to signify the pushed flow
/// Complexity:
///     O(EV log V) if swap_const is small enough
long long dinic(FlowGraph* graph, int swap_const);

/// Regular dfs dinic's algorithm
/// This is faster than calling dinic swap_const = infinity, since this doesn't need to initialize link/cut nodes.
/// In:
///     flowgraph: Graph to do dinic on;
/// Out:
///     int, giving how much flow was pushed.
/// Side effects:
///     the flow graph is modified to signify the pushed flow
/// Complexity:
///     O(E^2 V)
long long basicDinic(FlowGraph* graph);

#endif // __DINIC_DINIC_H_
