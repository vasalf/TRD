struct line {
    ll k, b;

    line():
        k(inf64), b(inf64)
    { }

    line(ll _k, ll _b):
        k(_k), b(_b)
    { }

    ll get(ll x) {
        return k == inf64 ? inf64 : k * x + b;
    }
};

struct LiChaoTree {

    ll get_mid(ll lx, ll rx) {
        ll mx = lx + rx;
        return mx >= 0 ? mx / 2 : mx / 2 - 1;
    }

    struct Node {
        ll lx, rx;
        line best;
        Node* l;
        Node* r;

        Node():
            lx(-inf), rx(+inf), best(), l(0), r(0)
        { }

        Node(ll _lx, ll _rx):
            lx(_lx), rx(_rx), best(), l(0), r(0)
        { }

        ~Node() {
            if(l) {
                delete l;
            }
            if(r) {
                delete r;
            }
        }
    };

    Node* get_left(Node* v) {
        if(!v->l) {
            v->l = new Node(v->lx, get_mid(v->lx, v->rx));
        }
        return v->l;
    }

    Node* get_right(Node* v) {
        if(!v->r) {
            v->r = new Node(get_mid(v->lx, v->rx) + 1, v->rx);
        }
        return v->r;
    }

    Node* root;
    vec< line > add_list;

    LiChaoTree(): root(new Node())
    { }

    void update(Node* v, line add) {
        ll mx = get_mid(v->lx, v->rx);
        if(add.get(mx) < v->best.get(mx)) {
            std::swap(v->best, add);
        }
        if(v->lx == v->rx) {
            return;
        }
        if(add.get(mx) == v->best.get(mx)) {
            if(add.get(v->lx) < v->best.get(v->lx)) {
                update(get_left(v), add);
            }else {
                update(get_right(v), add);
            }
        }else if(add.get(v->lx) < v->best.get(v->lx)) {
            update(get_left(v), add);
        }else {
            update(get_right(v), add);
        }
    }

    void add_line(line add) {
        add_list.push_back(add);
        update(root, add);
    }

    ll query(Node* v, ll x) {
        if(!v) return inf64;
        ll res = v->best.get(x);
        if(v->lx == v->rx) {
            return res;
        }
        ll mx = get_mid(v->lx, v->rx);
        if(x <= mx) {
            res = min(res, query(v->l, x));
        }else {
            res = min(res, query(v->r, x));
        }
        return res;
    }

    ll query(ll x) {
        return query(root, x);
    }

    ~LiChaoTree() {
        delete root;
    }

    void swap(LiChaoTree& o) {
        std::swap(root, o.root);
        std::swap(add_list, o.add_list);
    }

    int size() const {
        return (int)add_list.size();
    }
};

void merge(LiChaoTree& a, LiChaoTree& b) {
    if(a.size() < b.size()) {
        a.swap(b);
    }
    for(line l : b.add_list) {
        a.add_line(l);
    }
}