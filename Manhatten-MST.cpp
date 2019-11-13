#ifdef debug
#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>

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

const int N = 250 * 1000 + 5;

struct FEN {
    int n = 0;
    vec< pii > f;

    FEN(int _n) : n(_n) {
        f.resize(n, make_pair(inf, inf));
    }

    void upd(int pos, pii val) {
        for(int cur = pos;cur < n;cur |= (cur + 1)) {
            if(f[cur] > val) f[cur] = val;
        }
    }

    pii get(int pos) {
        pii res = make_pair(inf, inf);
        for(int cur = pos;cur >= 0;cur = (cur & (cur + 1)) - 1) {
            if(res > f[cur]) res = f[cur];
        }
        return res;
    }
};

struct pt {
    int x, y, id;
    bool operator<(const pt &o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
} p[N];

int n, sz;
pii edge[N * 8];
int x0[N];
int Y0[N];

inline int get_dist(pii e) {
    return abs(x0[e.first] - x0[e.second]) + abs(Y0[e.first] - Y0[e.second]);
}

void rotate() {
    for(int i = 0;i < n;i++) {
        int x1 = p[i].x - p[i].y;
        int y1 = p[i].x + p[i].y;
        p[i].x = x1;
        p[i].y = y1;
    }
}

int par[N];
int Size[N];

int find(int x) { return x == par[x] ? x : par[x] = find(par[x]); }

void add_edges() {
    sort(p, p + n, [](const pt &p1, const pt &p2){
        return p1.x - p1.y < p2.x - p2.y;
    });
    vec< int > ys;
    for(int i = 0;i < n;i++) ys.push_back(p[i].y);
    sort(ALL(ys));
    ys.resize(unique(ALL(ys)) - ys.begin());
    FEN fen((int)ys.size());
    for(int i = 0;i < n;i++) {
        int j = lower_bound(ALL(ys), p[i].y) - ys.begin();
        pii tmp = fen.get(j);
        if(tmp.second != inf) {
            edge[sz++] = {tmp.second, p[i].id};
        }
        fen.upd(j, {-p[i].x - p[i].y, p[i].id});
    }
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    scanf("%d", &n);

    for(int i = 0;i < n;i++) {
        scanf("%d %d", &p[i].x, &p[i].y);
        p[i].id = i;
        x0[i] = p[i].x;
        Y0[i] = p[i].y;
    }

    for(int iter = 0;iter < 2;iter++) {
        for(int it = 0;it < 4;it++) {
            for(int i = 0;i < n;i++) {
                p[i].id = i;
                p[i].x = (it & (1 << 0)) ? x0[i] : -x0[i];
                p[i].y = (it & (1 << 1)) ? Y0[i] : -Y0[i];
            }
            add_edges();
        }
        for(int i = 0;i < n;i++) {
            swap(x0[i], Y0[i]);
        }
    }

    sort(edge, edge + sz, [&](pii e1, pii e2){
        return get_dist(e1) < get_dist(e2);
    });

    ll res = 0;

    for(int i = 0;i < n;i++) {
        Size[i] = 1;
        par[i] = i;
    }

    for(int iter = 0;iter < sz;iter++) {
        int u, v;
        tie(u, v) = edge[iter];
        int we = get_dist({u, v});
        u = find(u);
        v = find(v);
        if(u == v) continue;
//        cout << u << " " << v << " " << we << " " << Size[u] << " " << Size[v] << "\n";
        res += 1ll * Size[u] * Size[v] * (we >> 1);
        if(Size[u] < Size[v]) swap(u, v);
        par[v] = u;
        Size[u] += Size[v];
    }

    printf("%lld\n", res);

    return 0;
}
