mt19937 gen(42);
struct FastRope { // bunch of ropes - the forest, where each component is a bamboo
    struct Node;

    int n; // [1, n]
    vector<Node*> nodes;
    Node *nl, *nm, *nr;

    FastRope(int nn) {
        n = nn;
        nodes.resize(n + 1);
        for (int i = 1; i <= n; i++) {
            nodes[i] = new Node();
        }
    }

    bool is_end(int v) {
        auto [root, pos] = get_pos(nodes[v]);
        return pos == 1 || pos == get_cnt(root);
    }

    bool are_connected(int u, int v) {
        return get_root(nodes[u]) == get_root(nodes[v]);
    }

    void add(int u, int v) {
        auto [ru, pu] = get_pos(nodes[u]);
        auto [rv, pv] = get_pos(nodes[v]);
        if (pu != get_cnt(ru))
            do_rev(ru);
        if (pv != 1)
            do_rev(rv);
        merge(ru, ru, rv);
    }

    void remove(int u, int v) {
        auto [root, pu] = get_pos(nodes[u]);
        auto [_, pv] = get_pos(nodes[v]);
        nl = nr = nullptr;
        split(root, nl, nr, min(pu, pv));
    }

    struct Node {
        Node* l = nullptr;
        Node* r = nullptr;
        Node* p = nullptr;
        int prior = gen();
        int cnt = 1;
        int rev = 0;
    };

    int get_cnt(Node* v) {
        return v ? v->cnt : 0;
    }

    void update(Node* v) {
        if (!v)
            return;
        v->cnt = get_cnt(v->l) + get_cnt(v->r) + 1;
        if (v->l)
            v->l->p = v;
        if (v->r)
            v->r->p = v;
        v->p = nullptr;
    }

    void do_rev(Node* v) {
        if (v) {
            v->rev ^= 1;
            swap(v->l, v->r);
        }
    }

    void push(Node* v) {
        if (!v)
            return;
        if (v->rev) {
            do_rev(v->l);
            do_rev(v->r);
            v->rev = 0;
        }
    }

    void merge(Node*& v, Node* l, Node* r) {
        if (!l || !r) {
            v = l ? l : r;
            return;
        }
        push(l);
        push(r);
        if (l->prior < r->prior) {
            merge(l->r, l->r, r);
            v = l;
        } else {
            merge(r->l, l, r->l);
            v = r;
        }
        update(v);
    }

    void split(Node* v, Node*& l, Node*& r, int x) { // get_cnt(l) == x
        if (!v) {
            l = r = nullptr;
            return;
        }
        push(v);
        if (get_cnt(v->l) + 1 <= x) {
            split(v->r, v->r, r, x - get_cnt(v->l) - 1);
            l = v;
        } else {
            split(v->l, l, v->l, x);
            r = v;
        }
        update(l);
        update(r);
    }

    void push_path(Node* v) {
        static vector<Node*> path;
        path.clear();
        while (v) {
            path.push_back(v);
            v = v->p;
        }
        reverse(path.begin(), path.end());
        for (auto x : path)
            push(x);
    }

    pair<Node*, int> get_pos(Node* v) {
        if (!v)
            return {nullptr, 0};
        push_path(v);
        int pos = 1 + get_cnt(v->l);
        while (v->p) {
            if (v->p->r == v)
                pos += get_cnt(v->p->l) + 1;
            v = v->p;
        }
        return {v, pos};
    }

    Node* get_root(Node* v) {
        if (!v)
            return nullptr;
        while (v->p)
            v = v->p;
        return v;
    }
};
