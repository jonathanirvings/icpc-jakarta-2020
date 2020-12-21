#include <bits/stdc++.h>
using namespace std;

int N, M;
int A[1005], B[1005];
int dp[1005][10005];
int prefixA[1005];

int main() {
  scanf("%d", &N);
  M = 10*N;

  for (int i = 1; i <= N; i++) {
    scanf("%d", &A[i]);
    prefixA[i] = A[i] + prefixA[i-1];
  }

  for (int i = 1; i <= N; i++) {
    scanf("%d", &B[i]);
  }

  for (int i = 1; i <= N+1; i++) {
    for (int j = 0; j <= M; j++) {
      dp[i][j] = -1;
    }
  }

  dp[1][0] = 0;
  for (int i = 1; i <= N+1; i++) {
    for (int j = 0; j <= M; j++) {
      if (dp[i][j] == -1) {
        continue;
      }

      // read i th book
      if (i <= N && j+B[i] <= M) {
        dp[i+1][j+B[i]] = max(dp[i+1][j+B[i]], min(dp[i][j]+B[i], prefixA[i-1]));
      }

      // read i+1 th book
      if (i>1 && i+1 <= N && j+B[i+1] <= M) {
        dp[i+2][j+B[i+1]] = max(dp[i+2][j+B[i+1]], min(dp[i][j]+B[i+1], prefixA[i]));
      }
    }
  }

  int res = 2000000000;
  for (int j = 0; j <= M; j++) {
    if (dp[N+1][j] != -1) {
      res = min(res, j + prefixA[N] - dp[N+1][j]);
    }
  }

  printf("%d\n", res);
}
