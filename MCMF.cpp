struct MCMF {
  struct Edge {
    int fr, to, cp, fl, cs, id;
  };
  int n, S, T;
  vec<Edge> es;
  vec<vec<int>> g;
  vec<ll> dist, phi;
  vec<int> from;
  MCMF(int _n, int _S, int _T)
      : n(_n), S(_S), T(_T) {
    g.resize(n);
  }
  void add_edge(int fr, int to, int cp, int cs,
                int id) {
    g[fr].push_back((int)es.size());
    es.push_back({fr, to, cp, 0, cs, id});
    g[to].push_back((int)es.size());
    es.push_back({to, fr, 0, 0, -cs, -1});
  }
  void init_phi() {
    dist.assign(n, LLONG_MAX);
    dist[S] = 0;
    for (int any, iter = 0; iter < n - 1;
         iter++) { // Ford Bellman
      any = 0;
      for (Edge e : es) {
        if (e.fl == e.cp) continue;
        if (dist[e.to] - dist[e.fr] > e.cs) {
          dist[e.to] = dist[e.fr] + e.cs;
          any = 1;
        }
      }
      if (!any) break;
    }
    phi = dist;
  }
  bool Dijkstra() {
    dist.assign(n, LLONG_MAX);
    from.assign(n, -1);
    dist[S] = 0;
    priority_queue<pair<ll, int>,
                   vec<pair<ll, int>>,
                   greater<pair<ll, int>>>
      pq;
    pq.push({dist[S], S});
    while (!pq.empty()) {
      int v;
      ll di;
      tie(di, v) = pq.top();
      pq.pop();
      if (di != dist[v]) continue;
      for (int ps : g[v]) {
        Edge& e = es[ps];
        if (e.fl == e.cp) continue;
        if (dist[e.to] - dist[e.fr] >
            e.cs + phi[e.fr] - phi[e.to]) {
          dist[e.to] = dist[e.fr] + e.cs +
                       phi[e.fr] - phi[e.to];
          from[e.to] = ps;
          pq.push({dist[e.to], e.to});
        }
      }
    }
    for (int v = 0; v < n; v++) {
      phi[v] += dist[v];
    }
    return dist[T] < LLONG_MAX;
  }
  pll find_mcmf() {
    init_phi();
    ll flow = 0, cost = 0;
    while (Dijkstra()) {
      int mn = INT_MAX;
      for (int v = T; v != S;
           v = es[from[v]].fr) {
        mn = min(mn,
                 es[from[v]].cp - es[from[v]].fl);
      }
      flow += mn;
      for (int v = T; v != S;
           v = es[from[v]].fr) {
        es[from[v]].fl += mn;
        es[from[v] ^ 1].fl -= mn;
      }
    }
    for (Edge& e : es) {
      if (e.fl >= 0) cost += 1ll * e.fl * e.cs;
    }
    return make_pair(flow, cost);
  }
  bool go(int v, vec<int>& F, vec<int>& path,
          vec<int>& used) {
    if (used[v]) return 0;
    used[v] = 1;
    if (v == T) return 1;
    for (int ps : g[v]) {
      if (F[ps] <= 0) continue;
      if (go(es[ps].to, F, path, used)) {
        path.push_back(ps);
        return 1;
      }
    }
    return 0;
  }
  vec<pair<int, vec<int>>>
  decomposition(ll& _flow, ll& _cost) {
    tie(_flow, _cost) = find_mcmf();
    vec<int> F((int)es.size()), path, add,
      used(n);
    vec<pair<int, vec<int>>> dcmp;
    for (int i = 0; i < (int)es.size(); i++)
      F[i] = es[i].fl;
    while (go(S, F, path, used)) {
      used.assign(n, 0);
      int mn = INT_MAX;
      for (int ps : path)
        mn = min(mn, F[ps]);
      for (int ps : path)
        F[ps] -= mn;
      for (int ps : path)
        add.push_back(es[ps].id);
      reverse(ALL(add));
      dcmp.push_back({mn, add});
      add.clear();
      path.clear();
    }
    return dcmp;
  }
};
