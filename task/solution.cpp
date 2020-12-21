#include <bits/stdc++.h>
using namespace std;

constexpr int kBruteForceLimit = 2 * 2 * 2;

void read(int count, vector<set<int>> &vs1, vector<set<int>> &vs2) {
  for (int i = 0; i < count; ++i) {
    int num;
    scanf("%d", &num);
    while (num--) {
      int j;
      scanf("%d", &j);
      --j;
      vs1[i].insert(j);
      vs2[j].insert(i);
    }
  }
}

int main() {
  int A, C, T;
  scanf("%d %d %d", &A, &C, &T);

  vector<set<int>> AC(A), AT(A);
  vector<set<int>> CA(C), CT(C);
  vector<set<int>> TA(T), TC(T);
  read(A, AC, CA);
  read(A, AT, TA);
  read(C, CT, TC);
  
  vector<tuple<int, int, int>> triangles;
  map<tuple<int, int, int>, int> triangle_index;
  for (int a = 0; a < A; ++a) {
    for (int c : AC[a]) {
      for (int t : AT[a]) {
        if (CT[c].count(t) > 0) {
          tuple<int, int, int> triangle = make_tuple(a, c, t);
          triangle_index[triangle] = triangles.size();
          triangles.push_back(triangle);
        }
      }
    }
  }
  vector<vector<int>> adj(triangles.size());

  auto add_edge = [&] (
      tuple<int, int, int> triangle1, tuple<int, int, int> triangle2) {
    adj[triangle_index[triangle1]].push_back(triangle_index[triangle2]);
  };

  for (tuple<int, int, int> triangle : triangles) {
    int a = get<0>(triangle);
    int c = get<1>(triangle);
    int t = get<2>(triangle);
    for (int t_prime : AT[a]) {
      if (t_prime != t && triangle_index.count(make_tuple(a, c, t_prime))) {
        add_edge(triangle, make_tuple(a, c, t_prime));
      }
    }
    for (int c_prime : AC[a]) {
      if (c_prime != c && triangle_index.count(make_tuple(a, c_prime, t))) {
        add_edge(triangle, make_tuple(a, c_prime, t));
      }
    }
    for (int a_prime : CA[c]) {
      if (a_prime != a && triangle_index.count(make_tuple(a_prime, c, t))) {
        add_edge(triangle, make_tuple(a_prime, c, t));
      }
    }
  }

  vector<bool> visited(triangles.size(), false);
  int answer = 0;
  vector<int> nodes;

  function<int (int)> count_edge = [&] (int u) {
    nodes.push_back(u);
    int edge = adj[u].size();
    for (int v : adj[u]) {
      if (!visited[v]) {
        visited[v] = true;
        edge += count_edge(v);
      }
    }
    return edge;
  };

  for (int i = 0; i < triangles.size(); ++i) {
    if (!visited[i]) {
      visited[i] = true;
      nodes.clear();
      int edge = count_edge(i) >> 1;
      if (nodes.size() <= kBruteForceLimit) {
        vector<int> bit_neighbour(nodes.size());
        for (int j = 0; j < nodes.size(); ++j) {
          for (int neighbour : adj[nodes[j]]) {
            bit_neighbour[j] |= 1 << (find(nodes.begin(),
                                           nodes.end(),
                                           neighbour) - nodes.begin());
          }
        }
        int max_nodes = 0;
        for (int bit = 0; bit < (1 << nodes.size()); ++bit) {
          bool valid = true;
          for (int j = 0; j < nodes.size() && valid; ++j) {
            if ((bit & (1 << j)) && (bit & bit_neighbour[j])){
              valid = false;
            }
          }
          if (valid) {
            max_nodes = max(max_nodes, __builtin_popcount(bit));
          }
        }
        answer += max_nodes;
      } else {
        if (edge == nodes.size()) {
          answer += (nodes.size()) >> 1;
        } else {
          answer += (nodes.size() + 1) >> 1;
        }
      }
    }
  }
  printf("%d\n", answer);
}
