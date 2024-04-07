int pos(int v) { return v + 1; }
int neg(int v) { return -v - 1; }
int var(int v) { return abs(v) - 1; }
struct SATSolver {
  int n;
  vector<vector<int>> f;
  vector<int> xval;
  SATSolver(int n) : n(n), xval(n, -1) {}
  void add_clause(vector<int> clause) {
    f.emplace_back(std::move(clause));
  }
  int value(int lit) {
    if (lit > 0) {
      return xval[lit - 1];
    } else if (xval[-lit - 1] == -1) {
      return -1;
    } else {
      return !xval[-lit - 1];
    }
  }
  vector<int> clause_vals;
  vector<int> clause_size;
  void calc_clause_vals() {
    clause_vals.assign(f.size(), -1);
    clause_size.assign(f.size(), 0);
    forn(i, f.size()) {
      for (auto l : f[i]) {
        int val = value(l);
        if (val == -1) {
          clause_size[i]++;
        } else if (val == 1) {
          clause_vals[i] = 1;
          break;
        }
      }
      if (clause_vals[i] == -1 &&
          clause_size[i] == 0) {
        clause_vals[i] = 0;
      }
    }
  }
  int formula_val() {
    bool undecided = false;
    forn(i, f.size()) {
      if (clause_vals[i] == -1) {
        undecided = true;
      }
      if (clause_vals[i] == 0) { return 0; }
    }
    return undecided ? -1 : 1;
  }
  vector<int> cpos, cneg;
  bool reduce_small_vars() {
    cpos.assign(n, 0);
    cneg.assign(n, 0);
    forn(i, f.size()) {
      if (clause_vals[i] != -1) { continue; }
      for (int l : f[i]) {
        if (value(l) != -1) { continue; }
        (l > 0 ? cpos : cneg)[var(l)]++;
      }
    }
    bool reduced = false;
    forn(i, n) {
      if (xval[i] != -1) continue;
      if (cpos[i] == 0 && cneg[i] == 0) {
        xval[i] = 1;
      } else if (cpos[i] == 0) {
        xval[i] = 0;
        reduced = true;
      } else if (cneg[i] == 0) {
        xval[i] = 1;
        reduced = true;
      }
    }
    return reduced;
  }
  bool reduce_small_clauses() {
    bool reduced = false;
    forn(i, f.size()) {
      if (clause_vals[i] == -1 &&
          clause_size[i] == 1) {
        int only_lit = 0;
        for (int l : f[i]) {
          if (value(l) == -1) { only_lit = l; }
        }
        if (only_lit == 0) continue;
        reduced = true;
        xval[var(only_lit)] = only_lit > 0;
      }
    }
    return reduced;
  }
  int reduce() {
    while (true) {
      calc_clause_vals();
      int fv = formula_val();
      if (fv != -1) { return fv; }
      if (reduce_small_vars()) { continue; }
      if (reduce_small_clauses()) { continue; }
      break;
    }
    return -1;
  }
  bool solve() {
    int r = reduce();
    if (r != -1) return r;
    int v = 0;
    forn(i, n) {
      if (cpos[v] + cneg[v] < cpos[i] + cneg[i]) {
        v = i;
      }
    }
    vector<int> save_xval = xval;
    xval[v] = 1;
    if (solve()) { return true; }
    xval = save_xval;
    xval[v] = 0;
    return solve();
  }
};
