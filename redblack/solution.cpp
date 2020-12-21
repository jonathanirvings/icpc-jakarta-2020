#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int maxn = 205;
const int inf = 1000000005;
const int mod = 998244353;

struct tdata { int pos, label; };
bool operator < (const tdata &a, const tdata &b) { return a.pos < b.pos; };

int N, M;
tdata p[maxn];
int fac[maxn] = {1};
int nck[maxn][maxn] = {1};
vector<pair<int, int>> segment;
vector<int> _size;

int num[maxn][maxn];

int labelL, labelR;
int dp_f[maxn][maxn][maxn];
int f(int L, int R, int k) {
  if (L+1 == R) return k == 0;
  if (dp_f[L][R][k] != -1) return dp_f[L][R][k];
  int &ret = dp_f[L][R][k] = 0;
  int n = R-L-1;
  for (int i = L+1; i < R; ++i) {
    int label = (p[i].pos-p[L].pos <= p[R].pos-p[i].pos) ? labelL : labelR;
    int n_fixed = 0;
    if (label == labelL) n_fixed += i-L-1;
    if (label == labelR) n_fixed += R-i-1;
    int n_var = n-n_fixed-1;
    int n_get = (label == 0) ? n_fixed+1 : 0;
    int n_var_way = 0;
    if (k >= n_get) {
      if (label != labelL) n_var_way = f(L, i, k-n_get);
      if (label != labelR) n_var_way = f(i, R, k-n_get);
    }
    int n_fixed_way = fac[n_fixed];
    int n_combin = nck[n-1][n_fixed];
    ret = (ret + (ll)n_combin*n_fixed_way%mod*n_var_way%mod) % mod;
  }

  return ret;
}

int dp_g[maxn][maxn];
int g(int x, int k) {
  if (x == (int)_size.size()) return k == 0;
  if (dp_g[x][k] != -1) return dp_g[x][k];
  int &ret = dp_g[x][k] = 0;
  for (int i = 0; i <= min(_size[x], k); ++i) {
    ret = (ret + (ll)num[x][i] * g(x+1, k-i) % mod) % mod;
  }
  return ret;
}


int main(int argc, char *argv[]) {
  for (int i = 1; i < maxn; ++i)
    fac[i] = (ll)fac[i-1] * i % mod;
  for (int i = 1; i < maxn; ++i) {
    nck[i][0] = nck[i][i] = 1;
    for (int j = 1; j < i; ++j)
      nck[i][j] = (nck[i-1][j] + nck[i-1][j-1]) % mod;
  }

  scanf("%d %d", &N, &M) != EOF;
  for (int i = 1; i <= N; ++i) {
    char s[10];
    scanf("%d %s", &p[i].pos, s) != EOF;
    p[i].label = (s[0] == 'R') ? 0 : 1;
  }
  for (int i = N+1; i <= N+M; ++i) {
    scanf("%d", &p[i].pos) != EOF;
    p[i].label = -1;
  }

  p[0] = (tdata){-inf, -1};
  p[N+M+1] = (tdata){inf, -1};
  
  sort(p, p+N+M+2);

  int n_all = N+M+2;

  for (int i = 1; i < n_all; ++i)
    if (p[i].label != -1) {
      p[0].label = p[i].label;
      break;
    }
  for (int i = n_all-2; i >= 0; --i)
    if (p[i].label != -1) {
      p[N+M+1].label = p[i].label;
      break;
    }

  int n_fixed = 0;
  int n_var = 0;
  int n_a = 0;
  int n_b = 0;

  int prev = -2;
  for (int i = 0; i < n_all; ++i) {
    if (p[i].label != -1) {
      if (prev != -2) {
        if (p[prev].label == p[i].label) {
          if (p[i].label == 0) n_a += i-prev-1;
          if (p[i].label == 1) n_b += i-prev-1;
          n_fixed += i-prev-1;
        }
        else if (i-prev-1 > 0) {
          segment.push_back(make_pair(prev, i));
          n_var += i-prev-1;
        }
      }
      prev = i;
    }
    if (0 < i && i < n_all-1 && p[i].label == 0) ++n_a;
    if (0 < i && i < n_all-1 && p[i].label == 1) ++n_b;
  }

  for (int i = 0; i < (int)segment.size(); ++i)
    _size.push_back(segment[i].second-segment[i].first-1);

  memset(dp_f, -1, sizeof(dp_f));
  for (int i = 0; i < (int)segment.size(); ++i)
    for (int k = 0; k <= segment[i].second-segment[i].first-1; ++k) {
      labelL = p[segment[i].first].label;
      labelR = p[segment[i].second].label;
      num[i][k] = f(segment[i].first, segment[i].second, k);
    }
  
  int n_way_inter = 1;
  int m_var = n_var;
  for (int i = 0; i < (int)_size.size(); ++i) {
    n_way_inter = (ll)n_way_inter * nck[m_var][_size[i]] % mod;
    m_var -= _size[i];
  }

  int n_way_fixed = (ll)fac[n_fixed] * nck[M][n_fixed] % mod;

  int ans = 0;
  memset(dp_g, -1, sizeof(dp_g));
  for (int k = max(n_a, (N+M)/2+1); k <= N+M; ++k) {
    ans = (ans + g(0, k-n_a)) % mod;
  }
  
  ans = (ll)ans * n_way_inter % mod * n_way_fixed % mod;

  printf("%d\n", ans);

  return 0;
}
