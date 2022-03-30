template<int mod, int root, int LOG>
struct FFT {
//    const int mod = 998244353;
//    const int root = 31;
//    const int LOG = 23;
    vector<int> G[LOG + 1];
    vector<int> rev[LOG + 1];    
    FFT() {
        for (int start = root, lvl = LOG; lvl >= 0; lvl--, start = mul(start, start)) {
            int tot = 1 << lvl;
            G[lvl].resize(tot);
            for (int cur = 1, i = 0; i < tot; i++, cur = mul(cur, start)) {
                G[lvl][i] = cur;
            }
        }
        for (int lvl = 1; lvl <= LOG; lvl++) {
            int tot = 1 << lvl;
            rev[lvl].resize(tot);
            for (int i = 1; i < tot; i++) {
                rev[lvl][i] = ((i & 1) << (lvl - 1)) | (rev[lvl][i >> 1] >> 1);
            }
        }
    }
    void fft(vector<int>& a, int sz, bool invert) {
        int n = 1 << sz;
        for (int j, i = 0; i < n; i++) {
            if ((j = rev[sz][i]) < i)
                swap(a[i], a[j]);
        }
        for (int f1, f2, lvl = 0, len = 1; len < n; len <<= 1, lvl++) {
            for (int i = 0; i < n; i+= (len << 1)) {
                for (int j = 0; j < len; j++) {
                    f1 = a[i + j];
                    f2 = mul(a[i + j + len], G[lvl + 1][j]);
                    a[i + j] = sum(f1, f2);
                    a[i + j + len] = sub(f1, f2);
                }
            }
        }
        if (invert) {
            reverse(a.begin() + 1, a.end());
            int rn = binpow(n, mod - 2);
            for (int i = 0; i < n; i++) {
                a[i] = mul(a[i], rn);
            }
        }
    }
    vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        vector<int> fa(a.begin(), a.end());
        vector<int> fb(b.begin(), b.end());
        int n = (int) a.size();
        int m = (int) b.size();
        int maxnm = max(n, m), sz = 0;
        while ((1 << sz) < maxnm)
            sz++;
        sz++;
        fa.resize(1 << sz);
        fb.resize(1 << sz);
        fft(fa, sz, false);
        fft(fb, sz, false);
        int SZ = 1 << sz;
        for (int i = 0; i < SZ; i++) {
            fa[i] = mul(fa[i], fb[i]);
        }
        fft(fa, sz, true);
        return fa;
    }
    int sum(int x, int y) {
        return x + y < mod ? x + y : x + y - mod;
    }
    int sub(int x, int y) {
        return x >= y ? x - y : x - y + mod;
    }
    int mul(int x, int y) {
        return (1ll * x * y) % mod;
    }
    int mul(const vector<int>& a) {
        int res = 1;
        for (const auto& x : a)
            res = mul(res, x);
        return res;
    }
    void inc(int& x, int y) {
        if ((x += y) >= mod)
            x -= mod;
    }
    void dec(int& x, int y) {
        if ((x -= y) < 0)
            x += mod;
    }
    int binpow(int x, int pw) {
        int res = 1, tmp = x;
        while (pw > 0) {
            if (pw & 1) res = mul(res, tmp);
            tmp = mul(tmp, tmp);
            pw >>= 1;
        }
        return res;
    }
};