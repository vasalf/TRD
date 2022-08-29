#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ui = unsigned int;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<long long, long long>;

const int inf = 1e9;
const ll inf64 = 1e18;

namespace FACTORIZATION {
    __extension__ typedef __int128 int128;
    constexpr ll mmul(ll a, ll b, ll mod) {
        return (static_cast<int128>(a) * b) % mod;
    }
    template <typename T, typename K>
    constexpr T mpow(T a, K n, T mod) {
        T res = 1;
        for (T now = a; n;
             n >>= 1, now = mmul(now, now, mod)) {
            if (n & 1) res = mmul(res, now, mod);
        }
        return res;
    }
    inline bool miller_rabin_det(ll n) {
        static const ll bases[] = {
                2, 3, 5, 7, 11,
                13, 17, 19, 23}; // works for n < 3.8e18
// static const int bases[] = {2, 3, 5, 7, 11,
// 13, 17, 19, 23, 29, 31, 37}; // n < 3.1e23
        if (n <= 2) return (n == 2);
        if (n % 2 == 0) return false;
        ll d = n - 1;
        while (!(d & 1))
            d >>= 1;
        for (ll a : bases) {
            if (a == n) return true;
            a = mpow(a, d, n);
            if (a == 1) continue;
            for (ll nd = d; nd != n - 1 && a != n - 1;
                 nd <<= 1)
                a = mmul(a, a, n);
            if (a != n - 1) return false;
        }
        return true;
    }
    inline ll pollard(ll n) {
        static std::mt19937_64 gen;
        static const int LOG = 50;
        if (n <= 1 || miller_rabin_det(n)) return 1;
        if (!(n & 1)) return 2;
        auto f = [n](ll x) {
            return mmul(x, x, n) + 1;
        }; // it is ok if 0 == n
        for (int st = 2, lg = 0;; st = gen() % n) {
            ll cur = 1;
            for (ll x = st, y = f(st); x != y;
                 x = f(x), y = f(f(y))) {
                if (ll c = mmul(cur, std::abs(x - y), n); c)
                cur = c;
                if (lg++ == LOG) {
                    lg = 0;
                    if (ll val = std::__gcd(cur, n); val != 1)
                    return val;
                }
            }
        }
        return 1;
    }
}

const int mod = 998'244'353;

int mul(int x, int y) {
    return (1ll * x * y) % mod;
}

int sum(int x, int y) {
    return x + y < mod ? x + y : x + y - mod;
}

void inc(int& x, int y) {
    if ((x += y) >= mod)
        x -= mod;
}

void dec(int& x, int y) {
    if ((x -= y) < 0) x += mod;
}

int binpow(int x, int pw) {
    int res = 1;
    for (int tmp = x; pw > 0; pw >>= 1, tmp = mul(tmp, tmp))
        if (pw & 1) res = mul(res, tmp);
    return res;
}

ll MUL(ll x, ll y, ll p) {
    return (__int128(x) * y) % p;
}

ll BINPOW(ll x, ll pw, ll p) {
    ll res = 1;
    for (ll tmp = x; pw > 0; pw >>= 1, tmp = MUL(tmp, tmp, p))
        if (pw & 1) res = MUL(res, tmp, p);
    return res;
}

ll find_sqrt(ll p) {
    assert(1 <= p && p <= inf64);
    ll bl = 0, br = inf + 10, bm;
    while (br - bl > 1) {
        bm = bl + (br - bl) / 2;
        if (bm * bm <= p) bl = bm;
        else br = bm;
    }
    return bl;
}

