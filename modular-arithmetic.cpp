const int mod = 998244353;

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