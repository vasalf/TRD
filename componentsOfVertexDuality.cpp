struct Edge {
    int fr, to, id;
    int get(int v) {
        return v == fr ? to : fr;
    }
};

void dfs(const vector<vector<Edge>> &g, vector<int> &fup, vector<int> &tin, vector<int> &used, int &timer, int v, int par = -1) {
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

void paintEdges(const vector<vector<Edge>> &g, vector<int> &fup, vector<int> &tin, vector<int> &used,
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
            } else {
                colors[e.id] = curColor;
                paintEdges(g, fup, tin, used, colors, to, curColor, maxColor, v);
            }
        } else if (tin[to] < tin[v]) {
            colors[e.id] = curColor;
        }
    }
}

vector<vector<Edge>> get2components(const vector<vector<Edge>> &g, int m, const vector<Edge> &es) {
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

//    for (int v = 0; v < n; v++) {
//        cout << " v = " << v + 1 << " : " << tin[v] << ", " << fup[v] << "  !\n";
//    }

    used.assign(n, 0);
    timer = 0;
    for (int v = 0; v < n; v++) {
        if (used[v]) continue;
        paintEdges(g, fup, tin, used, colors, v, timer, timer, -1);
    }

    vector<vector<Edge>> res(timer);
    for (int i = 0; i < m; i++) {
//        cout << i << " : " << es[i].fr << " -> " << es[i].to << " color = " << colors[i] << "\n";
        res[colors[i]].push_back(es[i]);
    }

    return res;
}
