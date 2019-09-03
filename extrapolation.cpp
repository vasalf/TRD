int fact[N];
int rfact[N];

void precalc2() {
    fact[0] = 1;
    for (int i = 1; i < N; i++) {
        fact[i] = _mul(fact[i - 1], i);
    }
    rfact[N - 1] = _rev(fact[N - 1]);
    for (int i = N - 2; i >= 0; i--) {
        rfact[i] = _mul(rfact[i + 1], i + 1);
    }
}

int getMulOnSegment(int l, int r) {
    assert(l <= r);
    if (l == 0 && r == 0) return 1;
    if (r <= 0) {
        int res = getMulOnSegment(-r, -l);
        int cnt = r - l + 1;
        if (cnt % 2) {
            res = (-res % mod + mod) % mod;
        }
        return res;
    }
    if (l < 0) {
        int resl = getMulOnSegment(0, -l);
        if (l % 2) {
            resl = (-resl % mod + mod) % mod;
        }
        int resr = getMulOnSegment(0, r);
        return _mul(resl, resr);
    }
    assert(l >= 0);
    int res = fact[r];
    if (l > 0) {
        res = _mul(res, rfact[l - 1]);
    }
    return res;
}

vector<int> extrapolate(vector<int> y, int m) {
    vector<int> yy = y;
    int n = (int)y.size() - 1;
    for (int i = 0; i <= n; i++) {
        yy[i] = _mul(y[i], _rev(getMulOnSegment(i - n, i - 0)));
    }
    vector<int> ff(n + m + 1);
    for (int i = 1; i <= n + m; i++) {
        ff[i] = _mul(fact[i - 1], rfact[i]);
    }
    vector<int> ss = multiply(yy, ff);
    for (int i = 1; i <= m; i++) {
        int cc = getMulOnSegment(i, n + i);
        int Si = ss[n + i];
        y.push_back(_mul(cc, Si));
    }
    return y;
}
