class AVL {
public:
    void insert(int x) {
        insert(root, x);
    }

    void erase(int x) {
        erase(root, x);
    }

    int kth(int k) {
        Node *v = kth(root, k);
        return v->key;
    }

    void print() {
        dfs(root);
        cout << "\n";
    }

    int size() const {
        return (root ? root->cnt : 0);
    }

    AVL():
        root(0)
    { }

private:

    struct Node {
        Node *l;
        Node *r;
        int key;
        int mark;
        int h;
        int cnt;

        Node(int x):
            l(0), r(0),
            key(x), mark(1), h(1), cnt(1)
        { }
    } *root, *v1;

    inline void update(Node *v) {
        v->h = 1 + max(v->l ? v->l->h : 0, v->r ? v->r->h : 0);
        v->cnt = v->mark + (v->l ? v->l->cnt : 0) + (v->r ? v->r->cnt : 0);
    }

    inline void assign_sons(Node *v, Node *l, Node *r) {
        if(v) {
            v->l = l;
            v->r = r;
        }
    }

    inline int get_h(Node *v) {
        return v ? v->h : 0;
    }

    inline int get_cnt(Node *v) const {
        return v ? v->cnt : 0;
    }

    inline bool check_balance(Node *v) {
        assert(v);
        return abs(get_h(v->l) - get_h(v->r)) <= 1;
    }

    Node* left_rotate(Node *v) {
        v1 = v->l;
        v->l = v1->r;
        v1->r = v;
        update(v);
        update(v1);
        return v1;
    }

    Node* right_rotate(Node *v) {
        v1 = v->r;
        v->r = v1->l;
        v1->l = v;
        update(v);
        update(v1);
        return v1;
    }

    void insert(Node *&v, int x) {
        if(!v) {
            v = new Node(x);
            return;
        }
        if(v->key == x) {
            if(v->mark) {
            }else {
                v->mark = 1;
                v->cnt++;
            }
            return;
        }
        if(x < v->key) {
            insert(v->l, x);
        }else {
            insert(v->r, x);
        }
        update(v);
        if(check_balance(v)) {
            return;
        }
        if(get_h(v->l) > get_h(v->r)) {
            assert(get_h(v->l) == get_h(v->r) + 2);
            if(v->l && get_h(v->l->l) + 1 < get_h(v->l)) {
                v->l = right_rotate(v->l);
            }
            v = left_rotate(v);
        }else {
            assert(get_h(v->r) == get_h(v->l) + 2);
            if(v->r && get_h(v->r->r) + 1 < get_h(v->r)) {
                v->r = left_rotate(v->r);
            }
            v = right_rotate(v);
        }
    }

    void erase(Node *v, int x) {
        if(!v) {
            return;
        }
        if(v->key == x) {
            if(v->mark) {
                v->mark = 0;
                v->cnt--;
            }
            return;
        }
        if(x < v->key) {
            erase(v->l, x);
        }else {
            erase(v->r, x);
        }
        update(v);
    }

    Node* kth(Node *v, int k) {
        if(v->mark && get_cnt(v->l) + v->mark == k) {
            return v;
        }
        if(get_cnt(v->l) + v->mark < k) {
            return kth(v->r, k - get_cnt(v->l) - v->mark);
        }else {
            return kth(v->l, k);
        }
    }

    void dfs(Node *v) {
        if(!v) return;
        dfs(v->l);
        if(v->mark) {
            cout << v->key << " ";
        }
        dfs(v->r);
    }
};
