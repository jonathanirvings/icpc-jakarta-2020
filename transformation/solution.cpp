#include <bits/stdc++.h>

using namespace std;

const int mod = 998244353;

int main() {
  int n;
  scanf("%d", &n);
  vector<int> p(n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", &p[i]);
    --p[i];
  }
  map<int, int> powers;
  int offset = 0;
  for (int i = 0; i < n; ++i) {
    if (p[i] < 0) {
      continue;
    }
    int cycle = 0;
    int now = i;
    while (p[now] >= 0) {
      ++cycle;
      int nxt = p[now];
      p[now] = -1;
      now = nxt;
    }
    int pw2 = 0;
    while ((cycle % 2) == 0) {
      ++pw2;
      cycle /= 2;
    }
    offset = max(offset, pw2);
    if (cycle <= 1) continue;
    int period = 1, cur = 2;
    while (cur != 1) {
      cur = cur * 2 % cycle;
      ++period;
    }
    for (int p = 2; p * p <= period; ++p) {
      if (period % p) continue;
      int cnt = 0;
      while ((period % p) == 0) {
        ++cnt;
        period /= p;
      }
      powers[p] = max(powers[p], cnt);
    }
    if (period > 1) {
      powers[period] = max(powers[period], 1);
    }
  }
  int answer = 1;
  for (auto f : powers) {
    while (f.second-- > 0)
      answer = 1LL * answer * f.first % mod;
  }
  answer = (answer + offset) % mod;
  if (answer < 0)
    answer += mod;
  printf("%d\n", answer);
  return 0;
}
