struct HopcroftKarp {
    int n, m;
    vec< vec< int > > g;
    vec< int > pl, pr, dist;
    vec< bool > vis;

    HopcroftCarp():
        n(0), m(0)
    { }

    HopcroftCarp(int _n, int _m):
        n(_n), m(_m)
    {
        g.resize(n);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    bool bfs() {
        dist.assign(n + 1, inf);
        queue< int > q;
        for(int u = 0;u < n;u++) {
            if(pl[u] < m) continue;
            dist[u] = 0;
            q.push(u);
        }
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            if(dist[u] >= dist[n]) continue;
            for(int v : g[u]) {
                if(dist[ pr[v] ] > dist[u] + 1) {
                    dist[ pr[v] ] = dist[u] + 1;
                    q.push(pr[v]);
                }
            }
        }
        return dist[n] < inf;
    }

    bool dfs(int v) {
        if(v == n) return 1;
        vis[v] = true;
        for(int to : g[v]) {
            if(dist[ pr[to] ] != dist[v] + 1) continue;
            if(vis[pr[to]]) continue;
            if(!dfs(pr[to])) continue;
            pl[v] = to;
            pr[to] = v;
            return 1;
        }
        return 0;
    }

    int find_max_matching() {
        pl.resize(n, m);
        pr.resize(m, n);
        int result = 0;
        while(bfs()) {
            vis.assign(n + 1, false);
            for(int u = 0;u < n;u++) {
                if(pl[u] < m) continue;
                if(vis[u]) continue;
                result += dfs(u);
            }
        }
        return result;
    }
};