ll rec(ll pos, ll left_len, ll left_cost,
       ll right_len, ll right_cost, ll k) {
    if (!k || !right_len) return pos;
    if (pos >= right_len) {
        ll t = (left_len - pos + right_len - 1) /
               right_len;
        if (t * right_cost + left_cost > k)
            return pos;
        pos += t * right_len - left_len;
        k -= (t * right_cost + left_cost);
    }
    ll nxt_left_len = left_len % right_len;
    ll nxt_left_cost =
            (left_len / right_len) * right_cost +
            left_cost;
    if (nxt_left_len == 0) return pos;
    {
        ll t = pos / nxt_left_len;
        if (t * nxt_left_cost > k)
            return pos -
                   nxt_left_len * (k / nxt_left_cost);
        k -= t * nxt_left_cost;
        pos -= t * nxt_left_len;
    }
    return rec(pos, nxt_left_len, nxt_left_cost,
               right_len % nxt_left_len,
               (right_len / nxt_left_len) *
               nxt_left_cost +
               right_cost,
               k);
}
// finds (nw_st + step * x) % mod --> min, 0 <= x
// <= bound
ll euclid(ll nw_st, ll step, ll mod, ll bound) {
    return rec(nw_st, mod, 0, step, 1, bound);
}

pll find_sums_of_two_squares_for_prime(ll p) {
    // O(poly log p) a^2 + b^2 = p, 1 <= a < b <= sqrt(p)
    assert(p % 4 == 1); // p mod 4 = 1 (Ferma th)
    ll x = 1;
    { // find x: x^2=-1(mod p)
        ll pw = (p - 1) / 2;
        while (BINPOW(x, pw, p) != p - 1) x++;
        x = BINPOW(x, (p - 1) / 4, p);
        assert(MUL(x, x, p) == p - 1);
    }
    ll y = BINPOW(x, p - 2, p);
    ll sqrt_p = find_sqrt(p);
    // 0 <= b' < sqrt_p
    // (y + y * b') % p -> min
    // b = b' + 1
    ll a = euclid(y, y, p, sqrt_p - 1);
    ll b = (__int128(a) * x) % p;
    assert(a != b);
    if (a > b) swap(a, b);
    return {a, b};
}

const int R2 = binpow(2, mod - 2);

ll gcd(ll a, ll b) {
    return !a ? b : gcd(b % a, a);
}

int calc(ll a, ll b) {
    int res = 0;
    inc(res, mul(a % mod, b % mod));
    inc(res, 2);
    ll B = (a + b + gcd(a, b)) % mod;
    dec(res, B);
    res = mul(res, R2);
    inc(res, a % mod);
    inc(res, b % mod);
    dec(res, 1);
    return res;
}

struct output {
    int res;

    void print() {
        cout << res << "\n";
    }

    bool operator == (const output& o) const {
        return res == o.res;
    }
};

vector<pll> factorize(ll n) {
    vector<ll> ar;
    function<void(ll)> rec = [&](ll nn) {
        if (nn <= 1) return;
        ll p = FACTORIZATION::pollard(nn);
        if (p == 1) {
            ar.push_back(nn);
            return;
        }
        rec(p);
        rec(nn / p);
    };
    rec(n);
    sort(ar.begin(), ar.end());
    vector<pll> res;
    for (int i = 0; i < (int) ar.size(); i++) {
        int j = i;
        while (j < (int) ar.size() && ar[j] == ar[i]) j++; j--;
        res.emplace_back(ar[i], j - i + 1);
        i = j;
    }
    return res;
}

vector<pll> find_all_sums_of_two_squares_slow(ll n) {
    vector<pll> res;
    for (ll a = 0; a * a <= n; a++) {
        for (ll b = a; b * b <= n; b++) {
            if (a * a + b * b == n)
                res.emplace_back(a, b);
        }
    }
    return res;
}

