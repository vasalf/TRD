const int mod = inf + 7;

inline int _sum(int a, int b) {
    return a + b < mod ? a + b : a + b - mod;
}

inline int _mul(int a, int b) {
    return (1ll * a * b) % mod;
}

inline int _sub(int a, int b) {
    return a >= b ? a - b : a - b + mod;
}

inline void _inc(int &x, int y) {
    if ((x += y) >= mod) {
        x -= mod;
    }
}

inline void _dec(int &x, int y) {
    if ((x -= y) < 0) {
        x += mod;
    }
}

inline int _binpow(int x, int pw) {
    int res = 1;
    int tmp = x;
    while (pw > 0) {
        if (pw & 1) {
            res = _mul(res, tmp);
        }
        tmp = _mul(tmp, tmp);
        pw >>= 1;
    }
    return res;
}

inline int _div(int a, int b) {
    return _mul(a, _binpow(b, mod - 2));
}

struct Polynom {
    vector<int> a = {1};
    void normalize() {
        while ((int)a.size() > 1 && !a.back()) {
            a.pop_back();
        }
    }
    Polynom() = default;
    Polynom(const vector<int> &b): a(b) {
        normalize();
    }
    Polynom operator * (int k) const {
        vector<int> res = a;
        for (int &x : res) {
            x = _mul(x, k);
        }
        return Polynom(res);
    }
    Polynom operator + (const Polynom &other) const {
        vector<int> res(max(a.size(), other.a.size()));
        for (int i = 0; i < (int) res.size(); i++) {
            if (i < (int) a.size()) _inc(res[i], a[i]);
            if (i < (int) other.a.size()) _inc(res[i], other.a[i]);
        }
        return Polynom(res);
    }
    Polynom operator - (const Polynom &other) const {
        return *this + (other * (mod - 1));
    }
    // O(n^2) !!!
    Polynom operator * (const Polynom &other) const {
        vector<int> res(a.size() + other.a.size());
        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = 0; j < (int)other.a.size(); j++) {
                _inc(res[i + j], _mul(a[i], other.a[j]));
            }
        }
        return Polynom(res);
    }
    Polynom operator % (const Polynom &other) const {
        Polynom res(a);
        int mem_rev_cf = _div(1, other.a.back());
        while (res.a.size() >= other.a.size()) {
            int cf = (mod - _mul(res.a.back(), mem_rev_cf)) % mod;
            int delta = res.a.size() - other.a.size();
            for (int i = 0; i < (int)other.a.size(); i++) {
                _inc(res.a[delta + i], _mul(cf, other.a[i]));
            }
            res.normalize();
        }
        return res;
    }
    static Polynom get_monom(int deg) {
        vector<int> res(deg + 1);
        res[deg] = 1;
        return Polynom(res);
    }
    void print() const {
        cout << "[ ";
        for (int x : a) {
            cout << x << " ";
        }
        cout << "]\n";
    }
    // [(*this) ** pw] % MOD
    Polynom binpow_mod(int pw, const Polynom &MOD) const {
        Polynom res = Polynom::get_monom(0);
        Polynom tmp(a);
        while (pw > 0) {
            if (pw & 1) {
                res = (res * tmp) % MOD;
            }
            tmp = (tmp * tmp) % MOD;
            pw >>= 1;
        }
        return res;
    }
};