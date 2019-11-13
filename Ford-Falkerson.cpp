struct FordFalkerson { // O(|f| * E)

    struct Edge {
        int fr, to, cp, fl;
    };

    int n, S, T;
    vec< Edge  > es;
    vec< vec< int > > g;
    vec< int > used;

    FordFalkerson(int _n, int _S, int _T):
        n(_n), S(_S), T(_T)
    {
        g.resize(n);
    }

    void add_edge(int fr, int to, int cp) {
        g[fr].push_back((int)es.size());
        es.push_back({fr, to, cp, 0});
        g[to].push_back((int)es.size());
        es.push_back({to, fr, 0, 0});
    }

    int dfs(int v, int _push = INT_MAX) {
        if(used[v]) return 0;
        if(v == T || !_push) return _push;
        used[v] = 1;
        for(int ps : g[v]) {
            Edge &e = es[ps];
            int tmp = dfs(e.to, min(_push, e.cp - e.fl));
            if(tmp) {
                e.fl += tmp;
                es[ps ^ 1].fl -= tmp;
                return tmp;
            }
        }
        return 0;
    }

    int get_max_flow() {
        int flow = 0, add;
        used.assign(n, 0);
        while((add = dfs(S))) {
            flow += add;
            used.assign(n, 0);
        }
        return flow;
    }
};
