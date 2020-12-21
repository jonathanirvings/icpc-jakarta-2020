#include<bits/stdc++.h>
#define F first
#define S second
#define pb push_back
using namespace std;

typedef long long ll;
typedef pair<int, int> ii;

const int N = 2e5 + 5;

vector<int> changes;
vector<int> sorted_changes;

int mx;
int bit[N];

void upd(int x){
    for(;x <= mx; x += x & (-x))bit[x]++;
}

int get(int x){
    int res = 0;
    for(;x > 0; x -= x & (-x)) res += bit[x];
    return res;
}

ll get_inversion(vector<int> &wallet){
    vector<ii> temp;
    mx = wallet.size();
    for(int i = 1; i <= mx; ++i){
        bit[i] = 0;
        // sort by first ascending, second descending
        temp.pb({-wallet[i - 1], -i});
    }

    sort(temp.begin(), temp.end());

    ll res = 0;
    for(int i = 0; i < mx; ++i){
        res += get(-temp[i].second);
        upd(-temp[i].second);
    }

    return res;
}

int get_lesser_count(int x){
    return lower_bound(sorted_changes.begin(),sorted_changes.end(), x) - sorted_changes.begin();
}

int get_bigger_count(int x){
    return sorted_changes.end() - upper_bound(sorted_changes.begin(),sorted_changes.end(), x);
}

int main(){
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int n, m; cin >> n >> m;
    for(int i = 0; i < n; ++i){
        int x; cin >> x;
        changes.pb(x);
        sorted_changes.pb(x);
    }

    sort(sorted_changes.begin(), sorted_changes.end());
    ll change_inversion = get_inversion(changes);

    while(m--){
        int k; cin >> k;

        vector<int> wallet;

        ll cur_inv = 0;
        while(k--){
            int x; cin >> x;
            wallet.pb(x);

            cur_inv += get_bigger_count(x);
        }

        ll best = cur_inv;
        for(int i = 0; i < wallet.size(); ++i){
            cur_inv -= get_bigger_count(wallet[i]);
            cur_inv += get_lesser_count(wallet[i]);
            best = min(best, cur_inv);
        }

        cout << best + get_inversion(wallet) + change_inversion << endl;
    }


}

/*
Input:
3 3
5 6 7
6 2 3 4 8 9 10 
2 100 99 
3 5 6 7

Output:
0 
1 
1

Input:
3 2
7 6 5
6 2 3 4 8 9 10
6 10 9 8 4 3 2

Output:
3
27

*/