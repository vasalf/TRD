void xor_fwht(vector<int>& a, bool inverse = false) {
    for (int x, y, len = 1; len < (int) a.size(); len <<= 1) {
        for (int i = 0; i < (int) a.size(); i += len << 1) {
            for (int j = 0; j < len; j++) {
                x = a[i + j], y = a[i + j + len];
                a[i + j] = _sum(x, y), a[i + j + len] = _sub(x, y);
            }
        }
    }
    if (inverse) {
        int rn = _binpow((int) a.size(), mod - 2);
        for (int& x : a)
            x = _mul(x, rn);
    }
}

void or_fwht(vector<int>& a, bool inverse = false) {
    for (int x, y, len = 1; len < (int) a.size(); len <<= 1) {
        for (int i = 0; i < (int) a.size(); i += len << 1) {
            for (int j = 0; j < len; j++) {
                x = a[i + j], y = a[i + j + len];
                a[i + j] = x, a[i + j + len] = inverse ? _sub(y, x) : _sum(y, x);
            }
        }
    }
}

void and_fwht(vector<int>& a, bool inverse = false) {
    for (int x, y, len = 1; len < (int) a.size(); len <<= 1) {
        for (int i = 0; i < (int) a.size(); i += len << 1) {
            for (int j = 0; j < len; j++) {
                x = a[i + j], y = a[i + j + len];
                a[i + j] = inverse ? _sub(x, y) : _sum(x, y), a[i + j + len] = y;
            }
        }
    }
}

// ================

// https://codeforces.com/contest/1218/submission/61445132

#include <bits/stdc++.h>
 
using namespace std;
 
typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef long long ll;
typedef unsigned int ui;
typedef unsigned long long ull;
typedef long double ld;
 
const int inf = 1e9;
const ll inf64 = 1e18;
 
inline int _sum(int a, int b, int mod) {
    return a + b < mod ? a + b : a + b - mod;
}
 
inline int _mul(int a, int b, int mod) {
    return (1ll * a * b) % mod;
}
 
inline int _sub(int a, int b, int mod) {
    return a >= b ? a - b : a - b + mod;
}
 
inline void _inc(int &x, int y, int mod) {
    if ((x += y) >= mod) {
        x -= mod;
    }
}
 
inline void _dec(int &x, int y, int mod) {
    if ((x -= y) < 0) {
        x += mod;
    }
}
 
inline int _binpow(int x, int pw, int mod) {
    int res = 1;
    int tmp = x;
    while (pw > 0) {
        if (pw & 1) {
            res = _mul(res, tmp, mod);
        }
        tmp = _mul(tmp, tmp, mod);
        pw >>= 1;
    }
    return res;
}
 
inline int _div(int a, int b, int mod) {
    return _mul(a, _binpow(b, mod - 2, mod), mod);
}
 
int r2;
 
const int N = 1e5 + 100;
 
int n, m;
vector<pii> g[N];
unordered_set<ll> br;
 
void add_bridge(int u, int v) {
    if (u > v) swap(u, v);
    br.insert(1ll * (u - 1) * n + v);
}
 
bool is_bridge(int u, int v) {
    if (u > v) swap(u, v);
    return br.count(1ll * (u - 1) * n + v);
}
 
int fup[N];
int tin[N], timer;
int used[N];
int xr[N];
vector<int> comp[N];
 
void dfs(int v, int from = -1) {
    used[v] = 1;
    tin[v] = fup[v] = timer++;
    for (pii ed : g[v]) {
        int to = ed.first;
        if (to == from) continue;
        if (used[to]) {
            fup[v] = min(fup[v], tin[to]);
        } else {
            dfs(to, v);
            fup[v] = min(fup[v], fup[to]);
            if (fup[to] > tin[v]) {
                add_bridge(v, to);
            }
        }
    }
}
 
int color[N];
 
void go(int v, int cl) {
    color[v] = cl;
    for (pii ed : g[v]) {
        int to = ed.first;
        if (is_bridge(v, to)) continue;
        if (!color[to]) {
            go(to, cl);
        }
    }
}
 
const int LOG = 17;
 
void fwht(ll* a, ll mod) {
    ll f1, f2;
    for (int len = 1; len < (1 << LOG); len <<= 1) {
        for (int i = 0; i < (1 << LOG); i += 2 * len) {
            for (int j = 0; j < len; j++) {
                f1 = a[i + j] + a[i + j + len];
                f2 = a[i + j] - a[i + j + len];
                if (f1 >= mod) f1 -= mod;
                if (f2 < 0) f2 += mod;
                a[i + j] = f1;
                a[i + j + len] = f2;
            }
        }
    }
}
 
int main() {
 
#ifdef debug
    freopen("input.txt", "r", stdin);
#endif
 
    ios_base::sync_with_stdio(0);
    cin.tie(0);
 
    cin >> n >> m;
 
    for (int w, u, v, i = 0; i < m; i++) {
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
 
    dfs(1);
 
    int cl = 0;
    for (int v = 1; v <= n; v++) {
        if (color[v]) continue;
        cl++;
        go(v, cl);
    }
 
    vector<vector<int>> vals;
 
    ll cur_dp[1 << LOG] {0};
    ll cur_can[1 << LOG] {0};
    ll tmp_dp[1 << LOG];
    ll tmp_can[1 << LOG];
 
    cur_dp[0] = 1;
    cur_can[0] = 1;
 
    int tot = 0;
 
    for (int v = 1; v <= n; v++) {
        for (pii ed : g[v]) {
            int to = ed.first;
            if (v > to) continue;
            int we = ed.second;
            if (is_bridge(v, to)) {
                tot ^= we;
                continue;
            }
            xr[color[v]] ^= we;
            comp[color[v]].push_back(we);
        }
    }
 
    for (int c = 1; c <= cl; c++) {
        vector<int> add;
        for (int we : comp[c]) {
            add.push_back(xr[c] ^ we);
        }
        if (!add.empty())
            vals.push_back(add);
    }
 
    const ll mod1 = inf + 7;
    const ll mod2 = inf + 21;
 
    fwht(cur_dp, mod1);
    fwht(cur_can, mod2);
 
    for (const auto& ar : vals) {
        memset(tmp_dp, 0, sizeof(ll) * (1 << LOG));
        memset(tmp_can, 0, sizeof(ll) * (1 << LOG));
        for (int x : ar) {
            tmp_dp[x]++;
            tmp_can[x]++;
        }
        fwht(tmp_dp, mod1);
        fwht(tmp_can, mod2);
        for (int i = 0; i < (1 << LOG); i++) {
            cur_dp[i] = (cur_dp[i] * tmp_dp[i]) % mod1;
            cur_can[i] = (cur_can[i] * tmp_can[i]) % mod2;
        }
    }
 
    fwht(cur_dp, mod1);
    fwht(cur_can, mod2);
 
    r2 = _binpow(1 << LOG, mod1 - 2, mod1);
 
    int best = -1;
 
    for (int i = 0; i < (1 << LOG); i++) {
        if (cur_dp[i] || cur_can[i]) {
            if (best == -1 || (best > (i ^ tot))) {
                best = i ^ tot;
            }
        }
    }
 
    cout << best << " " << _mul(r2, cur_dp[best ^ tot], mod1) << "\n";
 
    return 0;
}
