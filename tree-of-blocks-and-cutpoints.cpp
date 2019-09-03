#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef long long ll;

const int inf = 1e9;
const ll inf64 = 1e18;

struct Edge {
    int fr, to, id;

    int get(int v) {
        return v == fr ? to : fr;
    }
};

void dfs(const vector<vector<Edge>> &g, vector<int> &fup, vector<int> &tin,
        vector<int> &used, int &timer, int v, int par = -1) {
    tin[v] = fup[v] = timer++;
    used[v] = 1;
    for (Edge e : g[v]) {
        int to = e.get(v);
        if (to == par) continue;
        if (used[to]) {
            fup[v] = min(fup[v], tin[to]);
        } else {
            dfs(g, fup, tin, used, timer, to, v);
            fup[v] = min(fup[v], fup[to]);
        }
    }
}

void paintEdges(const vector<vector<Edge>> &g, vector<int> &fup,
                vector<int> &tin, vector<int> &used,
                vector<int> &colors, int v, int curColor, int &maxColor, int par = -1) {
    used[v] = 1;
    for (Edge e : g[v]) {
        int to = e.get(v);
        if (to == par) continue;
        if (!used[to]) {
            if (tin[v] <= fup[to]) {
                int tmpColor = maxColor++;
                colors[e.id] = tmpColor;
                paintEdges(g, fup, tin, used, colors, to, tmpColor, maxColor, v);
            } else { colors[e.id] = curColor;
                paintEdges(g, fup, tin, used, colors, to, curColor, maxColor, v);}
        } else if (tin[to] < tin[v]) {
            colors[e.id] = curColor;
        }
    }
}

vector<vector<Edge>> get2components(const vector<vector<Edge>> &g,
                                    int m, const vector<Edge> &es) {
    int n = (int)g.size();
    vector<int> fup(n), tin(n), used(n);
    vector<int> colors(m);
    int timer;
    used.assign(n, 0);
    timer = 0;
    for (int v = 0; v < n; v++) {
        if (used[v]) continue;
        dfs(g, fup, tin, used, timer, v);
    }
    used.assign(n, 0);
    timer = 0;
    for (int v = 0; v < n; v++) {
        if (used[v]) continue;
        paintEdges(g, fup, tin, used, colors, v, timer, timer, -1);
    }
    vector<vector<Edge>> res(timer);
    for (int i = 0; i < m; i++) {
        res[colors[i]].push_back(es[i]);
    }
    return res;
}

void go(const vector<vector<int>> &g, vector<int> &parent, int v, int par = -1) {
    parent[v] = par;
    for (int to : g[v]) {
        if (to == par) continue;
        go(g, parent, to, v);
    }
}

