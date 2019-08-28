
#ifdef debug
#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>
#include "optimization.h"

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

typedef
    tree<
        pair< int, int >,
        null_type,
        less< pair< int, int > >,
        rb_tree_tag,
        tree_order_statistics_node_update
    > stat_set;

#define vec vector
#define ALL(x) begin(x), end(x)

#define pb push_back

typedef unsigned int ui;
typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< long long, long long > pll;

const int inf = 1e9;
const ll inf64 = 1e18;

struct pt {
    int x = 0, y = 0, r = 0, t = 0;
    void read() {
        t = readInt();
        x = readInt();
        y = readInt();
        if(t == 2) r = readInt();
    }
};

int m;
vec< int > xs, ys, xys;

vec< stat_set > tri;
vec< stat_set > rec;

void add(int v, int tl, int tr, int xi, int yi, int xyi, int id) {
    tri[v].insert({xyi, id});
    rec[v].insert({yi, id});
    if(tl == tr) return;
    int tm = (tl + tr) >> 1;
    if(xi <= tm) {
        add(v << 1, tl, tm, xi, yi, xyi, id);
    }else {
        add(v << 1 | 1, tm + 1, tr, xi, yi, xyi, id);
    }
}

int get(int v, int tl, int tr, int lx, int rx, int yi, int xyi) {
    if(lx > rx) return 0;
    if(lx <= tl && tr <= rx) {
        return tri[v].order_of_key({xyi + 1, -inf}) - rec[v].order_of_key({yi, -inf});
    }
    int tm = (tl + tr) >> 1;
    int res = 0;
    if(lx <= tm) {
        res += get(v << 1, tl, tm, lx, rx, yi, xyi);
    }
    if(rx > tm) {
        res += get(v << 1 | 1, tm + 1, tr, lx, rx, yi, xyi);
    }
    return res;
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    m = readInt();

    xs.reserve(m + m);
    ys.reserve(m + m);
    xys.reserve(m + m);

    vec< pt > p(m + 1);

    for(int i = 1;i <= m;i++) {
        p[i].read();
        xs.push_back(p[i].x);
        ys.push_back(p[i].y);
        xs.push_back(p[i].x + p[i].r);
        ys.push_back(p[i].y + p[i].r);
        xys.push_back(p[i].x + p[i].y + p[i].r);
    }

    sort(ALL(xs));
    xs.resize(unique(ALL(xs)) - xs.begin());
    sort(ALL(ys));
    ys.resize(unique(ALL(ys)) - ys.begin());
    sort(ALL(xys));
    xys.resize(unique(ALL(xys)) - xys.begin());

    tri.resize((int)xs.size() * 4);
    rec.resize((int)xs.size() * 4);

    for(int i = 1;i <= m;i++) {
        if(p[i].t == 1) {
            int xi = lower_bound(ALL(xs), p[i].x) - xs.begin() + 1;
            int yi = lower_bound(ALL(ys), p[i].y) - ys.begin() + 1;
            int xyi = lower_bound(ALL(xys), p[i].x + p[i].y) - xys.begin() + 1;
            add(1, 1, (int)xs.size(), xi, yi, xyi, i);
        }else {
            int lx = lower_bound(ALL(xs), p[i].x) - xs.begin() + 1;
            int rx = lower_bound(ALL(xs), p[i].x + p[i].r) - xs.begin() + 1;
            int yi = lower_bound(ALL(ys), p[i].y) - ys.begin() + 1;
            int xyi = lower_bound(ALL(xys), p[i].x + p[i].y + p[i].r) - xys.begin() + 1;
            writeInt(get(1, 1, (int)xs.size(), lx, rx, yi, xyi), '\n');
        }
    }

    return 0;
}
