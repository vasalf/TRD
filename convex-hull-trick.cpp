struct Line {
    ll k, b;
    int type;
    ld x;

    Line(): k(0), b(0), type(0), x(0) { }

    Line(ll _k, ll _b, ld _x = 1e18, int _type = 0): k(_k), b(_b), x(_x), type(_type) { }

    bool operator<(const Line& other) const {
        if(type + other.type > 0) {
            return x < other.x;
        }else {
            return k < other.k;
        }
    }

    ld intersect(const Line& other) const {
        return ld(b - other.b) / ld(other.k - k);
    }

    ll get_func(ll x0) const {
        return k * x0 + b;
    }
 };

struct CHT {
    set< Line > qs;
    set< Line > :: iterator fnd, help;

    bool hasr(const set< Line > :: iterator& it) {
        return it != qs.end() && next(it) != qs.end();
    }

    bool hasl(const set< Line > :: iterator& it) {
        return it != qs.begin();
    }

    bool check(const set< Line > :: iterator& it) {
        if(!hasr(it)) return true;
        if(!hasl(it)) return true;
        return it->intersect(*prev(it)) < it->intersect(*next(it));
    }

    void update_intersect(const set< Line > :: iterator& it) {
        if(it == qs.end()) return;
        if(!hasr(it)) return;
        Line tmp = *it;
        tmp.x = tmp.intersect(*next(it));
        qs.insert(qs.erase(it), tmp);
    }

    void add_line(Line L) {
        if(qs.empty()) {
            qs.insert(L);
            return;
        }
        {
            fnd = qs.lower_bound(L);
            if(fnd != qs.end() && fnd->k == L.k) {
                if(fnd->b >= L.b) return;
                else qs.erase(fnd);
            }
        }
        fnd = qs.insert(L).first;
        if(!check(fnd)) {
            qs.erase(fnd);
            return;
        }
        while(hasr(fnd) && !check(help = next(fnd))) {
            qs.erase(help);
        }
        while(hasl(fnd) && !check(help = prev(fnd))) {
            qs.erase(help);
        }
        if(hasl(fnd)) {
            update_intersect(prev(fnd));
        }
        update_intersect(fnd);
    }

    ll get_max(ld x0) {
        if(qs.empty()) return -inf64;
        fnd = qs.lower_bound(Line(0, 0, x0, 1));
        if(fnd == qs.end()) fnd--;
        ll res = -inf64;
        int i = 0;
        while(i < 2 && fnd != qs.end()) {
            res = max(res, fnd->get_func(x0));
            fnd++;
            i++;
        }
        while(i-- > 0) fnd--;
        while(i < 2) {
            res = max(res, fnd->get_func(x0));
            if(hasl(fnd)) {
                fnd--;
                i++;
            }else {
                break;
            }
        }
        return res;
    }
};
