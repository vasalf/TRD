struct GeneralMatching { // O(n^3)
    int n = 0, cc = 10; // [0, n)
    vector<vector<int>> g; // undirected
    vector<int> mt, used, base, p, color;
    queue<int> q;

    GeneralMatching(int nn): n(nn), mt(n, -1), used(n), base(n), p(n), color(n), g(n) {}

    void add_edge(int u, int v) {
        g[u].push_back(v), g[v].push_back(u);
    }

    void add(int v) {
        if (!used[v])
            used[v] = 1, q.push(v);
    }

    int get_lca(int u, int v) {
        cc++;
        while (1) {
            u = base[u], color[u] = cc;
            if (mt[u] == -1) break;
            u = p[mt[u]];
        }
        while (1) {
            v = base[v];
            if (color[v] == cc) break;
            v = p[mt[v]];
        }
        return v;
    }

    void mark_path(int v, int child, int b) {
        while (base[v] != b) {
            color[base[v]] = color[base[mt[v]]] = cc;
            p[v] = child, child = mt[v], v = p[child];
        }
    }

    int bfs(int root) {
        add(root);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int to : g[v]) {
                if (base[v] == base[to] || mt[v] == to)
                    continue;
                else if (used[to]) {
                    int w = get_lca(v, to);
                    cc++, mark_path(v, to, w), mark_path(to, v, w);
                    for (int i = 0; i < n; i++)
                        if (color[base[i]] == cc)
                            base[i] = w, add(i);
                } else if (p[to] == -1) {
                    p[to] = v; if (mt[to] == -1) return to;
                    add(mt[to]);
                }
            }
        }
        return -1;
    }

    void xor_path(int v) {
        while (v != -1) {
            int pv = p[v], ppv = mt[pv];
            mt[v] = pv, mt[pv] = v;
            v = ppv;
        }
    }

    bool inc(int root) {
        used.assign(n, 0), p.assign(n, -1), iota(base.begin(), base.end(), 0);
        while (!q.empty()) q.pop();
        int v = bfs(root);
        if (v == -1) return false;
        xor_path(v);
        return true;
    }

    void match() {
        for (int i = 0; i < n; i++) if (mt[i] == -1) inc(i);
    }
};
