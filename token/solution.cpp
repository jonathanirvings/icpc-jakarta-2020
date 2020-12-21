#include <bits/stdc++.h>
using namespace std;

template <int (*Merge)(int, int), int Identity>
class SegTree {
 public:
  void Build(const vector<int>& v) {
    N = v.size();
    tree.resize(8 * N);
    Build(0, 0, N - 1, v);
  }

  void Update(int pos, int val) {
    Update(0, 0, N - 1, pos, val);
  }

  int Query(int x, int y) {
    return Query(0, 0, N - 1, x, y);
  }

 private:
  void Build(int ix, int L, int R, const vector<int>& v) {
    if (L == R) {
      tree[ix] = v[L];
      return;
    }
    int M = (L + R) >> 1;
    Build(ix * 2 + 1, L, M, v);
    Build(ix * 2 + 2, M + 1, R, v);
    tree[ix] = Merge(tree[ix * 2 + 1], tree[ix * 2 + 2]);
  }

  void Update(int ix, int L, int R, int pos, int val) {
    if (L == R) {
      tree[ix] = val;
      return;
    }
    int M = (L + R) >> 1;
    if (pos <= M) {
      Update(ix * 2 + 1, L, M, pos, val);
    } else {
      Update(ix * 2 + 2, M + 1, R, pos, val);
    }
    tree[ix] = Merge(tree[ix * 2 + 1], tree[ix * 2 + 2]);
  }

  int Query(int ix, int L, int R, int x, int y) {
    if (x <= L && R <= y) {
      return tree[ix];
    }
    if (R < x || y < L) {
      return Identity;
    }
    int M = (L + R) >> 1;
    return Merge(Query(ix * 2 + 1, L, M, x, y),
                 Query(ix * 2 + 2, M + 1, R, x, y));
  }

  vector<int> tree;
  int N;
};

int Min(int a, int b) {
  return min(a, b);
}

int Max(int a, int b) {
  return max(a, b);
}

int Gcd(int a, int b) {
  return (a == 0 && b == 0) ? 0 : __gcd(a, b);
}

typedef SegTree<Min, INT_MAX> MinSegTree;
typedef SegTree<Max, INT_MIN> MaxSegTree;
typedef SegTree<Gcd, 0> GcdSegTree;

const string kYes = "YES";
const string kNo = "NO";

int main() {
  int N, Q;
  scanf("%d %d", &N, &Q);
  
  vector<int> T(N);
  vector<int> D(N - 1);
  vector<int> leftEqual(N);
  map<int, set<int>> position;

  for (int i = 0; i < N; ++i) {
    scanf("%d", &T[i]);
    if (i > 0) {
      D[i - 1] = abs(T[i] - T[i - 1]);
    }
    if (position.count(T[i]) == 0 || position[T[i]].empty()) {
      leftEqual[i] = INT_MIN;
    } else {
      leftEqual[i] = *(position[T[i]].rbegin());
    }
    position[T[i]].insert(i);
  }

  MinSegTree minT;
  MaxSegTree maxT, maxLeftEqual;
  GcdSegTree gcdD;

  minT.Build(T);
  maxT.Build(T);
  maxLeftEqual.Build(leftEqual);
  gcdD.Build(D);

  while (Q--) {
    int op;
    scanf("%d", &op);
    if (op == 1) {
      int X, Y;
      scanf("%d %d", &X, &Y);
      --X;

      {
        set<int>& oldPos = position[T[X]];
        set<int>::iterator it = oldPos.upper_bound(X);
        if (it != oldPos.end()) {
          int toChange = *it;
          --it;
          leftEqual[toChange] = it == oldPos.begin() ? INT_MIN : *prev(it);
          maxLeftEqual.Update(toChange, leftEqual[toChange]);
        }
        oldPos.erase(X);
      }

      T[X] = Y;
      minT.Update(X, T[X]);
      maxT.Update(X, T[X]);

      {
        set<int>& newPos = position[T[X]];
        set<int>::iterator it = newPos.upper_bound(X);
        if (it != newPos.end()) {
          int toChange = *it;
          leftEqual[toChange] = X;
          maxLeftEqual.Update(toChange, leftEqual[toChange]);  
        }
        leftEqual[X] = it == newPos.begin() ? INT_MIN : *prev(it);
        maxLeftEqual.Update(X, leftEqual[X]);
        newPos.insert(X);
      }
      
      if (X > 0) {
        D[X - 1] = abs(T[X] - T[X - 1]);
        gcdD.Update(X - 1, D[X - 1]);
      }
      if (X + 1 < N) {
        D[X] = abs(T[X + 1] - T[X]);
        gcdD.Update(X, D[X]);
      }
    } else {
      int L, R;
      scanf("%d %d", &L, &R);
      --L; --R;
      
      int delta = maxT.Query(L, R) - minT.Query(L, R);
      int cnt = R - L;
      if (delta == 0) {
        printf("%s\n", kYes.c_str());
      } else if (delta % cnt != 0) {
        printf("%s\n", kNo.c_str());
      } else if (maxLeftEqual.Query(L, R) >= L) {
        printf("%s\n", kNo.c_str());
      } else if (gcdD.Query(L, R - 1) == (delta / cnt)) {
        printf("%s\n", kYes.c_str());
      } else {
        printf("%s\n", kNo.c_str());
      }
    }
  }
}
