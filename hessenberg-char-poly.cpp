#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef long long ll;
typedef unsigned int ui;
typedef unsigned long long ull;
typedef long double ld;

const int inf = 1e9;
const ll inf64 = 1e18;

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

int main() {

#ifdef DEBUG
    freopen("input.txt", "r", stdin);
#endif

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> a(n, vector<int>(n)), b;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> a[i][j];

    auto _swap = [&](int i, int j) {
        if (i == j)
            return;
        swap(a[i], a[j]);
        for (int k = 0; k < n; k++)
            swap(a[k][i], a[k][j]);
    };

    // row[j] += coef * row[i]
    auto _add = [&](int i, int j, int coef) {
        for (int col = 0; col < n; col++)
            _inc(a[j][col], _mul(a[i][col], coef));
        coef = _sub(0, coef);
        for (int row = 0; row < n; row++)
            _inc(a[row][i], _mul(a[row][j], coef));
    };

    // transform a to the Hessenberg Form (all values under 2nd main diagonal are zeros)
    // O(n^3) gauss-like algo
    for (int j = 0; j + 1 < n; j++) {
        int i = j + 1;
        while (i < n && !a[i][j])
            i++;
        if (i == n)
            continue;
        if (j + 1 < i)
            _swap(i, j + 1);
        int coef = _binpow(a[j + 1][j], mod - 2);
        coef = _sub(0, coef);
        for (i = j + 2; i < n; i++) {
            if (!a[i][j])
                continue;
            _add(j + 1, i, _mul(coef, a[i][j]));
        }
    }

//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++)
//            cout << a[i][j] << " ";
//        cout << "\n";
//    }

    mt19937_64 rnd(42);

    set<int> points_set;
    for (int iter = 0; iter <= n; iter++) {
        while (1) {
            int x = (int(rnd() % mod) + mod) % mod;
            if (points_set.count(x))
                continue;
            points_set.insert(x);
            break;
        }
    }
    vector<int> points(points_set.begin(), points_set.end());
    vector<int> total(n + 1);

    for (int it = 0; it <= n; it++) {
        b = a;
        for (int i = 0; i < n; i++)
            _dec(b[i][i], points[it]);

        vector<int> f(n);
        f[0] = b[0][0];
        for (int i = 1; i < n; i++) {
            int ml = 1;
            for (int j = i; j >= 0; j--) {
                int sign = (j + i) % 2 ? mod - 1 : 1;
                _inc(f[i], _mul(_mul(ml, sign), _mul(j > 0 ? f[j - 1] : 1, b[j][i])));
                if (j > 0)
                    ml = _mul(ml, b[j][j - 1]);
            }
        }
        int det = f[n - 1];

        int cc = 1;
        for (int jit = 0; jit <= n; jit++) {
            if (it == jit)
                continue;
            cc = _mul(cc, _sub(points[it], points[jit]));
        }
        cc = _binpow(cc, mod - 2);
        cc = _mul(cc, det);

        vector<int> hlp = {1};
        for (int jit = 0; jit <= n; jit++) {
            if (it == jit)
                continue;
            vector<int> nxt(hlp.size() + 1);
            for (int i = 0; i < (int) hlp.size(); i++)
                _inc(nxt[i + 1], hlp[i]), _dec(nxt[i], _mul(hlp[i], points[jit]));
            hlp = nxt;
        }
        for (int i = 0; i <= n; i++)
            _inc(total[i], _mul(cc, hlp[i]));
    }

    for (int iter = 0; iter < q; iter++) {
        int x;
        cin >> x;

        int res = 0;
        for (int i = n; i >= 0; i--)
            res = _sum(_mul(res, x), total[i]);

        cout << res << " ";
    }
    cout << "\n";

    return 0;
}
