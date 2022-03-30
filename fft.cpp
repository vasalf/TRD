template <int LOG>
struct FFT {
    vector<int> rev[LOG + 1];
    vector<base> G[LOG + 1];
    FFT() {
        int N = 1 << LOG;
        base root(cosl(2 * pi / N), sinl(2 * pi / N));
        base start = root;
        for (int lvl = LOG; lvl >= 0; lvl--, start = start * start) {
            int tot = 1 << lvl;
            G[lvl].resize(tot);
            base cur = 1;
            for (int i = 0; i < tot; i++, cur *= start)
                G[lvl][i] = cur;
        }
        for (int lvl = 1; lvl <= LOG; lvl++) {
            int tot = 1 << lvl;
            rev[lvl].resize(tot);
            for (int i = 1; i < tot; i++) {
                rev[lvl][i] = ((i & 1) << (lvl - 1)) | (rev[lvl][i >> 1] >> 1);
            }
        }
    }
    void fft(vector<base>& a, int sz, bool invert) {
        int n = 1 << sz;
        for (int j, i = 0; i < n; i++) {
            if ((j = rev[sz][i]) < i) {
                swap(a[i], a[j]);
            }
        }
        base f1, f2;
        for (int lvl = 0, len = 1; len < n; len <<= 1, lvl++) {
            for (int i = 0; i < n; i += (len << 1)) {
                for (int j = 0; j < len; j++) {
                    f1 = a[i + j];
                    f2 = a[i + j + len] * G[lvl + 1][j];
                    a[i + j] = f1 + f2;
                    a[i + j + len] = f1 - f2;
                }
            }
        }
        if (invert) {
            reverse(a.begin() + 1, a.end());
            for (int i = 0; i < n; i++)
                a[i] /= n;
        }
    }
    vector<ld> multiply(const vector<ld>& a, const vector<ld>& b) {
        vector<base> fa(a.begin(), a.end());
        vector<base> fb(b.begin(), b.end());
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
        for (int i = 0; i < SZ; i++)
            fa[i] = fa[i] * fb[i];
        fft(fa, sz, true);
        vector<ld> res(SZ);
        for (int i = 0; i < SZ; i++)
            res[i] = fa[i].real();
        return res;
    }
};