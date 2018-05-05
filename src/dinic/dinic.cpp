#include <assert.h> // assert
#include <utility> // std::move
#include "./../linkcut/link_cut_functions.h" // Link/Cut functions
#include "./../util/vector.h" // std::vector
#include "./../util/two_dim_array.h" // Two dimensional vector
#include "dinic.h"

#include <iostream> // TODO: remove

FlowGraph::FlowGraph(const FlowGraph& oth) :
	n(oth.n), m(oth.m), source(oth.source), sink(oth.sink),
	edge_source(oth.edge_source), edge_target(oth.edge_target),
	flow(oth.flow), capacity(oth.capacity), edges(oth.edges) {
}

FlowGraph::FlowGraph(int n, int m, int source, int sink, Vector<int>&& edge_source, Vector<int>&& edge_target, Vector<int>&& flow, Vector<int>&& capacity) {
	this->n = n;
	this->m = m;
	this->source = source;
	this->sink = sink;
	this->edge_source = edge_source;
	this->edge_target = edge_target;
	this->flow = flow;
	this->capacity = capacity;

	// Init edge array from edge data
	// edges[i][j] = edge, means that the j'th edge coming or leaving from node "i" has index "edge"
	Vector<int> conns (n, 0);
	for (int i = 0; i < m; ++i) {
		++conns[edge_source[i]];
		++conns[edge_target[i]];
	}
	TwoDimArray<int> tmp (2*m, conns);
	// Fill tmp
	Vector<int> inds (n, 0);
	for (int i = 0; i < m; ++i) {
		int es = edge_source[i];
		int et = edge_target[i];
		tmp[es][inds[es]] = i;
		tmp[et][inds[et]] = i;
		++inds[es];
		++inds[et];
	}
	edges = std::move(tmp);
}

/// Empty namespace.
/// Hides visibility of functions and structs.
namespace {
	/// Graph representing a level graph on a flow graph.
	/// A level graph is a grpah consisting of edges such that:
	/// 1. All paths from the source to the sink have the same length.
	/// 2. All paths of this length from the source to the sink appear on the path.
	struct LevelGraph {
		/// Edges that leave this node.
		TwoDimArray<int> edges;
		/// Index of active edge for every node.
		/// Active edge is the outgoing edge from a node that we select when leaving that node.
		Vector<int> active_inds;

		/// Initializes a level graph.
		/// Data still has to be filled afterwards.
		LevelGraph(int total_edges, const Vector<int>& edge_counts)
			: edges(total_edges, edge_counts), active_inds(edge_counts.size()) {}

		/// Initializes the graph.
		/// Copies edges from base_edges.
		LevelGraph(int total_edges, int nodes, const int* offsets)
			: edges(total_edges, nodes, offsets), active_inds(nodes) {};

		/// What is the active edge leaving this node?
		/// Returns -1 if no active edge.
		inline int getActiveEdge(int index) {
			if (active_inds[index] == -1) return -1;
			else return edges[index][active_inds[index]];
		}

		/// Switches active edge for this node to the next edge.
		/// Returns -1 if no next edge exists.
		inline int nextEdge(int index) {
			--active_inds[index];
			if (active_inds[index] <= -1) return -1;
			else return getActiveEdge(index);
		}
	};

	/// Flow graph to do dinic on.
	FlowGraph* graph = nullptr;
	/// Current level graph.
	LevelGraph* level_graph = nullptr;

	/// Push flow in the graph.
	inline void pushFlow(int source, int edge, int amount) {
		if (source == graph->edge_source[edge]) graph->flow[edge] += amount;
		else graph->flow[edge] -= amount;
	}
	/// Get capacity of edge "edge" leaving from source, in the residual graph, with flow taken into account.
	inline int getCapacity(int source, int edge) {
		if (source == graph->edge_source[edge]) return graph->capacity[edge] - graph->flow[edge];
		else return graph->flow[edge];
	}
	/// Get other endpoint of edge "edge".
	inline int getOther(int source, int edge) {
		if (source == graph->edge_source[edge]) return graph->edge_target[edge];
		return graph->edge_source[edge];
	}

