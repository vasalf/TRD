pair<ll, vec<pii>>
hungary_algorithm(vec<vec<int>> a) {
  int n = (int)a.size();
  int m = (int)a[0].size();
  int inv = 0;
  if (n > m) {
    inv = 1;
    swap(n, m);
    vec<vec<int>> b(n, vec<int>(m));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
        b[i][j] = a[j][i];
      }
    a = b;
  }
  vec<int> pair_i(n, -1), pair_j(m, -1);
  vec<ll> add_str(n), add_col(m);
  for (int v = 0; v < n; v++) {
    vec<int> used_i(n), used_j(m);
    vec<int> prev(m, -1);
    vec<pair<int, ll>> arg(
      m, make_pair(-1, LLONG_MAX));
    auto add_new_str = [&](int ni) -> void {
      for (int qj = 0; qj < m; qj++) {
        ll hlp =
          a[ni][qj] + add_str[ni] + add_col[qj];
        if (hlp < arg[qj].second) {
          arg[qj] = {ni, hlp};
        }
      }
      used_i[ni] = 1;
    };
    add_new_str(v);
    while (1) {
      int i = -1, j;
      ll x = LLONG_MAX;
      for (int qj = 0; qj < m; qj++) {
        if (used_j[qj]) continue;
        int qi;
        ll tx;
        tie(qi, tx) = arg[qj];
        if (tx < x) x = tx, i = qi, j = qj;
      }
      if (i == -1) break;
      for (int qi = 0; qi < n; qi++) {
        if (used_i[qi]) continue;
        add_str[qi] += x;
      }
      for (int qj = 0; qj < m; qj++) {
        if (used_j[qj]) continue;
        add_col[qj] -= x;
        arg[qj].second -= x;
      }
      prev[j] = i;
      used_j[j] = 1;
      if (pair_j[j] == -1) {
        for (int u = j; u != -1;) {
          int nu = pair_i[prev[u]];
          pair_i[prev[u]] = u;
          pair_j[u] = prev[u];
          u = nu;
        }
        break;
      }
      add_new_str(pair_j[j]);
    }
  }
  ll total_sum = 0;
  vec<pii> pairs;
  for (int j, i = 0; i < n; i++) {
    if ((j = pair_i[i]) == -1) continue;
    total_sum += a[i][j];
    if (inv)
      pairs.push_back({j, i});
    else
      pairs.push_back({i, j});
  }
  return {total_sum, pairs};
}
