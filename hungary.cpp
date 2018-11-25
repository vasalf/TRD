#ifdef debug
#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>
#include "optimization.h"

using namespace std;

#define vec vector
#define ALL(x) begin(x), end(x)

typedef unsigned int ui;
typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< long long, long long > pll;

const int inf = 1e9;
const ll inf64 = 1e18;

pair< ll, vec< pii > > hungary_algorithm(vec< vec< int > > a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    int inv = 0;

    if(n > m) {
        inv = 1;
        swap(n, m);
        vec< vec< int > > b(n, vec< int >(m));
        for(int i = 0;i < n;i++) for(int j = 0;j < m;j++) {
            b[i][j] = a[j][i];
        }
        a = b;
    }

    vec< int > pair_i(n, -1), pair_j(m, -1);
    vec< ll > add_str(n), add_col(m);

    for(int v = 0;v < n;v++) {
        vec< int > used_i(n), used_j(m);
        vec< int > prev(m, -1);
        vec< pair< int, ll > > arg(m, make_pair(-1, LLONG_MAX));
        auto add_new_str = [&](int ni) -> void {
            for(int qj = 0;qj < m;qj++) {
                ll hlp = a[ni][qj] + add_str[ni] + add_col[qj];
                if(hlp < arg[qj].second) {
                    arg[qj] = {ni, hlp};
                }
            }
            used_i[ni] = 1;
        };
        add_new_str(v);
        while(1) {
            int i = -1, j;
            ll x = LLONG_MAX;
            for(int qj = 0;qj < m;qj++) {
                if(used_j[qj]) continue;
                int qi;
                ll tx;
                tie(qi, tx) = arg[qj];
                if(tx < x) x = tx, i = qi, j = qj;
            }
            if(i == -1) break;
            for(int qi = 0;qi < n;qi++) {
                if(used_i[qi]) continue;
                add_str[qi] += x;
            }
            for(int qj = 0;qj < m;qj++) {
                if(used_j[qj]) continue;
                add_col[qj] -= x;
                arg[qj].second -= x;
            }
            prev[j] = i;
            used_j[j] = 1;
            if(pair_j[j] == -1) {
                for(int u = j;u != -1;) {
                    int nu = pair_i[prev[u]];
                    pair_i[prev[u]] = u;
                    pair_j[u] = prev[u];
                    u = nu;
                }
                break;
            }
            add_new_str(pair_j[j]);
        }
    }

    ll total_sum = 0;
    vec< pii > pairs;

    for(int j, i = 0;i < n;i++) {
        if((j = pair_i[i]) == -1) continue;
        total_sum += a[i][j];
        if(inv) pairs.push_back({j, i});
        else pairs.push_back({i, j});
    }

    return {total_sum, pairs};
}

pair< ll, vec< pii > > slow(vec< vec< int > > a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    int inv = 0;

    if(n > m) {
        inv = 1;
        swap(n, m);
        vec< vec< int > > b(n, vec< int >(m));
        for(int i = 0;i < n;i++) for(int j = 0;j < m;j++) {
            b[i][j] = a[j][i];
        }
        a = b;
    }

    ll total_sum = LLONG_MAX;
    vec< pii > pairs;

    for(int mask = 0;mask < (1 << m);mask++) {
        if(__builtin_popcount(mask) != n) continue;
        vec< int > perm;
        for(int j = 0;j < m;j++) {
            if((1 << j) & mask) {
                perm.push_back(j);
            }
        }
        assert((int)perm.size() == n);
        do{
            ll tmp_cost = 0;
            vec< pii > tmp_pairs;
            for(int i = 0;i < n;i++) {
                tmp_cost += a[i][perm[i]];
                if(inv) tmp_pairs.push_back({perm[i], i});
                else tmp_pairs.push_back({i, perm[i]});
            }
            if(tmp_cost < total_sum) {
                total_sum = tmp_cost;
                pairs = tmp_pairs;
            }
        }while(next_permutation(ALL(perm)));
    }

    return {total_sum, pairs};
}

void test() {
    int T = 500;
    const int MAXN = 8;
    const int MAXX = 1000000;

    mt19937 rnd(42);

    int cnt_ok = 0;

    for(int test = 1;test <= T;test++) {
        cout << "test #" << test << ": ";
        int n = rnd() % MAXN + 1;
        int m = rnd() % MAXN + 1;
        vec< vec< int > > a(n, vec< int >(m));
        for(int i = 0;i < n;i++) for(int j = 0;j < m;j++) {
            a[i][j] = rnd() % MAXX;
        }
        ll exp = slow(a).first;
        ll fnd = hungary_algorithm(a).first;
        cout << "expected = " << exp << ", ";
        cout << "found = "<< fnd << " verdict = ";
        if(exp != fnd) {
            cout << "WA\n";
        }else {
            cout << "OK\n";
            cnt_ok++;
        }
    }
    cout << "OK = " << cnt_ok << "/" << T << "\n";
}

void solve() {
    int n;

    n = readInt();

    vec< vec< int > > a(n, vec< int >(n));

    for(int i = 0;i < n;i++) {
        for(int j = 0;j < n;j++) {
            a[i][j] = readInt();
        }
    }

    ll cost;
    vec< pii > pairs;

    tie(cost, pairs) = hungary_algorithm(a);

    writeInt< ll >(cost, '\n');
    for(pii p : pairs) {
        writeInt(p.first + 1, ' ');
        writeInt(p.second + 1, '\n');
    }
}

void max_test() {
    const int MAXN = 300;
    const int MAXX = 1e6;
    mt19937 rnd(42);
    int n, m;
    n = m = MAXN;
    vec< vec< int > > a(n, vec< int >(m));
    for(int i = 0;i < n;i++) for(int j = 0;j < m;j++) {
        a[i][j] = rnd() % (MAXX + 1);
    }
    ll exp;
    vec< pii > pairs;
    tie(exp, pairs) = hungary_algorithm(a);
    cout << "w = " << exp << ", sz = " << (int)pairs.size() << "\n";
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

   solve();
    // test();
    // max_test();

    return 0;
}