	/// A Link/Cut node.
	/// When this node lazily pushes dec_value to its children, It also pushes flow in the graph.
	struct DinicLinkCutNode {
		/// Left child.
		DinicLinkCutNode* left;
		/// Right child.
		DinicLinkCutNode* right;
		/// Parent.
		DinicLinkCutNode* parent;
		/// Capacity of edge leaving from this node.
		int value;
		/// Minimum value in this node's subtree in its chain.
		int min_value;
		/// What to decrement all nodes in this node's subtree by.
		int dec_value;
		/// Index of this node.
		int index;

		DinicLinkCutNode(int val, int ind) {
			value = val;
			index = ind;
			min_value = val;
			dec_value = 0;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		~DinicLinkCutNode() = default;

		/// Returns minimum value in this node's subtree.
		/// Used so that we don't need to push the node beforehand.
		inline int getMinVal() {
			return min_value - dec_value;
		}

		/// Assumes that this node has been pushed. 
		inline void update() {
			min_value = value;
			if (left != nullptr && left->getMinVal() < min_value) min_value = left->getMinVal();
			if (right != nullptr && right->getMinVal() < min_value) min_value = right->getMinVal();
		}

		/// Push the decrement from this node to its children.
		/// Also, push flow in this node's edge.
		inline void push() {
			if (dec_value > 0) {
				// Push value to childs
				if (left != nullptr) left->dec_value += dec_value;
				if (right != nullptr) right->dec_value += dec_value;
				// Push flow from this node's active edge, assuming it is not the sink
				if (index != graph->sink) {
					min_value -= dec_value;
					value -= dec_value;
					pushFlow(index, level_graph->getActiveEdge(index), dec_value);
				} else {
					update();
				}
				dec_value = 0;
			}
		}

		/// Is this node its chain's root.
		inline bool isRoot() {
			return (parent == nullptr || ((parent->left != this) && (parent->right != this)));
		}
	};

	/// Builds the level graph.
	/// Level graph variable has to be initialized first.
	/// returns length of level graph.
	/// Returns -1 if no paths from the source to the sink exist in the residual graph.
	int buildLevelGraph() {
		int n = graph->n;
		int m = graph->m;

		for (int i = 0; i < n; ++i) {
			level_graph->active_inds[i] = -1;
		}

		Vector<int> dist (n, n);
		Vector<int> que;
		que.reserve(n);

		// Basic bfs from the sink, in the residual graph, with inverted edges.
		// Edge a->b is added to the level graph if
		// dist[a] = dist[b] + 1
		que.push(graph->sink);
		assert(que.size() == 1);

		dist[graph->sink] = 0;
		for (int i = 0; i < que.size(); ++i) {
			assert(que.size() > i);
			int ind = que[i];
			assert(que.size() > i);
			for (int j = 0; j < graph->edges.getArraySize(ind); ++j) {
				int edge = graph->edges[ind][j];
				int target = getOther(ind, edge);
				if (getCapacity(target, edge) > 0) {
					if (dist[target] >= dist[ind] + 1) {
						if (dist[target] > dist[ind] + 1) {
							que.push(target);
							dist[target] = dist[ind] + 1;
						}
						++level_graph->active_inds[target];
						
						assert(level_graph->edges.getArraySize(target) > level_graph->active_inds[target]);
						level_graph->edges[target][level_graph->active_inds[target]] = edge;
					}
				}
			}
		}


		int res = dist[graph->source];
		if (res == n) return -1;
		else return res;
	}

