#ifdef debug
#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>

using namespace std;

#define vec vector
#define ALL(x) (x).begin(), (x).end()

typedef unsigned int ui;
typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< long long, long long > pll;

const int inf = 1e9;
const ll inf64 = 1e18;

vec< pii > delt = {
    {-1, -2},
    {-2, -1},
    {+1, -2},
    {+2, -1},
    {-2, +1},
    {-1, +2},
    {+2, +1},
    {+1, +2}
};

const int N = 405 * 405;

mt19937 rnd(42);
int DELT_ITER;
int MAX_ITER = DELT_ITER;
int timer;
int mx = 0;
int cnt0;

int g[N][40];
int SZ[N];
int way[N];
int sz;
int deg[N];
int used[N];
int n;

inline int encode(int i, int j) {
    return i * n + j;
}

inline int encode(pii v) {
    return encode(v.first, v.second);
}

inline pii decode(int v) {
    return make_pair(v / n, v % n);
}

inline int dist(int v) {
    int i, j;
    tie(i, j) = decode(v);
    int res = min(i, j);
    res = min(res, min(n - i, n - j));
    return res;
}

void build_graph() {
    for(int v = 0;v < n * n;v++) {
        SZ[v] = 0;
    }
    for(int i = 0;i < n;i++) {
        for(int j = 0;j < n;j++) {
            for(pii d : delt) {
                int ti = i + d.first;
                int tj = j + d.second;
                if(ti < 0 || ti >= n || tj < 0 || tj >= n) {
                    continue;
                }
                int from = encode(i, j);
                int to = encode(ti, tj);
                g[from][SZ[from]++] = to;
            }
        }
    }
}

bool rec(int last) {
    used[last] = 1;
    way[sz++] = last;
    if(sz == n * n) {
        return true;
    }
    if(cnt0 > 0) {
        goto end_rec;
    }
    timer++;
    if(timer >= MAX_ITER) {
        goto end_rec;
    }
    shuffle(g[last], g[last] + SZ[last], rnd);
    sort(g[last], g[last] + SZ[last], [&](int v1, int v2){
        return used[v1] ? 0 : used[v2] ? 1 : deg[v1] < deg[v2] ? 1 : deg[v1] > deg[v2] ? 0 : dist(v1) < dist(v2);
    });
    for(int it = 0;it < SZ[last];it++) {
        int to = g[last][it];
        if(used[to]) break;
        if(--deg[to] == 0) {
            cnt0++;
        }
    }
    for(int it = 0;it < SZ[last];it++) {
        int to = g[last][it];
        if(used[to]) break;
        if(rec(to)) {
            return true;
        }
    }
    for(int it = 0;it < SZ[last];it++) {
        int to = g[last][it];
        if(used[to]) break;
        if(deg[to]++ == 0) {
            cnt0--;
        }
    }
    end_rec:;
    used[last] = 0;
    sz--;
    return false;
}

vec< pii > solve() {
    build_graph();

    MAX_ITER = 0;

    for(;;MAX_ITER += DELT_ITER) {
        sz = 0;
        for(int v = 0;v < n * n;v++) {
            deg[v] = SZ[v];
            used[v] = 0;
        }
        timer = 0;
        cnt0 = 0;
        if(rec(rnd() % (n * n))) {
            break;
        }
    }

    vec< pii > res;

    for(int i = 0;i < sz;i++) {
        res.push_back(decode(way[i]));
    }

    return res;
}

int get_type(pii from, pii to) {
    for(int i = 0;i < (int)delt.size();i++) {
        if(from.first + delt[i].first == to.first &&
            from.second + delt[i].second == to.second) {
            return i;
        }
    }
}

int main(int argc, char** argv) {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    if(argc < 2) {
        return 0;
    }

    n = atoi(argv[1]);

    DELT_ITER = n <= 392 ? 100000 : n == 395 ? 20000 : 50000;

    vec< pii > pway = solve();
    
    // for(pii x : pway) {
    //     cout << x.first << " " << x.second << "\n";
    // }

    cout << "start[" << n << "] = {" << pway[0].first << ", " << pway[0].second << "};\n";
    cout << "howto[" << n << "] = \"";
    for(int i = 1;i < (int)pway.size();i++) {
        cout << get_type(pway[i - 1], pway[i]);
    }
    cout << "\";\n";

    // cout << "\n";

//    for(n = 6;n <= 6;n++) {
//        cout << n << " : \n";
//        vec< pii > pway = solve();
//        for(pii x : pway) {
////            cout << x.first << " " << x.second << "\n";
//            cout << char(x.second + 'a') << (x.first + 1) << "\n";
//        }
//        cout << "-------------------\n";
//    }

//    cout << "\ntime : " << 1.0 * clock() / CLOCKS_PER_SEC << "\n";

    return 0;
}
