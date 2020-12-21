#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int maxn = 100005;

struct tnode { int node, weight; };
struct tdata { ll yes, no; };
bool operator < (const tdata &a, const tdata &b) {
  return a.yes-a.no < b.yes-b.no;
}

int N;
int root;
vector <tnode> con[maxn];
vector <tnode> chl[maxn];
int par[maxn];

void MakeRooted(int node, int parent) {
  par[node] = parent;
  for (auto &p : con[node])
    if (p.node != parent) {
      chl[node].push_back(p);
      MakeRooted(p.node, node);
    }
}

ll dp[maxn][2];

ll f(int curr, bool has_path=false) {
  if (chl[curr].empty()) {
    return 0;
  }
  if (dp[curr][has_path] != -1) {
    return dp[curr][has_path];
  }

  ll &ret = dp[curr][has_path] = 0;

  vector<tdata> data;
  for (auto &child : chl[curr]) {
    ll yes = f(child.node, true)+child.weight;
    ll no  = f(child.node, false);
    data.push_back((tdata){yes, no});
  }

  sort(data.rbegin(), data.rend());

  ll sum_no = 0;
  for (auto &d : data) {
    sum_no += d.no;
  }

  int n_child = (int)chl[curr].size();

  ret = sum_no + (has_path ? data[0].yes-data[0].no : 0);

  for (int i = has_path ? 1 : 0; i+1 < n_child; i += 2) {
    ll yes = data[i].yes+data[i+1].yes;
    ll no = data[i].no+data[i+1].no;
    if (yes < no) {
      break;
    }
    else {
      ret += yes - no;
    }
  }

  return ret;
}


int main(int argc, char *argv[]) {
  scanf("%d", &N) != EOF;
  for (int i = 0; i < N-1; ++i) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w) != EOF;
    con[u].push_back((tnode){v, w});
    con[v].push_back((tnode){u, w});
  }

  MakeRooted(1, 1);

  memset(dp, -1, sizeof(dp));

  ll ans = f(1, false);
  if (chl[1].size() == 1) {
    ans = max(ans, f(1, true));
  }

  printf("%lld\n", ans);

  return 0;
}
