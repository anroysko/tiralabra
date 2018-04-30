#include <vector>
#include "./../util/two_dim_array.h"
#include "./../linkcut/link_cut_functions.h"
#include "dinic.h"

// Definitions of flowgraph's constructors
FlowGraph::FlowGraph(const FlowGraph& oth) :
	n(oth.n), m(oth.m), source(oth.source), sink(oth.sink),
	edge_source(oth.edge_source), edge_target(oth.edge_target),
	flow(oth.flow), capacity(oth.capacity), edges(oth.edges) {
}
FlowGraph::FlowGraph(int n, int m, int source, int sink, std::vector<int>&& edge_source, std::vector<int>&& edge_target, std::vector<int>&& flow, std::vector<int>&& capacity) {
	this->n = n;
	this->m = m;
	this->source = source;
	this->sink = sink;
	this->edge_source = edge_source;
	this->edge_target = edge_target;
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
		int es = edge_source[i];
		int et = edge_target[i];
		tmp[es][inds[es]] = i;
		tmp[et][inds[et]] = i;
		++inds[es];
		++inds[et];
	}
	edges = tmp;
	tmp.data = nullptr;
	tmp.offset = nullptr;
}

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
		std::vector<bool> handled(n, false);
		que.reserve(n);
		que.push_back(graph->sink);
		dist[graph->sink] = 0;
		for (int i = 0; i < que.size(); ++i) {
			int ind = que[i];
			for (int j = 0; j < graph->edges.getArraySize(ind); ++j) {
				int edge = graph->edges[ind][j];
				int target = getOther(ind, edge);
				if (getCapacity(target, edge) > 0) {
					if (dist[target] >= dist[ind] + 1) {
						if (dist[target] > dist[ind] + 1) {
							que.push_back(target);
							dist[target] = dist[ind] + 1;
						}
						// std::cout << "Add level graph edge from to edge " << target << ' ' << ind << ' ' << edge << '\n';
						++level_graph->active_inds[target];
						level_graph->edges[target][level_graph->active_inds[target]] = edge;
					}
				}
			}
		}

		// for (int i = 0; i < n; ++i) std::cout << dist[i] << ' ' ; std::cout << '\n';

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
			// std::cout << graph->edge_source[edge] << ' ' << graph->edge_target[edge] << "\n";
			int sub = dfsFlow(t, std::min(prev_min - res, cap));
			// std::cout << ind << " " << "dfsFlow: " << edge << " " << cap << " " << t << " " << sub << "\n";
			if (sub == 0) {
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

int dinic(FlowGraph* flow_graph, bool use_dfs) {
	// Init graph and level_graph
	graph = flow_graph;
	int n = graph->n;
	int m = graph->m;
	
	// std::cout << "Dinic on:\n";
	// graph->edges.print();
	// for (int i = 0; i < n; ++i) std::cout << graph->edges.offset[i] << ' '; std::cout << '\n';

	LevelGraph lg (2*m, n, graph->edges.offset);
	level_graph = &lg;

	// Reserve memory for link/cut tree
	std::vector<DinicLinkCutNode*> link_cut_nodes (n);
	for (int i = 0; i < n; ++i) link_cut_nodes[i] = new DinicLinkCutNode(0, i);

	// Define for convenience
	DinicLinkCutNode* source_node = link_cut_nodes[graph->source];
	DinicLinkCutNode* sink_node = link_cut_nodes[graph->sink];
	sink_node->value = ((1<<30)-1) + (1<<30);

	// Perform dinic
	int ans = 0;
	while(true) {
		// Find level graph
		int dist = buildLevelGraph();
		if (dist == -1) break;
		// std::cout << "Level graph: " << dist << '\n';
		// level_graph->edges.print();
	
		// Use dfs if specified to do so, otherwise use link/cut tree
		if (use_dfs) {
			// Saturate level graph
			const int inf = ((1<<30)-1) + (1<<30); // Max value of a int
			int cap = dfsFlow(graph->source, inf);
			if (cap == 0) break;
			ans += cap;
		} else {
			// Init link/cut tree
			for (int ind = 0; ind < n; ++ind) {
				int edge = level_graph->getActiveEdge(ind);
				DinicLinkCutNode* node = link_cut_nodes[ind];
				if (edge != -1) {
					// std::cout << "Add parent " << getOther(ind, edge) << " to " << ind << "\n";
					node->value = getCapacity(ind, edge);
					node->update();
					linkChild(node, link_cut_nodes[getOther(ind, edge)]);
				}
			}
			
			// std::cout << "Here!\n";
			
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
							// std::cout << "Add parent " << getOther(ind, edge) << " to " << ind << "\n";
							node->value = getCapacity(ind, edge);
							node->update();
							linkChild(node, link_cut_nodes[getOther(ind, edge)]);
							break; // Active edge swapped, break
						}
					}
					// If no paths exist
					if (node == nullptr) break;
				}
				if (cap == 0) break; // When we can't push any more flow
				// Push flow
				// std::cout << "Push " << cap << "\n";
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