vector<int> getCycle(vector<Edge> edges) {
    vector<int> crd;
    for (Edge e : edges) {
        crd.push_back(e.fr);
        crd.push_back(e.to);
    }
    sort(crd.begin(), crd.end());
    crd.resize(unique(crd.begin(), crd.end()) - crd.begin());
    int sz = (int)crd.size();
    vector<vector<int>> g(sz);
    for (Edge e : edges) {
        int u, v;
        u = lower_bound(crd.begin(), crd.end(), e.fr) - crd.begin();
        v = lower_bound(crd.begin(), crd.end(), e.to) - crd.begin();
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> cycle;
    cycle.push_back(0);
    vector<int> used(sz);
    used[0] = 1;
    while ((int)cycle.size() < sz) {
        int last = cycle.back();
        for (int to : g[last]) {
            if (!used[to]) {
                used[to] = 1;
                cycle.push_back(to);
                break;
            }
        }
    }
    for (int &v : cycle) {
        v = crd[v];
    }
    return cycle;
}

pair<vector<vector<int>>, vector<vector<int>>> get_tree_of_blocks_and_cut_points(int n, vector<Edge> edges) {
    // be careful with n = 1
    int m = (int)edges.size();
    vector<vector<Edge>> g(n);
    for (int i = 0; i < m; i++) {
        g[edges[i].fr].push_back(edges[i]);
        g[edges[i].to].push_back(edges[i]);
    }
    vector<vector<Edge>> cycles = get2components(g, m, edges);
    vector<int > cutPoint(n);
    vector<int> color(m);
    for (int i = 0; i < (int)cycles.size(); i++) {
        for (Edge e : cycles[i]) {
            color[e.id] = i;
        }
    }
    vector<set<int>> neighColors(n);
    int cntCutPoints = 0;
    {
        for (Edge e : edges) {
            neighColors[e.fr].insert(color[e.id]);
            neighColors[e.to].insert(color[e.id]);
        }
        for (int v = 0; v < n; v++) {
            if ((int)neighColors[v].size() < 2) {
                continue;
            }
            cutPoint[v] = 1;
            cntCutPoints++;
        }
    }
    vector<vector<int>> blocks;
    for (int v = 0; v < n; v++) {
        if (cutPoint[v]) {
            vector<int> block;
            block.push_back(v);
            blocks.push_back(block);
        }
    }
    for (auto &cycle : cycles) {
        blocks.push_back(getCycle(cycle));
    }
    int N = (int)blocks.size();
    vector<vector<int>> G(N);
    for (int cur = -1, v = 0; v < n; v++) {
        if (!cutPoint[v]) continue;
        cur++;
        for (int cl : neighColors[v]) {
            G[cur].push_back(cntCutPoints + cl);
            G[cntCutPoints + cl].push_back(cur);
        }
    }
    return make_pair(blocks, G);
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m;

    cin >> n >> m;

    if (n == 1) {
        cout << "0\n";
        return 0;
    }

    vector<Edge> edges(m);
    vector<vector<Edge>> g(n);

    for (int i = 0; i < m; i++) {
        edges[i].id = i;
        cin >> edges[i].fr >> edges[i].to;
        edges[i].fr--;
        edges[i].to--;

        g[edges[i].fr].push_back(edges[i]);
        g[edges[i].to].push_back(edges[i]);
    }

    vector<vector<Edge>> cycles = get2components(g, m, edges);
    vector<int > cutPoint(n);
    vector<int> color(m);
    for (int i = 0; i < (int)cycles.size(); i++) {
        for (Edge e : cycles[i]) {
            color[e.id] = i;
        }
    }

    vector<set<int>> neighColors(n);
    int cntCutPoints = 0;

    {
        for (Edge e : edges) {
            neighColors[e.fr].insert(color[e.id]);
            neighColors[e.to].insert(color[e.id]);
        }
        for (int v = 0; v < n; v++) {
            if ((int)neighColors[v].size() < 2) {
                continue;
            }
            cutPoint[v] = 1;
            cntCutPoints++;
        }
    }

    vector<vector<int>> blocks;

    for (int v = 0; v < n; v++) {
        if (cutPoint[v]) {
            vector<int> block;
            block.push_back(v);
            blocks.push_back(block);
        }
    }

    for (auto &cycle : cycles) {
        blocks.push_back(getCycle(cycle));
    }

    int N = (int)blocks.size();
//
//    for (int i = 0; i < N; i++) {
//        cout << i << " : ";
//        for (int x : blocks[i]) {
//            cout << x + 1 << " ";
//        }
//        cout << "\n";
//    }

    vector<vector<int>> G(N);

    for (int cur = -1, v = 0; v < n; v++) {
        if (!cutPoint[v]) continue;
        cur++;
        for (int cl : neighColors[v]) {
//            cout << cur << " " << cntCutPoints + cl << "\n";
            G[cur].push_back(cntCutPoints + cl);
            G[cntCutPoints + cl].push_back(cur);
        }
    }

    vector<int> parent(N, -1);

    go(G, parent, 0, -1);

//    cout << "N = " << N << "\n";

    vector<int> degree(N, 0);

    for (int b = 1; b < N; b++) {
        degree[parent[b]]++;
    }

    vector<int> stack;

    for (int b = 0; b < N; b++) {
        if (!degree[b]) {
            stack.push_back(b);
        }
    }

//    for (int b = 0; b < N; b++) {
//        cout << b << " parent = " << parent[b] << ", degree = " << degree[b] << "\n";
//    }

    ll result = 0;

    vector<ll> weight(n, 1);
    vector<int> done(N, 0);

    while (!stack.empty()) {
        int b = stack.back();
//        cout << b << " : ";
//        for (int x : blocks[b]) {
//            cout << x << " ";
//        }
//        cout << "\n";
        stack.pop_back();
        done[b] = 1;
        if (blocks[b].size() == 1) { // cut point
            if(b && !--degree[parent[b]]) {
                stack.push_back(parent[b]);
            }
            continue;
        }
        if (blocks[b].size() == 2) { // bridge
            int u, v;
            u = blocks[b][0];
            v = blocks[b][1];
            if (!b) {
                break;
            }
            int cp = blocks[parent[b]][0];
            if (u == cp) {
                result += weight[v] * (n - weight[v]);
                weight[u] += weight[v];
            } else {
                result += weight[u] * (n - weight[u]);
                weight[v] += weight[u];
            }
            if (b && !--degree[parent[b]]) {
                stack.push_back(parent[b]);
            }
            continue;
        }
        int cp = b ? blocks[parent[b]][0] : -1;
        auto & cycle = blocks[b];
        ll sumW = 0;
        for (int x : cycle) {
            sumW += weight[x];
        }
        // BEGIN
        int kk = (int)cycle.size();
        ll sum_w_1 = 0;
        ll sum_w_2 = 0;
        ll sum_wj_1 = 0;
        ll sum_wj_2 = 0;
        for (int j = 0, i = 0; i < kk; i++) {
            // [0, j) [j, i)
            while (j < i - kk / 2) {
                sum_w_1 -= weight[cycle[j]];
                sum_w_2 += weight[cycle[j]];
                sum_wj_1 -= weight[cycle[j]] * j;
                sum_wj_2 += weight[cycle[j]] * j;
                j++;
            }
            result += weight[cycle[i]] * (sum_w_1 * i - sum_wj_1);
            result += weight[cycle[i]] * (sum_w_2 * (kk - i) + sum_wj_2);
            sum_w_1 += weight[cycle[i]];
            sum_wj_1 += weight[cycle[i]] * i;
        }
//        for (int i = 0; i < kk; i++) {
//            for (int j = 0; j < i; j++) {
//                int d = abs(i - j);
//                d = min(d, (int)cycle.size() - d);
//                result += weight[cycle[i]] * weight[cycle[j]] * d;
//            }
//        }
        // END
        if (cp != -1) {
            int pos_cp = -1;
            for (int i = 0; i < (int)cycle.size(); i++) {
                if (cycle[i] == cp) {
                    pos_cp = i;
                }
            }
            ll cf = n - sumW;
            for (int i = 0; i < (int)cycle.size(); i++) {
                int d = abs(pos_cp - i);
                d = min(d, (int)cycle.size() - d);
                result += cf * d * weight[cycle[i]];
            }
            weight[cp] = sumW;
            if (b && !--degree[parent[b]]) {
                stack.push_back(parent[b]);
            }
        }
    }

    cout << result << "\n";

    return 0;
}