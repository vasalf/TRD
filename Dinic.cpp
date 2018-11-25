struct Dinic {

    struct Edge {
        int fr, to, cp, id, fl;
    };

    int n, S, T;
    vec< Edge > es;
    vec< vec< int > > g;
    vec< int > dist, res, ptr;

    Dinic(int _n, int _S, int _T):
        n(_n), S(_S), T(_T)
    {
        g.resize(n);
    }

    void add_edge(int fr, int to, int cp, int id) {
        g[fr].push_back((int)es.size());
        es.push_back({fr, to, cp, id, 0});
        g[to].push_back((int)es.size());
        es.push_back({to, fr, 0, -1, 0});
    }

    bool bfs(int K) {
        dist.assign(n, inf);
        dist[S] = 0;
        queue< int > q;
        q.push(S);
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            for(int ps : g[v]) {
                Edge &e = es[ps];
                if(e.fl + K > e.cp) continue;
                if(dist[e.to] > dist[e.fr] + 1) {
                    dist[e.to] = dist[e.fr] + 1;
                    q.push(e.to);
                }
            }
        }
        return dist[T] < inf;
    }

    int dfs(int v, int _push = INT_MAX) {
        if(v == T || !_push) return _push;
        for(int &iter = ptr[v];iter < (int)g[v].size();iter++) {
            int ps = g[v][ ptr[v] ];
            Edge &e = es[ps];
            if(dist[e.to] != dist[e.fr] + 1) continue;
            int tmp = dfs(e.to, min(_push, e.cp - e.fl));
            if(tmp) {
//                cout << e.fr << " " << e.to << ", ";
                e.fl += tmp;
                es[ps ^ 1].fl -= tmp;
                return tmp;
            }
        }
        return 0;
    }

    ll find_max_flow() {
        ptr.resize(n);
        ll max_flow = 0, add_flow;
        for(int K = 1 << 30;K > 0;K >>= 1) {
            while(bfs(K)) {
        //            cout << "HM\n";
                ptr.assign(n, 0);
                while((add_flow = dfs(S))) {
                    max_flow += add_flow;
        //                cout << "\n";
                }
            }
        }
//        for(Edge e : es) {
//            cout << e.fr+1 << " -> " << e.to+1 << " flow = " << e.fl << ", cap = " << e.cp << "\n";
//        }
        return max_flow;
    }

    void assign_result() {
        res.resize(es.size());
        for(Edge e : es) if(e.id != -1) res[e.id] = e.fl;
    }

    int get_flow(int id) {
        return res[id];
    }

    bool go(int v, vec< int > &F, vec< int > &path) {
        if(v == T) return 1;
        for(int ps : g[v]) {
            if(F[ps] <= 0) continue;
            if(go(es[ps].to, F, path)) {
                path.push_back(ps);
                return 1;
            }
        }
        return 0;
    }

    vec< pair< int, vec< int > > > decomposition() {
        find_max_flow();
        vec< int > F((int)es.size()), path, add;
        vec< pair< int, vec< int > > > dcmp;
        for(int i = 0;i < (int)es.size();i++) F[i] = es[i].fl;
        while(go(S, F, path)) {
            int mn = INT_MAX;
            for(int ps : path) mn = min(mn, F[ps]);
            for(int ps : path) F[ps] -= mn;
            for(int ps : path) add.push_back(es[ps].id);
            reverse(ALL(add));
            dcmp.push_back({mn, add});
            add.clear();
            path.clear();
        }
        return dcmp;
    }
};
