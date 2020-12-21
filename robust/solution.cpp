// O(M^2 * (N+M) + M^2 log(M) + M^3)

#include <bits/stdc++.h>

using namespace std;

typedef long long LD;

inline bool eq(LD x, LD y) { return x == y; }
inline bool le(LD x, LD y) { return x <= y; }
inline bool lt(LD x, LD y) { return x < y; }
inline int sign(LD x) { return eq(x, 0) ? 0 : (x < 0 ? -1 : 1); }

struct point {
  LD x, y;
  point(LD _x = 0, LD _y = 0) : x(_x), y(_y) {}
  point operator+(point p) { return point(x+p.x, y+p.y); }
  point operator-(point p) { return point(x-p.x, y-p.y); }
  point operator*(LD s) { return point(x * s, y * s); }
  point operator/(LD s) { return point(x / s, y / s); }
  LD operator*(point p) { return x * p.x + y * p.y; }
  LD operator%(point p) { return x * p.y - y * p.x; }
  LD norm2() { return *this * *this; }
  bool operator<(point p) const { return eq(y, p.y) ? x < p.x : y < p.y; }
  bool operator==(point p) { return eq(x, p.x) && eq(y, p.y); }
};

ostream& operator<<(ostream & os, point p) {
  return os << "(" << (long double)p.x << ", " << (long double)p.y << ")";
}

// 1 : ccw, -1 : cw, 0 : colinear
int ccw(point a, point b, point c) {
  return sign((b-a) % (c-b));
}

bool on_segment(point a, point mid, point b) {
  return ccw(a, mid, b) == 0 && (a - mid) * (b - mid) <= 0;
}

const int mod = 998244353;

long long powmod(long long b, long long p) {
  long long r = 1;
  for (; p; p >>= 1, b = b * b % mod)
    if (p & 1)
      r = r * b % mod;
  return r;
}

vector<vector<int>> trans, dp;

int main() {
  int n, m, success;
  scanf("%d %d %d", &n, &m, &success);
  success = success * powmod(100, mod - 2) % mod;
  int fail = (1 + mod - success) % mod;
  vector<point> a(n), b(m);
  for (int i = 0; i < n; ++i) {
    int x, y;
    scanf("%d %d", &x, &y);
    a[i] = point(x, y);
  }
  for (int i = 0; i < m; ++i) {
    int x, y;
    scanf("%d %d", &x, &y);
    b[i] = point(x, y);
  }
  point pivot = a[0];
  trans.assign(m, vector<int>(m, 0));
  dp.assign(m, vector<int>(m, 0));
  vector<pair<int, int>> valid_trans;
  sort(b.begin(), b.end(), [&](point le, point ri) {
    // sort by angle
    bool isupper_le = pivot < le, isupper_ri = pivot < ri;
    if (isupper_le != isupper_ri)
      return isupper_le;
    return ccw(pivot, le, ri) > 0;
  });
  for (int i = 0; i < m; ++i) {
    point from = b[i];
    for (int j = 0; j < m; ++j) {
      if (j == i) continue;
      point to = b[j];
      bool valid = true;
      for (auto & p : a) {
        int res_ccw = ccw(from, to, p);
        if (res_ccw < 0) {
          valid = false;
          break;
        } else if (res_ccw == 0) {
          if (!on_segment(from, p, to)) {
            valid = false;
            break;
          }
        }
      }
      if (!valid)
        continue;
      valid_trans.emplace_back(i, j);
      trans[i][j] = success;
      for (int k = 0; k < m; ++k) {
        if (k == i || k == j) continue;
        point p = b[k];
        bool deactivate = false;
        deactivate |= on_segment(pivot, to, p);
        deactivate |= ccw(from, to, p) < 0 && ccw(pivot, to, p) < 0 && ccw(pivot, from, p) > 0;
        if (deactivate) {
          trans[i][j] = (1LL * trans[i][j] * fail) % mod;
        }
      }
    }
  }
  sort(valid_trans.begin(), valid_trans.end(), [&](pair<int, int> & le, pair<int, int> & ri) {
    // sort by (angle, relative position)
    point vle = b[le.second] - b[le.first];
    point vri = b[ri.second] - b[ri.first];
    bool isupper_le = point() < vle, isupper_ri = point() < vri;
    if (isupper_le != isupper_ri)
      return isupper_le;
    long long cross = vle % vri;
    if (cross != 0)
      return cross > 0;
    assert(vle * vri > 0);
    if (b[le.second] == b[ri.second]) {
      assert(!(b[le.first] == b[ri.first]));
      return (b[ri.first] - b[le.first]) * vle < 0; 
    }
    return (b[ri.second] - b[le.second]) * vle < 0;
  });
  for (int i = 0; i < m; ++i)
    dp[i][i] = 1;
  for (auto & e : valid_trans) {
    int from = e.first, to = e.second;
    long long val = trans[e.first][e.second];
    for (int start = 0; start < m; ++start) {
      dp[start][to] = (dp[start][to] + dp[start][from] * val) % mod;
    }
  }
  long long ans = 0;
  for (int i = 0; i < m; ++i) {
    ans = (ans + dp[i][i] - 1) % mod;
  }
  if (ans < 0)
    ans += mod;
  printf("%lld\n", ans);
  return 0;
}
