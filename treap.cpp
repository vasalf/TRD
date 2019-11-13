mt19937 rnd(42);

class Treap {
private:
    struct Node {
        Node* l;
        Node* r;
        ll pushv;
        ll value;
        ll cnt;
        ll prior;

        Node():
            l(0), r(0),
            pushv(0),
            value(0),
            cnt(0),
            prior(rnd())
        { }

        Node(ll _value):
            l(0), r(0),
            pushv(0),
            value(_value),
            cnt(1),
            prior(rnd())
        { }
    } * root;
    int n;

    ll get_cnt(Node* v) {
        return (v ? v->cnt : 0);
    }

    void push(Node* v) {
        if(!v) return;
        if(v->l) {
            v->l->pushv += v->pushv;
            v->l->value += v->pushv;
        }
        if(v->r) {
            v->r->pushv += v->pushv;
            v->r->value += v->pushv;
        }
        v->pushv = 0;
    }

    void update(Node* v) {
        if(!v) return;
        ll _cnt = 1;
        if(v->l) {
            _cnt += v->l->cnt;
        }
        if(v->r) {
            _cnt += v->r->cnt;
        }
        v->cnt = _cnt;
    }

    void merge(Node*& v, Node* l, Node* r) {
        if(!l || !r) {
            v = l ? l : r;
            return;
        }
        push(l);
        push(r);
        if(l->prior <= r->prior) {
            merge(l->r, l->r, r);
            v = l;
        }else {
            merge(r->l, l, r->l);
            v = r;
        }
        update(v);
    }

    void split_by_cnt(Node* v, Node*& l, Node*& r, ll cnt) {
        if(!v) {
            l = r = 0;
            return;
        }
        push(v);
        if(get_cnt(v->l) + 1 <= cnt) {
            split_by_cnt(v->r, v->r, r, cnt - get_cnt(v->l) - 1);
            l = v;
        }else {
            split_by_cnt(v->l, l, v->l, cnt);
            r = v;
        }
        update(l);
        update(r);
    }

    // <=
    void split_by_value(Node* v, Node*& l, Node*& r, ll value) {
        if(!v) {
            l = r = 0;
            return;
        }
        push(v);
        if(v->value <= value) {
            split_by_value(v->r, v->r, r, value);
            l = v;
        }else {
            split_by_value(v->l, l, v->l, value);
            r = v;
        }
        update(l);
        update(r);
    }

public:

    Treap(int _n, ll value): n(_n) { // 0 .. n = {0, inf64, ...}
        root = new Node(0);
        for(int i = 1;i <= n;i++) {
            merge(root, root, new Node(value));
        }
    }

    ll get_value(int pos) {
        assert(0 <= pos && pos <= n);
        Node *nl, *nm, *nr;
        nl = nm = nr = 0;
        split_by_cnt(root, nl, nr, pos + 1);
        split_by_cnt(nl, nl, nm, pos);
        assert(nm);
        ll res = nm->value;
        merge(nl, nl, nm);
        merge(root, nl, nr);
        return res;
    }

    void insert(int pos, ll value) { // 0, 1, 2, ..., pos, insert, pos + 1, ...
        assert(-1 <= pos && pos <= n);
        Node* nl, *nr;
        nl = nr = 0;
        split_by_cnt(root, nl, nr, pos + 1);
        merge(nl, nl, new Node(value));
        merge(root, nl, nr);
    }

    void erase(int pos) { // 0, 1, 2, ..., pos - 1, pos, pos + 1, ...
        assert(0 <= pos && pos <= n);
        Node *nl, *nm, *nr;
        nl = nm = nr = 0;
        split_by_cnt(root, nl, nr, pos + 1);
        if(!pos) {
            root = nr;
        }else {
            split_by_cnt(nl, nl, nm, pos);
            merge(root, nl, nr);
        }
    }

    void add(int l, int r, ll value) {
        assert(0 <= l && l <= r && r <= n);
        Node *nl, *nm, *nr;
        nl = nm = nr = 0;
        split_by_cnt(root, nl, nr, r + 1);
        split_by_cnt(nl, nl, nm, l);
        assert(nm);
        nm->pushv += value;
        nm->value += value;
        merge(nl, nl, nm);
        merge(root, nl, nr);
    }

    int upper_bound(ll value) {
        Node *nl, *nr;
        nl = nr = 0;
        split_by_value(root, nl, nr, value);
        int res = get_cnt(nl);
        merge(root, nl, nr);
        return res;
    }

    void set_value(int pos, ll value) {
        assert(0 <= pos && pos <= n);
        Node *nl, *nm, *nr;
        nl = nm = nr = 0;
        split_by_cnt(root, nl, nr, pos + 1);
        split_by_cnt(nl, nl, nm, pos);
        assert(nm);
        nm->value = value;
        merge(nl, nl, nm);
        merge(root, nl, nr);
    }

    int get_left(int pos, ll value) {
        assert(0 <= pos && pos <= n);
        if(get_value(pos) <= value) return inf;
        Node *nl, *nm, *nr;
        nl = nm = nr = 0;
        split_by_cnt(root, nl, nr, pos + 1);
        split_by_value(nl, nl, nm, value);
        int res = get_cnt(nl);
        merge(nl, nl, nm);
        merge(root, nl, nr);
        return res;
    }
};
