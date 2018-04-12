#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Regular dinic for reference
// Seems to be working pretty well, it got fastest code: https://cses.fi/dt/task/422/
// for compiling, g++ basic.cpp -o basic -std=c++11 -O2
// constants N and M give max amount of nodes and edges respectively

typedef long long ll;
const int N = 10000;
const int M = 10000;
const int inf = 1e9;

int flow[M];
int capacity[M];
int tar[M];
int src[M];

vector<int> edges[N];
vector<int> tmp_edges[N];
int dist[N];

ll res = 0;

int push_lim(int j, int s) {
	return (s == src[j]) ? (capacity[j] - flow[j]) : (flow[j]);
}

void push_flow(int j, int s, int v) {
	if (s == src[j]) {
		flow[j] += v;
	} else {
		flow[j] -= v;
	}
}
int target(int j, int s) {
	return (s == src[j] ? tar[j] : src[j]);
}

int dfs(int i, int sink, int min_cap) {
	if (i == sink) return min_cap;
	for (int j = 0; j < tmp_edges[i].size(); ++j) {
		int ed = tmp_edges[i][j];
		int lim = push_lim(ed, i);
		if (lim != 0) {
			int sub = dfs(target(ed, i), sink, min(lim, min_cap));
			if (sub > 0) {
				push_flow(ed, i, sub);
				return sub;
			}
		}
		tmp_edges[i][j] = tmp_edges[i].back();
		tmp_edges[i].pop_back();
		--j;
	}
	return 0;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;
	for (int i = 0; i < m; ++i) {
		int a, b, c;
		cin >> a >> b >> c;
		--a; --b;

		flow[i] = 0;
		capacity[i] = c;

		src[i] = a;
		tar[i] = b;

		edges[a].push_back(i);
		edges[b].push_back(i);
	}

	int source = 0;
	int sink = n-1;

	vector<int> que;
	que.reserve(n);

	while(true) {
		for (int i = 0; i < n; ++i) dist[i] = inf;
		dist[sink] = 0;
		
		que.clear();
		que.push_back(sink);
		for (int z = 0; z < que.size(); ++z) {
			int v = que[z];
			for (auto j : edges[v]) {
				int t = target(j, v);
				int pl = push_lim(j, t);
				if (pl <= 0) continue;
				if (dist[t] >= dist[v] + 1) {
					dist[t] = dist[v] + 1;
					tmp_edges[t].push_back(j);
					que.push_back(t);
				}
			}
		}
		// for (int i = 0; i < n; ++i) cout << dist[i] << ' '; cout << '\n';
		if (dist[source] == inf) break;
		// DFS paths in tmp_edges
		while(true) {
			int sub = dfs(source, sink, inf);
			if (!sub) break;
			res += sub;
		}
		for (int i = 0; i < n; ++i) tmp_edges[i].clear();
	}
	cout << res << '\n';
}
