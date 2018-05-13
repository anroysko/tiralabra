#include <iostream> // cin, cout
#include <utility> // move

#include "util/array.h" // Vector
#include "dinic/dinic.h" // dinic

int main() {
	// Optimize I/O
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);

	// Construct flow graph from input
	int n, m, source, sink; // Nodes, edges
	std::cin >> n >> m >> source >> sink;
	Array<int> edge_source(m);
	Array<int> edge_target(m);
	Array<int> flow(m, 0);
	Array<int> capacity(m);
	for (int i = 0; i < m; ++i) {
		std::cin >> edge_source[i] >> edge_target[i] >> capacity[i];
	}
	FlowGraph graph (n, m, source, sink, std::move(edge_source), std::move(edge_target), std::move(flow), std::move(capacity));
	
	// Solve the flow problem with dinic
	int res = dinic(&graph, 100);

	// Output solution
	std::cout << res << "\n";
	for (int i = 0; i < m; ++i) std::cout << graph.flow[i] << ' ';
	std::cout << '\n';
}
