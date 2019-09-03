#ifdef debug
#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>
#include "optimization.h"

using namespace std;

#define vec vector
#define ALL(x) begin(x), end(x)

typedef unsigned int ui;
typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< long long, long long > pll;

const int inf = 1e9;
const ll inf64 = 1e18;

mt19937 rnd(42);

class EulerTourTrees {
/*
graph - forest
1 .. n
get = is connected?
no memory leaks
1 <= n, q <= 10^5
0.7 sec
*/
private:
    struct Node {
        Node *l;
        Node *r;
        Node *p;
        int prior;
        int cnt;
        int rev;
        

        Node():
            l(nullptr), r(nullptr), p(nullptr),
            prior(rnd()), cnt(1), rev(0)
        { }

        ~Node() {
            delete l;
            delete r;
        }
    };

    void do_rev(Node *v) {
        if(v) v->rev ^= 1, swap(v->l, v->r);
    }

    int get_cnt(Node *v) const {
        return v ? v->cnt : 0;
    }

    void update(Node *v) {
        if(!v) return;
        v->cnt = 1 + get_cnt(v->l) + get_cnt(v->r);
        v->p = nullptr;
        if(v->l) v->l->p = v;
        if(v->r) v->r->p = v;
    }

    void push(Node *v) {
        if(!v) return;
        if(v->rev) {
            do_rev(v->l);
            do_rev(v->r);
            v->rev ^= 1;
        }
    }

    void merge(Node *& v, Node *l, Node *r) {
        if(!l || !r) {
            v = l ? l : r;
            return;
        }
        push(l);
        push(r);
        if(l->prior < r->prior) {
            merge(l->r, l->r, r);
            v = l;
        }else {
            merge(r->l, l, r->l);
            v = r;
        }
        update(v);
    }

    void split_by_cnt(Node *v, Node *& l, Node *& r, int x) {
        if(!v) {
            l = r = nullptr;
            return;
        }
        push(v);
        if(get_cnt(v->l) + 1 <= x) {
            split_by_cnt(v->r, v->r, r, x - get_cnt(v->l) - 1);
            l = v;
        }else {
            split_by_cnt(v->l, l, v->l, x);
            r = v;
        }
        update(l);
        update(r);
    }

    void push_path(Node *v) {
        if(!v) return;
        push_path(v->p);
        push(v);
    }

    int get_pos(Node *v) {
        push_path(v);
        int res = 0, ok = 1;
        while(v) {
            if(ok) res += get_cnt(v->l) + 1;
            ok = v->p && v->p->r == v;
            v = v->p;
        }
        return res;
    }

    Node *get_root(Node *v) const{
        while(v && v->p) v = v->p;
        return v;
    }

    Node *shift(Node *v) {
        if(!v) return v;
        int pos = get_pos(v);
        Node *nl = nullptr, *nr = nullptr;
        Node *root = get_root(v);
        split_by_cnt(root, nl, nr, pos - 1);
        do_rev(nl);
        do_rev(nr);
        merge(root, nl, nr);
        do_rev(root);
        return root;
    }

public:
    EulerTourTrees() = default;
    EulerTourTrees(int _n): n(_n) {
        ptr.resize(_n + 1);
        where_edge.resize(_n + 1);
    }

    bool get(int u, int v) const {
        if(u == v) return true;
        Node *ru = get_root(ptr[u].empty() ? nullptr : *ptr[u].begin());
        Node *rv = get_root(ptr[v].empty() ? nullptr : *ptr[v].begin());
        return ru && ru == rv;
    }

    void link(int u, int v) {
        Node *ru = shift(ptr[u].empty() ? nullptr : *ptr[u].begin());
        Node *rv = shift(ptr[v].empty() ? nullptr : *ptr[v].begin());
        Node *uv = new Node();
        Node *vu = new Node();
        ptr[u].insert(uv);
        ptr[v].insert(vu);
        where_edge[u][v] = uv;
        where_edge[v][u] = vu;
        merge(ru, ru, uv);
        merge(ru, ru, rv);
        merge(ru, ru, vu);
    }

    void cut(int u, int v) {
        Node *uv = where_edge[u][v];
        Node *vu = where_edge[v][u];
        ptr[u].erase(uv);
        ptr[v].erase(vu);

        Node *root = shift(uv);
        Node *nl = nullptr, *nm = nullptr, *nr = nullptr;
        int pos1 = get_pos(uv);
        int pos2 = get_pos(vu);

        if(pos1 < pos2) {
            split_by_cnt(root, nl, nr, pos2);
            split_by_cnt(nl, nl, vu, pos2 - 1);
            split_by_cnt(nl, nl, nm, pos1);
            split_by_cnt(nl, nl, uv, pos1 - 1);
            merge(nl, nl, nr);
        }else {
            split_by_cnt(root, nl, nr, pos1);
            split_by_cnt(nl, nl, uv, pos1 - 1);
            split_by_cnt(nl, nl, nm, pos2);
            split_by_cnt(nl, nl, vu, pos2 - 1);
            merge(nl, nl, nm);
        }

        delete uv;
        delete vu;
    }

    ~EulerTourTrees() {
        set< Node* > roots;
        for(int i = 1;i <= n;i++) {
            for(Node *v : ptr[i]) {
                roots.insert(get_root(v));
            }
        }
        for(Node *root : roots) {
            delete root;
        }
    }
private:
    int n = 0;
    vec< set< Node* > > ptr;
    vec< unordered_map< int, Node* > > where_edge; // ptr to node
};

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    int n = readInt();
    int m = readInt();
    char op[10];

    EulerTourTrees ett(n);

    for(int u, v, iter = 0;iter < m;iter++) {
        readWord(op);
        u = readInt();
        v = readInt();
        switch(op[0]) {
        case 'g':
            writeInt(int(ett.get(u, v)));
            break;
        case 'l':
            ett.link(u, v);
            break;
        case 'c':
            ett.cut(u, v);
            break;
        }
    }

    writeChar('\n');

    return 0;
}
