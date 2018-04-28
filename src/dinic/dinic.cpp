#include <vector>
#include "./../util/two_dim_array.h"
#include "./../linkcut/link_cut_functions.h"
#include "dinic.h"


// Empty namespace to hide these functions and structs
namespace {
	struct LevelGraph {
		TwoDimArray<int> edges;		// Edges that are included in the level graph
		std::vector<int> active_inds;	// Indexes of active edges

		LevelGraph(int total_edges, std::vector<int> edge_counts)
			: edges(total_edges, edge_counts), active_inds(edge_counts.size()) {}
		LevelGraph(int total_edges, int nodes, int* edge_counts)
			: edges(total_edges, nodes, edge_counts), active_inds(nodes) {}

		inline int getActiveEdge(int index) {
			if (active_inds[index] == -1) return -1;
			else return edges[index][active_inds[index]];
		}

		// Switches active edge for this node to the next edge
		// Returns -1 if no next edge exists
		inline int nextEdge(int index) {
			--active_inds[index];
			if (active_inds[index] == -1) return -1;
			else return getActiveEdge(index);
		}
	};

	FlowGraph* graph = nullptr;
	LevelGraph* level_graph = nullptr;

	inline void pushFlow(int source, int edge, int amount) {
		if (source == graph->edge_source[edge]) graph->flow[edge] += amount;
		else graph->flow[edge] -= amount;
	}
	inline int getCapacity(int source, int edge) {
		if (source == graph->edge_source[edge]) return graph->capacity[edge] - graph->flow[edge];
		else return graph->flow[edge];
	}
	inline int getOther(int source, int edge) {
		if (source == graph->edge_source[edge]) return graph->edge_target[edge];
		return graph->edge_source[edge];
	}

	struct DinicLinkCutNode {
		DinicLinkCutNode* left;
		DinicLinkCutNode* right;
		DinicLinkCutNode* parent;
		int value;
		int min_value;
		int dec_value;
		int index;

		DinicLinkCutNode(int val, int ind);
		~DinicLinkCutNode() = default;

		// inline so defined in header
		inline int getMinVal() {
			return min_value - dec_value;
		}

		// Assumes that this node has been pushed. 
		inline void update() {
			min_value = value;
			if (left != nullptr && left->getMinVal() < min_value) min_value = left->getMinVal();
			if (right != nullptr && right->getMinVal() < min_value) min_value = right->getMinVal();
		}

		inline void push() {
			if (dec_value > 0) {
				value -= dec_value;
				min_value -= dec_value;
				// Push flow from this node's active edge, assuming it is not the sink
				if (index != graph->sink) {
					pushFlow(index, level_graph->getActiveEdge(index), dec_value);
				}

				// Push value to childs
				if (left != nullptr) left->dec_value += dec_value;
				if (right != nullptr) right->dec_value += dec_value;
				dec_value = 0;
			}
		}

		inline bool isRoot() {
			return (parent == nullptr || ((parent->left != this) && (parent->right != this)));
		}
	};

	// returns length of level graph. If -1, no level graph could be built.
	int buildLevelGraph() {
		int n = graph->n;
		int m = graph->m;

		for (int i = 0; i < n; ++i) {
			level_graph->active_inds[i] = -1;
		}

		std::vector<int> dist(n, n);
		std::vector<int> que;
		que.reserve(n);
		que.push_back(graph->sink);
		dist[graph->sink] = 0;
		for (int i = 0; i < que.size(); ++i) {
			int ind = que[i];
			for (int j = 0; j < graph->edges.getArraySize(ind); ++j) {
				int edge = graph->edges[ind][j];
				std::cout << "Handling edge " << edge << "\n";
				int target = getOther(ind, edge);
				if (getCapacity(target, edge) > 0) {
					if (dist[target] >= dist[i] + 1) {
						dist[target] = dist[i] + 1;
						++level_graph->active_inds[target];
						level_graph->edges[target][level_graph->active_inds[target]] = edge;
					}
				}
			}
		}
		int res = dist[graph->source];
		if (res == n) return -1;
		else return res;
	}

	int dfsFlow(int ind, int prev_min) {
		if (ind == graph->sink) return prev_min;
		int res = 0;
		while(res < prev_min) {
			int edge = level_graph->getActiveEdge(ind);
			if (edge == -1) break;
			int cap = getCapacity(ind, edge);
			if (cap == 0) {
				level_graph->nextEdge(ind);
				continue;
			}
			int t = getOther(ind, edge);
			int sub = dfsFlow(t, std::min(prev_min - res, cap));
			res += sub;
			pushFlow(ind, edge, sub);
			if (sub == cap) level_graph->nextEdge(ind);
		}
		return res;
	}
}	// Empty namespace

int dinic(FlowGraph* flow_graph, bool use_dfs) {
	// Init graph and level_graph
	graph = flow_graph;
	int n = graph->n;
	int m = graph->m;
	
	LevelGraph lg (2*m, n, graph->edges.data);
	level_graph = &lg;
	
	std::cout << "At least here: " << '\n';
	graph->edges.print();

	/*
	// Reserve memory for link/cut tree
	std::vector<DinicLinkCutNode*> link_cut_nodes (n);
	for (int i = 0; i < n; ++i) link_cut_nodes[i] = new DinicLinkCutNode(0, i);

	// Define for convenience
	DinicLinkCutNode* source_node = link_cut_nodes[graph->source];
	DinicLinkCutNode* sink_node = link_cut_nodes[graph->sink];
	*/


	// Perform dinic
	int ans = 0;
	while(true) {
		// Find level graph
		int dist = buildLevelGraph();
		if (dist == -1) break;
		std::cout << "Level graph: " << dist << '\n';

		// Use dfs if specified to do so, otherwise use link/cut tree
		if (use_dfs) {
			// Saturate level graph
			const int inf = ((1<<30)-1) + (1<<30); // Max value of a int
			int cap = dfsFlow(graph->source, inf);
			if (cap == 0) break;
			ans += cap;
		} else {
			/*
			// Init link/cut tree
			for (int ind = 0; ind < n; ++ind) {
				int edge = level_graph->activeEdge(i);
				DinicLinkCutNode* node = link_cut_nodes[ind];
				if (edge != -1) {
					node->value = getCapacity(ind, edge);
					node->update();
					linkChild(node, link_cut_nodes[getOther(node, edge)]);
				}
			}

			// Saturate level graph
			while (true) {
				// Find path to saturate
				int cap = 0;
				while (true) {
					// Make sure that the path is not already saturated
					access(source_node);
					cap = source->getMinVal();
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
							node = nextNode(node);
							if (node == nullptr) {
								// If node was root; no paths exist
								break;
							}
							continue;
						} else {
							node->value = getCapacity(ind, edge);
							node->update();
							linkChild(node, link_cut_nodes[getOther(node, edge)]);
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
				push(node);
				cutParent(node);
			}
			*/
		}
	}

	/*
	// Clean up
	for (int i = 0; i < n; ++i) delete link_cut_nodes[i];
	*/
	graph = nullptr;
	level_graph = nullptr;

	// Return result
	return ans;
}
