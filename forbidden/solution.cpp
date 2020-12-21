#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
const int M = 1e6 + 5;

int pset[M];
int who[M];

int a[N], b[N];
int n, m;

int ans[N];

int finds(int x) {
  return pset[x] == x ? x : pset[x] = finds(pset[x]);
}

void join(int x, int y) {
  x = finds(x);
  y = finds(y);
  pset[y] = x;
}

void read() {
  scanf("%d %d", &n, &m);
  for (int i = 0 ; i < n ; i++) {
    scanf("%d %d", &a[i], &b[i]);
  }
}

void work() {
  for (int i = 1 ; i <= m ; i++) {
    who[i] = -1;
    pset[i] = i;
  }

  int loserWithoutForbid = -1;
  for (int i = 0 ; ; i = (i + 1) % n) {
    if (who[a[i]] == -1) {
      who[a[i]] = i;
      if (who[b[i]] == -1) {
        join(b[i], a[i]);
      }
    } else if (who[b[i]] == -1) {
      who[b[i]] = i;
    } else {
      loserWithoutForbid = i;
      break;
    }
  }

  for (int i = 1 ; i <= m ; i++) {
    int root = finds(i);
    if (who[root] == -1) {
      ans[loserWithoutForbid]++;
    } else {
      ans[who[root]]++;
    }
  }

  for (int i = 0 ; i < n ; i++) {
    printf("%d\n", ans[i]);
  }
}

int main() {
  read();
  work();
  return 0;
}
