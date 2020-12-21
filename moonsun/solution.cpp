#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

const int maxn = 100005;
const int mod = 235813;
const int lrange = -100000;
const int rrange = 100000;

int power(int a, int b, int mod) {
  if (b == 0) return 1;
  int ret = power(a, b>>1, mod);
  ret = (ll)ret * ret % mod;
  if (b & 1) ret = (ll)ret * a % mod;
  return ret;
}
int inverse(int a, int m) {
  return power(a, m-2, m);
}

int posmod(int x) { return (x % mod + mod) % mod; }
int negmod(int x) { return (x % mod - mod) % mod; }


int N;
int A[maxn];
int val[maxn];

int main(int argc, char *argv[]) {
  scanf("%d", &N) != EOF;
  for (int i = 0; i < N; ++i)
    scanf("%d", &A[i]) != EOF;

  ll  p = 1;
  int a = N-1, b = 1;
  int sign = 1;
  for (int i = N-1; i >= 0; --i) {
    val[i] = sign * p * A[i] % mod;
    p = p * a % mod;
    p = p * inverse(b, mod) % mod;
    sign = -sign, --a, ++b;
  }

  int total = 0;
  for (int i = 0; i < N; ++i)
    total = (total + val[i]) % mod;

  if (total == 0) {
    printf("%d\n", 0);
    return 0;
  }

  int ans = 0;

  sign = 1, p = 1, a = N-1, b = 1;
  for (int i = N-1; i >= 0; --i) {
    int target = -(total - val[i]) % mod;
    int out = (ll)target * inverse(p, mod) % mod;
    out *= sign;

    bool found = false;
  
    int neg = negmod(out);
    if (lrange <= neg && neg <= rrange && neg != A[i]) {
      found = true;
    }

    int pos = posmod(out);
    if (lrange <= pos && pos <= rrange && pos != A[i]) {
      found = true;
    }

    if (found) {
      ++ans;
    }

    p = p * a % mod;
    p = p * inverse(b, mod) % mod;
    sign = -sign, --a, ++b;
  }

  printf("%d\n", ans);

  return 0;
}
