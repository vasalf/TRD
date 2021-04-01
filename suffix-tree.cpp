// Ukkonen's algorithm O(n)
const int A = 27; // Alphabet size
struct SuffixTree {
  struct Node { // [l, r) !!!
    int l, r, link, par;
    int nxt[A];
    Node() : l(-1), r(-1), link(-1), par(-1) {
      fill(nxt, nxt + A, -1);
    }
    Node(int _l, int _r, int _link, int _par)
        : l(_l), r(_r), link(_link), par(_par) {
      fill(nxt, nxt + A, -1);
    }
    int& next(int c) { return nxt[c]; }
    int get_len() const { return r - l; }
  };
  struct State {
    int v, len;
  };
  vec<Node> t;
  State cur_state;
  vec<int> s;
  SuffixTree() : cur_state({0, 0}) {
    t.push_back(Node());
  }
  // v -> v + s[l, r) !!!
  State go(State st, int l, int r) {
    while (l < r) {
      if (st.len == t[st.v].get_len()) {
        State nx = State({t[st.v].next(s[l]), 0});
        if (nx.v == -1) return nx;
        st = nx;
        continue;
      }
      if (s[t[st.v].l + st.len] != s[l])
        return State({-1, -1});
      if (r - l < t[st.v].get_len() - st.len)
        return State({st.v, st.len + r - l});
      l += t[st.v].get_len() - st.len;
      st.len = t[st.v].get_len();
    }
    return st;
  }
  int get_vertex(State st) {
    if (t[st.v].get_len() == st.len) return st.v;
    if (st.len == 0) return t[st.v].par;
    Node& v = t[st.v];
    Node& pv = t[v.par];
    Node add(v.l, v.l + st.len, -1, v.par);
    // nxt
    pv.next(s[v.l]) = (int)t.size();
    add.next(s[v.l + st.len]) = st.v;
    // par
    v.par = (int)t.size();
    // [l, r)
    v.l += st.len;
    t.push_back(add); // !!!
    return (int)t.size() - 1;
  }
  int get_link(int v) {
    if (t[v].link != -1) return t[v].link;
    if (t[v].par == -1) return 0;
    int to = get_link(t[v].par);
    to = get_vertex(
      go(State({to, t[to].get_len()}),
         t[v].l + (t[v].par == 0), t[v].r));
    return t[v].link = to;
  }
  void add_symbol(int c) {
    assert(0 <= c && c < A);
    s.push_back(c);
    while (1) {
      State hlp = go(cur_state, (int)s.size() - 1,
                     (int)s.size());
      if (hlp.v != -1) {
        cur_state = hlp;
        break;
      }
      int v = get_vertex(cur_state);
      Node add((int)s.size() - 1, +inf, -1, v);
      t.push_back(add);
      t[v].next(c) = (int)t.size() - 1;
      cur_state.v = get_link(v);
      cur_state.len = t[cur_state.v].get_len();
      if (!v) break;
    }
  }
};
