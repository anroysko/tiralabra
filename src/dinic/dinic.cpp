#include <iostream>
#include <vector>

template<class T>
struct ManyArrays {
	T* vals;
	int* starts;
	int array_count;
	int total_size;
	
	void init(int total_size, vector<int>& arrays) {
		this->total_size = total_size;
		this->vals = new T[total_size];
		this->starts = new int[arrays.size()];
		this->starts[0] = 0;
		for (int i = 1; i < arrays.size(); ++i) {
			starts[i] = starts[i-1] + arrays[i-1];
		}
		this->array_count = arrays.size();
	}

	ManyArrays() {
		vals = nullptr;
		starts = nullptr;
		array_count = 0;
		total_size = 0;
	}
	ManyArrays(int total_size, vector<int>& arrays) {
		init(total_size, arrays);
	}
	~ManyArrays() {
		if (vals != nullptr) delete[] vals;
		if (starts != nullptr) delete[] starts;
	}

	int arrays() {
		return array_count;
	}

	int size(int i) {
		if (i == array_count - 1) return total_size - starts[i];
		else return starts[i+1] - starts[i];
	}

	T* operator[](const int i) {
		return &vals[indexes[i]];
	}
};

namespace {
	struct DinicGraph {
		int source;
		int sink;
		std::vector<int> flow;		// Flow of edge j
		std::vector<int> capacity;	// Capacity of edge j
		std::vector<int> edge_target;	// Target of edge j
		std::vector<int> edge_source;	// Source of edge j
		FlowGraph flow_
		ManyArrays edges;		// Contains indexes of all edges from or to nodes in the graph
		ManyArrays level_edges;		// Temporary edges, representing the level graph
		std::vector<int> level_edges_sizes;
		
		DinicGraph(int n, int m, int source, int sink, std::vector<std::pair<std::pair<int, int>, int>> edges) : flow(m, 0), capacity(m), edge_target(m), edge_source(m), edges(), tmp_edges() {
			this->source = source;
			this->sink = sink;
			vector<int> conns(n, 0);
			vector<int> edge_inds(n, 0);
			for (int j = 0; j < m; ++j) {
				int src = edges[j].first.first;
				int tar = edges[j].first.second;
				int cap = edges[j].second;
				capacity[j] = cap;
				edge_target[j] = tar;
				edge_source[j] = src;
				++conns[tar];
				++conns[src];
			}
			edges.init(2*m. conns);
			tmp_edges.init(2*m, conns); // Only reserves memory, doesn't set values
			for (int j = 0; j < m; ++j) {
				int src = edges[j].first.first;
				int tar = edges[j].first.second;
				edges[src][edge_inds[src]] = j;
				edges[tar][edge_inds[tar]] = j;
				++edge_inds[src];
				++edge_inds[tar];
			}
		}

		inline void pushFlow(int edge, int target, int amount) {
			if (edge_target[edge] == target) flow[edge] += amount;
			else flow[edge] -= amount;
		}
		inline void getCapacity(int edge, int target) {
			return (edge_target[edge] == target) ? (capacity[edge] - flow[edge]) : (flow[edge]);
		}
		inline void getOther(int edge, int source) {
			return (edge_souce[edge] == source) ? (edge_target[edge]) : (edge_souce[edge]);
		}
		inline int size() {
			return edges.arrays(); // One array for one node -> amount of arrays is amount of nodes
		}
		int makeLevelGraph() {
			vector<int> ord;
			vector<int> dist (n, size()); // size() represents infinite, unreachable here
			ord.reserve(size());
			dist[sink] = 0;
			ord.push_back(sink);
			for (int a = 0; a < ord.size(); ++a) {
				int i = ord[a];
				for (int b = 0; b < edges.size(i); ++b) {
					int j = edges[i][b];
					if (getCapacity(j, i) == 0) continue;
					int t = getOther(j, i);
					if (dist[t] >= dist[i] + 1) {
						dist[t] = dist[i] + 1;
						level_edges[t][level_edges_sizes[t]] = j;
						++level_edges_sizes[t];
						ord.push_back(t);
					}
				}
			}
			if (dist[source] == size()) {
				return -1; // No paths from source to sink exist
			} else {
				return dist[source]; // depth of level graph
			}
		}
		
		int dfsLevelGraph(int i, int pathmin = (((1<<30)-1)+(1<<30)) ) {
			if (i == sink) return pathmin;
			for (int b = level_edges_sizes[i] - 1; b >= 0; --b) {
				int j = level_edges[i][b];
				int t = getOther(j, i);
				int cap = getCapacity(j, t);
				if (cap != 0) {
					int sub = dfsLevelGraph(t, std::min(pathmin, cap));
					if (sub != 0) {
						pushFlow(j, t, sub);
						return sub;
					}
				}
				--level_edges_sizes[i];			
			}
			return 0;
		}
		void pushLevelGraph() {
			while(true) {
				if (dfsLevelGraph(source) == 0) break;
			}
		}
		void pushLevelGraphLinkCut() {
			vector<int> values (size());
			for (int i = 0; i < size(); ++i) {
				if (level_edges_sizes[i] > 0) {
					int j = level_edges[i][0];
					values[i] = getCapacity(j, getOther(j, i));
				} else {
					values[i] = 0;
				}
			}
			LinkCutTree tree(size(), values);
			
		}
	};
} // Empty namespace

void dinic(

int main() {
	
}