vector<pll> find_all_sums_of_two_squares(ll n) {
    assert(n >= 1);
    if (n == 1) return {{0, 1}};
    // find all (0 <= a <= b <= n: a^2+b^2=n^2)
    // gaussian numbers multiplication
    // (complex values with integer coordinates)
    vector<pll> ds = factorize(n);
    for (auto& [p, c] : ds) {
        if (p % 4 == 3 && c % 2 == 1) return {};
    }
    vector<pll> res, add, nxt;
    for (auto [p, c] : ds) {
        add.clear();
        if (p == 2) {
            while (c--) add.emplace_back(1, 1);
        } else if (p % 4 == 3) {
            assert(c % 2 == 0);
            c /= 2;
            while (c--) add.emplace_back(0, p);
        } else {
            auto [a, b] = find_sums_of_two_squares_for_prime(p);
            while (c--) add.emplace_back(a, b);
        }
        for (auto [aa, bb] : add) {
            if (res.empty()) {
                res.emplace_back(aa, bb);
            } else {
                nxt.clear();
                for (auto [cc, dd]: res) {
                    for (int it = 0; it < 2; it++, swap(cc, dd)) {
                        ll A = abs(aa * cc - bb * dd);
                        ll B = abs(aa * dd + bb * cc);
                        if (A > B) swap(A, B);
                        nxt.emplace_back(A, B);
                    }
                }
                for (auto [cc, dd] : nxt) res.emplace_back(cc, dd);
                swap(res, nxt);
                sort(res.begin(), res.end());
                res.erase(unique(res.begin(), res.end()), res.end());
            }
        }
    }
    return res;
}

struct input {
    ll n, m;

    input() = default;

    void read() {
        cin >> n >> m;
    }

    void print() {
        cout << n << " " << m << "\n";
    }

    void gen() {
        static mt19937_64 rnd(42);
        const ll MAXN = 10;
        n = rnd() % MAXN + 1;
        m = rnd() % MAXN + 1;
    }

    void gen_max_test() {

    }

    output fast() {
        int res = 0;
        inc(res, mul(n % mod, m % mod));
        vector<pll> ar = find_all_sums_of_two_squares(n);
        for (auto [a, b] : ar) {
            assert(0 <= a && a < b && b <= n);
            if (!a) continue;
            __int128 xx = __int128(m) * __int128(b);
            __int128 yy = __int128(m) * __int128(a);
            if (xx % n || yy % n) continue;
            ll x = xx / n;
            ll y = yy / n;
            int tr1 = calc(a, b);
            int tr2 = calc(x, y);
            int tr = sum(tr1, tr2);
            int tmp = mul((a + x) % mod, (y + b) % mod);
            dec(tmp, mul(tr, 2));
            tmp = mul(tmp, 2);
            inc(res, tmp);
        }
        if (n != m) res = mul(res, 2);
        return output{res};
    }

#ifdef DEBUG
    output slow() {
        return output();
    }
#endif
};

void test_case() {
    input in;
    in.read();
    output res = in.fast();
    res.print();
}

void work() {
    int t;
    cin >> t;
    while (t--)
        test_case();
}

#ifdef DEBUG
void test() {
    for (int t = 1;;t++) {
        input in;
        in.gen();
        input in_fs = in;
        input in_sl = in;
        output fs = in_fs.fast();
        output sl = in_sl.slow();
        if (fs == sl) {
            cout << "OK" << endl;
            fs.print();
            cout << "\n=========" << endl;
        } else {
            cout << "WA " << t << "\n";
            cout << "exp\n";
            sl.print();
            cout << "\n=========\n";
            cout << "fnd\n";
            fs.print();
            cout << "\n=========\n";
            in.print();
            break;
        }
    }
}
#endif

#ifdef DEBUG
void max_test() {
    input in;
    in.gen_max_test();
    input in_fs = in;
    output fs = in_fs.fast();
    fs.print();
}
#endif

int main() {

#ifdef DEBUG
    freopen("input.txt", "r", stdin);
#endif

    ios_base::sync_with_stdio(0);
    cin.tie(0);

//    work();
//    test();
//    max_test();

//    for (ll n = 1; n <= 1000; n++) {
//        assert(find_all_sums_of_two_squares(n) == find_all_sums_of_two_squares_slow(n));
//    }
//    cout << "OK" << endl;

    mt19937_64 rnd(42);
    for (int it = 0; it < 100; it++) {
        ll n = rnd() % inf + 1;
        vector<pll> fs = find_all_sums_of_two_squares(n);
        vector<pll> sl = find_all_sums_of_two_squares_slow(n);
        assert(fs == sl);
        cout << "OK(" << n << ")" << endl;
    }

    return 0;
}
