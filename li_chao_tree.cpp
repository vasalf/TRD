struct LiChaoTree {
    int mn, mx;

    struct Node {
        int l, r;
        int lt, rt;
        Line line;
    };

    vector<Node> t;

    LiChaoTree(int L, int R)
        : mn(L), mx(R)
    {}

    void add_line(Line l) {
        if (t.empty()) {
            t = {{mn, mx, -1, -1, l}};
        }
        int v = 0;
        while (true) {
            int mid = (t[v].l + t[v].r) / 2;
            if (l.get_func(mid) > t[v].line.get_func(mid)) {
                swap(l, t[v].line);
                return;
            }
            if (l.get_func(t[v].l) > t[v].line.get_func(t[v].l)) {
                if (t[v].lt == -1) {
                    t[v].lt = t.size();
                    t.emplace_back(Node{t[v].l, mid, -1, -1, l});
                    return;
                }
                v = t[v].lt;
            } else if (l.get_func(t[v].r) > t[v].line.get_func(t[v].r)) {
                if (t[v].rt == -1) {
                    t[v].rt = t.size();
                    t.emplace_back(Node{mid, t[v].r, -1, -1, l});
                    return;
                }
                v = t[v].rt;
            } else {
                return;
            }
        }
    }

    ll get_max(ll x) {
        int v = 0;
        ll ret = -inf64;
        while (v != -1) {
            ret = max(ret, t[v].line.get_func(x));
            int mid = (t[v].l + t[v].r) / 2;
            if (x < mid) {
                v = t[v].lt;
            } else {
                v = t[v].rt;
            }
        }
        return ret;
    }

    bool empty() {
        return t.empty();
    }
};
