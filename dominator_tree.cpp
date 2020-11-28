struct Edge {
    int fr = -1;
    int to = -1;
    int id = -1;
};

struct DSU {
    int n = 0; // [0, n)
    vector<int> p, mn;

    DSU() = default;

    DSU(int nn) {
        n = nn;
        p.resize(n);
        mn.resize(n, inf);
        for (int v = 0; v < n; v++)
            p[v] = v;
    }

    void set_value(int v, int x) {
        mn[v] = x;
    }

    int find(int v) {
        if (p[v] == v)
            return v;
        int pv = find(p[v]);
        mn[v] = min(mn[v], mn[p[v]]);
        p[v] = pv;
        return pv;
    }

    void merge(int P, int S) {
        p[S] = P;
    }
};

struct DominatorTree {
    int n = 0; // [0, n)
    vector<Edge> edges;
    vector<vector<int>> g, gr;
    vector<int> used, tin, sdom, idom, order, depth;
    DSU dsu;

    vector<vector<int>> cost, parent;

    DominatorTree() = default;

    DominatorTree(int nn) {
        n = nn;
    }

    void add_edge(Edge e) {
        edges.push_back(e);
    }

    void dfs(int v) {
        used[v] = 1;
        tin[v] = (int) order.size();
        order.push_back(v);
        for (int eid : g[v]) {
            const auto& e = edges[eid];
            if (!used[e.to]) {
                depth[e.to] = depth[v] + 1;
                parent[0][e.to] = v;
                dfs(e.to);
            }
        }
    }

    void init_binary_jumps() {
        int LOG = 0;
        while ((1 << LOG) < n)
            LOG++;
        cost.resize(LOG, vector<int>(n, inf));
        parent.resize(LOG, vector<int>(n, -1));
    }

    void build_sdom(int s) {
        used.assign(n, 0);
        tin.assign(n, 0);
        depth.assign(n, 0);
        order.clear();
        dfs(s);

        sdom.assign(n, inf);
        idom.assign(n, inf);
        dsu = DSU(n);

        for (int it = (int) order.size() - 1; it >= 0; it--) {
            int v = order[it];
            for (int eid : gr[v]) {
                const auto& e = edges[eid];
                if (!used[e.fr])
                    continue;
                sdom[v] = min(sdom[v], tin[e.fr]);
                if (tin[e.fr] > tin[v]) {
                    dsu.find(e.fr);
                    sdom[v] = min(sdom[v], dsu.mn[e.fr]);
                }
            }
            dsu.set_value(v, sdom[v]);
            for (int eid : g[v]) {
                const auto& e = edges[eid];
                if (parent[0][e.to] == v) {
                    dsu.merge(v, e.to);
                }
            }
        }
    }

    int get_min_on_path(int P, int S) {
        int res = inf;
        for (int j = (int) cost.size() - 1; j >= 0; j--) {
            int pS = parent[j][S];
            if (pS == -1 || depth[pS] < depth[P])
                continue;
            res = min(res, cost[j][S]);
            S = pS;
        }
        return res;
    }

    void set_value(int v, int x) {
        cost[0][v] = x;
        for (int j = 1; j < (int) cost.size(); j++) {
            int pv = parent[j - 1][v];
            if (pv == -1) {
                cost[j][v] = cost[j - 1][v];
                parent[j][v] = pv;
            } else {
                cost[j][v] = min(cost[j - 1][v], cost[j - 1][pv]);
                parent[j][v] = parent[j - 1][pv];
            }
        }
    }

    void build_idom(int s) {
        for (int v : order) {
            if (v == s)
                continue;
            idom[v] = min(sdom[v], get_min_on_path(order[sdom[v]], parent[0][v]));
            set_value(v, idom[v]);
        }
    }

    void build(int s) {
        init_binary_jumps();
        g.clear();
        g.resize(n);
        gr.clear();
        gr.resize(n);
        for (int i = 0; i < (int) edges.size(); i++) {
            const auto& e = edges[i];
            g[e.fr].push_back(i);
            gr[e.to].push_back(i);
        }

        build_sdom(s);
        build_idom(s);

//        for (int v = 0; v < n; v++) {
//            if (!used[v])
//                continue;
//            cout << v << " : " << (sdom[v] == inf ? -1 : order[sdom[v]]) << ", " << (idom[v] == inf ? -1 : order[idom[v]]) << "\n";
//        }
    }
};
