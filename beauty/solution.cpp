#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
const int LOG = 17;

struct Fenwick {
  vector<long long> tree;
  int n;

  void init(int _n) {
    n = _n;
    tree.resize(n+1, 0);
    for (int i = 0 ; i <= n ; i++) {
      tree[i] = 0;
    }
  }

  void update(int idx, long long val) {
    for (; idx <= n ; idx += idx & -idx) {
      tree[idx] += val;
    }
  }

  long long query(int idx) {
    long long ret = 0;
    while (idx > 0) {
      ret += tree[idx];
      idx -= idx & -idx;
    }
    return ret;
  }

  long long query(int x, int y) {
    return query(y) - query(x-1);
  }
};

vector<int> chld[N];
int parent[N];
int n, q;

int depth[N];
int tin[N], tout[N];
vector<int> tinListByDepth[N+LOG+5];

Fenwick updateOneInSubtree; // calc value added due to update in subtree, with k = 1
Fenwick updateOneFromAncestor; // calc value added due to update in ancestor, with k = 1
Fenwick updateMoreThanOneInSubtree; // calc value added due to update in subtree, with k > 1
long long updateToDescendant[N][LOG]; // i-th entry is added value to descendant with distance i, for update k > 1

void read() {
  scanf("%d %d", &n, &q);
  for (int i = 2 ; i <= n ; i++) {
    scanf("%d", &parent[i]);
  }
}

void dfs(int now) {
  static int cntr = 0;
  tin[now] = ++cntr;
  tinListByDepth[depth[now]].push_back(tin[now]);

  for (int nex : chld[now]) {
    depth[nex] = 1 + depth[now];
    dfs(nex);
  }
  tout[now] = cntr;
}

void prepare() {
  for (int i = 2 ; i <= n ; i++) {
    chld[parent[i]].push_back(i);
  }

  dfs(1);

  updateOneInSubtree.init(n);
  updateOneFromAncestor.init(n);
  updateMoreThanOneInSubtree.init(n);
}

vector<int> fetchDescendantCountByDepth(int v) {
  vector<int> ret;

  for (int i = 0 ; i < LOG ; i++) {
    int d = depth[v] + i;
    auto up = upper_bound(tinListByDepth[d].begin(), tinListByDepth[d].end(), tout[v]);
    auto down = lower_bound(tinListByDepth[d].begin(), tinListByDepth[d].end(), tin[v]);
    ret.push_back((int)(up-down));
  }

  return ret;
}

void updateOne(int v, long long val) {
  updateOneInSubtree.update(tin[v], val * (tout[v] - tin[v] + 1));
  updateOneFromAncestor.update(tin[v]+1, val);
  updateOneFromAncestor.update(tout[v]+1, -val);
}

void updateMoreThanOne(int v, long long val, int k) {
  vector<int> descendantCountList = fetchDescendantCountByDepth(v);

  long long sum = 0;
  long long currentVal = val;
  for (int i = 0 ; i < LOG ; i++) {
    updateToDescendant[v][i] += currentVal;
    sum += currentVal * descendantCountList[i];
    currentVal /= k;
  }

  updateMoreThanOneInSubtree.update(tin[v], sum);
}

void update(int v, long long val, int k) {
  if (k == 1) updateOne(v, val);
  else        updateMoreThanOne(v, val, k);
}

long long calculate(int v) {
  long long ret = 0;

  // calculate from k = 1
  {
    ret += updateOneInSubtree.query(tin[v], tout[v]);
    long long updateFromAncestor = updateOneFromAncestor.query(1, tin[v]);
    ret += updateFromAncestor * (tout[v] - tin[v] + 1);
  }
  // end calculate from k = 1

  // calculate from k > 1
  {
    ret += updateMoreThanOneInSubtree.query(tin[v], tout[v]);

    vector<int> descendantCountList = fetchDescendantCountByDepth(v);
    for (int d = 1, cur = parent[v]; d < LOG ; d++, cur = parent[cur]) {
      for (int i = 0 ; i+d < LOG ; i++) {
        ret += updateToDescendant[cur][i+d] * descendantCountList[i];
      }
    }
  }
  // end calculate from k > 1

  return ret;
}

void work() {
  for (int i = 0 ; i < q ; i++) {
    int opt; scanf("%d", &opt);

    if (opt == 1) {
      int v, val, k;
      scanf("%d %d %d", &v, &val, &k);

      update(v, val, k);
    } else {
      int v; scanf("%d", &v);
      printf("%lld\n", calculate(v));
    }
  }
}

int main() {
  read();
  prepare();
  work();
  return 0;
}
