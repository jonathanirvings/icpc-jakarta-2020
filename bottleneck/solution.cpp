#include <bits/stdc++.h>
using namespace std;

int main() {
  int N;
  scanf("%d", &N);

  bool all_ones = true;
  int suffix_zeroes = 0;
  for (int i = 0; i < N - 1; ++i) {
    int E;
    scanf("%d", &E);
    if (E == 0 && i > 0) {
      ++suffix_zeroes;
      all_ones = false;
    } else {
      suffix_zeroes = 0;
    }
  }

  if (suffix_zeroes == 0) {
    printf("%d\n", all_ones ? 1 : 2);
  } else {
    printf("%d\n", suffix_zeroes + 1);
  }
}
