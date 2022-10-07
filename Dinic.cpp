struct Dinic {
  struct Edge {
    int fr, to, cp, id, fl;
  };
  int n, S, T;
  vector<Edge> es;
  vector<vector<int>> g;
  vector<int> dist, res, ptr, used;
  Dinic(int n_, int S_, int T_)
      : n(n_), S(S_), T(T_) {
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
    queue<int> q;
    q.push(S);
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int ps : g[v]) {
        Edge& e = es[ps];
        if (e.fl + K > e.cp) continue;
        if (dist[e.to] > dist[e.fr] + 1) {
          dist[e.to] = dist[e.fr] + 1;
          q.push(e.to);
        }
      }
    }
    return dist[T] < inf;
  }
  int dfs(int v, int _push = INT_MAX) {
    if (v == T || !_push) return _push;
    for (int& iter = ptr[v];
         iter < (int)g[v].size(); iter++) {
      int ps = g[v][ptr[v]];
      Edge& e = es[ps];
      if (dist[e.to] != dist[e.fr] + 1) continue;
      int tmp =
        dfs(e.to, min(_push, e.cp - e.fl));
      if (tmp) {
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
    for (int K = 1 << 30; K > 0; K >>= 1) {
      while (bfs(K)) {
        ptr.assign(n, 0);
        while ((add_flow = dfs(S))) {
          max_flow += add_flow;
        }
      }
    }
    return max_flow;
  }
  void assign_result() {
    res.resize(es.size());
    for (Edge e : es)
      if (e.id != -1) res[e.id] = e.fl;
  }
  int get_flow(int id) { return res[id]; }
  bool go(int v, vector<int>& F,
          vector<int>& path) {
    if (v == T) return 1;
    if (used[v]) return 0; used[v] = 1;
    for (int ps : g[v]) {
      if (F[ps] <= 0) continue;
      if (go(es[ps].to, F, path)) {
        path.push_back(ps);
        return 1;}}
    return 0;}
  vector<pair<int, vector<int>>> decomposition() {
    find_max_flow();
    vector<int> F((int)es.size()), path, add;
    vector<pair<int, vector<int>>> dcmp;
    for (int i = 0; i < (int)es.size(); i++)
      F[i] = es[i].fl;
    used.assign(n, 0);
    while (go(S, F, path)) {
      used.assign(n, 0);
      int mn = INT_MAX;
      for (int ps : path)
        mn = min(mn, F[ps]);
      for (int ps : path)
        F[ps] -= mn;
      for (int ps : path)
        add.push_back(es[ps].id);
      reverse(add.begin(), add.end());
      dcmp.push_back({mn, add});
      add.clear();
      path.clear();
    }
    return dcmp;
  }
};