	/// DFS flow in the flow graph.
	/// Basically, just find paths to the sink, and backtrack once a edge on the path has been saturated
	/// Or the current node has no outgoing edges.
	// prev_min shuld be infinity. 2^60 is good enough.
	long long dfsFlow(int ind, long long prev_min = 1ll<<60ll) {
		if (ind == graph->sink) return prev_min;
		long long res = 0; // How much flow we pushed
		while(res < prev_min) {
			int edge = level_graph->getActiveEdge(ind);
			if (edge == -1) break;
			int cap = getCapacity(ind, edge);
			if (cap == 0) {
				level_graph->nextEdge(ind);
				continue;
			}
			int t = getOther(ind, edge);
			// We cannot push more than previous edges (or the current) on the path can sustain.
			long long sub = dfsFlow(t, std::min(prev_min - res, (long long)cap));
			if (sub == 0) {
				// No flow can be pushed anymore through this node, so move to the next.
				level_graph->nextEdge(ind);
				continue;
			}
			res += sub;
			pushFlow(ind, edge, sub);
			if (sub == cap) level_graph->nextEdge(ind);
		}
		return res;
	}
}	// Empty namespace

// Regular dinic that only uses dfs.
long long basicDinic(FlowGraph* flow_graph) {
	// Init graph and level_graph
	graph = flow_graph;
	int n = graph->n;
	int m = graph->m;
	

	LevelGraph lg (2*m, n, graph->edges.getOffset());
	level_graph = &lg;

	// how much flow we pushed at the end
	long long ans = 0;
	// Perform dinic
	while(true) {
		// Find level graph
		int dist = buildLevelGraph();
		if (dist == -1) break;
	
		// Saturate level graph
		long long cap = dfsFlow(graph->source);
		if (cap == 0) break;
		ans += cap;
	}

	graph = nullptr;
	level_graph = nullptr;

	// Return result
	return ans;
}
long long dinic(FlowGraph* flow_graph, int swap_const = 100) {
	// Init graph and level_graph
	graph = flow_graph;
	int n = graph->n;
	int m = graph->m;
	
	LevelGraph lg (2*m, n, graph->edges.getOffset());
	level_graph = &lg;

	// Reserve memory for link/cut tree
	Vector<DinicLinkCutNode*> link_cut_nodes (n);
	for (int i = 0; i < n; ++i) link_cut_nodes[i] = new DinicLinkCutNode(0, i);

	// Define for convenience
	DinicLinkCutNode* source_node = link_cut_nodes[graph->source];
	DinicLinkCutNode* sink_node = link_cut_nodes[graph->sink];
	sink_node->value = ((1<<30)-1) + (1<<30); // Max edge weight

	// how much flow we pushed at the end
	long long ans = 0;
	// Perform dinic
	while(true) {
		// Find level graph
		int dist = buildLevelGraph();
		if (dist == -1) break;
		if (dist < swap_const) {
			// Saturate level graph
			long long cap = dfsFlow(graph->source);
			if (cap == 0) break;
			ans += cap;
		} else {
			// Init link/cut tree
			for (int ind = 0; ind < n; ++ind) {
				int edge = level_graph->getActiveEdge(ind);
				DinicLinkCutNode* node = link_cut_nodes[ind];
				if (edge != -1) {
					node->value = getCapacity(ind, edge);
					node->update();
					linkChild(node, link_cut_nodes[getOther(ind, edge)]);
				}
			}
			
			// Saturate level graph
			while (true) {
				// Find path to saturate
				int cap = 0;
				while (true) {
					// Make sure that the path is not already saturated
					access(source_node);
					cap = source_node->getMinVal();
					// If path doesn't lead to root, it ends in a node with value 0
					// So this check is enough
					if (cap > 0) break;

					// Get node that is saturated
					DinicLinkCutNode* node = pathMinNode(source_node);

					// Swap active edge for that node.
					while(true) {
						cutParent(node);
						
						int ind = node->index;
						int edge = level_graph->nextEdge(ind);
						if (edge == -1) {
							node->value = 0;
							node->update();
							// Paths from this node to sink no longer exist
							// Cut path leading to this node.
							access(source_node);
							node = findNext(node);
							if (node == nullptr) {
								// If node was root; no paths exist
								break;
							}
							continue;
						} else {
							node->value = getCapacity(ind, edge);
							node->update();
							assert(subtreeRoot(node) == node);
							assert(node != subtreeRoot(link_cut_nodes[getOther(ind, edge)]));
							linkChild(node, link_cut_nodes[getOther(ind, edge)]);
							break; // Active edge swapped, break
						}
					}
					// If no paths exist
					if (node == nullptr) break;
				}
				if (cap == 0) break; // When we can't push any more flow
				// Push flow
				splay(source_node);
				source_node->dec_value += cap;
				ans += cap;
			}

			// Clear link/cut tree
			for (int ind = 0; ind < n; ++ind) {
				DinicLinkCutNode* node = link_cut_nodes[ind];
				access(node); // To make sure that every node gets pushed
				node->push();
				cutParent(node);
			}
		}
	}

	// Clean up
	for (int i = 0; i < n; ++i) delete link_cut_nodes[i];
	
	graph = nullptr;
	level_graph = nullptr;

	// Return result
	return ans;
}
