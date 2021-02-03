// Pollard

#include <bits/stdc++.h>

using namespace std;

#define vec vector
#define ALL(x) (x).begin(), (x).end()

typedef pair< int, int > pii;
typedef long long ll;

const int inf = 1e9;
ll const inf64 = 1ll * inf * inf;

const ll MAXX = 1000;
const int FERMA_ITER = 30;

inline ll sqr(ll n) {
    return n * n;
}

void check_small(ll n) {
    for(ll x = 1;sqr(x) <= n && x <= MAXX;x++) {
        if(x > 1 && n % x == 0) {
            cout << x << " " << n / x << "\n";
            exit(0);
        }else if(sqr(x + 1) > n) {
            cout << "IMPOSSIBLE\n";
            exit(0);
        }
    }
}

void check_square(ll n) {
    ll bl = 0;
    ll br = 3e9+1;
    ll bm;
    while(br - bl > 1) {
        bm = (bl + br) / 2;
        if(sqr(bm) <= n) {
            bl = bm;
        }else {
            br = bm;
        }
    }
    if(sqr(bl) == n && bl > 1) {
        cout << bl << " " << n / bl << "\n";
        exit(0);
    }
}

inline ll _mul(ll a, ll b, ll m) {
    static __int128 xa = 1;
    static __int128 xb = 1;
    static __int128 xm = 1;
    xa = a;
    xb = b;
    xm = m;
    return ll(xa * xb % xm);
}

inline ll _binpow(ll x, ll p, ll m) {
    static ll res = 1;
    static ll tmp = 1;
    res = 1;
    tmp = x;
    while(p > 0) {
        if(p & 1ll) {
            res = _mul(res, tmp, m);
        }
        tmp = _mul(tmp, tmp, m);
        p >>= 1;
    }
    return res;
}

mt19937_64 next_rand(42);

ll gcd(ll x, ll y) {
    return !x ? y : gcd(y % x, x);
}

bool is_prime(ll n) {

    ll a, g;

    for(int iter = 0;iter < FERMA_ITER;iter++) {
        a = next_rand() % (n - 2);
        if(a < 0) a += n - 2;
        a += 2;
        assert(1 < a && a < n);
        g = gcd(a, n);
        if(g != 1) {
            cout << g << " " << n / g << "\n";
            exit(0);
        }
        if(_binpow(a, n - 1, n) != 1) {
            return false;
        }
    }

    return true;
}

inline ll _func(ll x, ll n) {
    static ll result = 1;
    result = _mul(x, x, n);
    return result + 1 < n ? result + 1 : 0;
}

ll diff(ll x, ll y, ll mod) {
    if (x - y < 0) return x - y + mod;
    else return x - y + mod;
}

ll pollard_po(ll n) {
    const int POLLARD_PO_ITER = 5 + 3 * pow(n, 0.25);
    const int MAGIC_LOG = 20;
    while(true) {
        ll x = next_rand() % n;
        for (int i = 0; i < POLLARD_PO_ITER; i++) {
            x = _mul(x, x, n) + 1;
        }
        ll y = _mul(x, x, n) + 1;
        for (int i = 0; i < POLLARD_PO_ITER / MAGIC_LOG; i++) {
            ll g = 1;
            for (int j = 0; j < MAGIC_LOG; j++) {
                g = _mul(g, diff(x, y, n), n);
                y = _mul(y, y, n) + 1;
            }
            ll res = __gcd(g, n);
            if (res != 1 && res != n)
                return res;
        }
    }
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    ll n;

    cin >> n;

    check_small(n);
    check_square(n);

    if(is_prime(n)) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    pollard_po(n);

    return 0;
}
//!!!

namespace FACTORIZE {
    const ll MAXX = 1000;
    const int FERMA_ITER = 30;
    //const int POLLARD_PO_ITER = 10000;
    int POLLARD_PO_ITER;

    inline ll sqr(ll n) {
        return n * n;
    }

    ll check_small(ll n) {
        for(ll x = 1;sqr(x) <= n && x <= MAXX;x++) {
            if(x > 1 && n % x == 0) {
                return x;
            }else if(sqr(x + 1) > n) {
                return -1;
            }
        }
        return -1;
    }

    ll check_square(ll n) {
        ll bl = 0;
        ll br = 3e9+1;
        ll bm;
        while(br - bl > 1) {
            bm = (bl + br) / 2;
            if(sqr(bm) <= n) {
                bl = bm;
            }else {
                br = bm;
            }
        }
        if(sqr(bl) == n && bl > 1) {
            return bl;
        }else {
            return -1;
        }
    }

    inline ll MUL(ll val, ll n, ll mod) {
        long long int q=((double)val*(double)n/(double)mod);
        long long int res=val*n-mod*q;
        res=(res%mod+mod)%mod;
        return res;
    }

    inline ll _mul(ll a, ll b, ll m) {
        static __int128 xa = 1;
        static __int128 xb = 1;
        static __int128 xm = 1;
        xa = a;
        xb = b;
        xm = m;
        return ll(xa * xb % xm);
    }

    inline ll _binpow(ll x, ll p, ll m) {
        static ll res = 1;
        static ll tmp = 1;
        res = 1;
        tmp = x;
        while(p > 0) {
            if(p & 1ll) {
                res = _mul(res, tmp, m);
            }
            tmp = _mul(tmp, tmp, m);
            p >>= 1;
        }
        return res;
    }

    mt19937_64 next_rand(42);

    ll gcd(ll x, ll y) {
        return !x ? y : gcd(y % x, x);
    }

    bool is_prime(ll n) {

        if(n <= 1) return false;
        if(n == 2) return true;

        ll a, g;

        for(int iter = 0;iter < FERMA_ITER;iter++) {
            a = next_rand() % (n - 2);
            if(a < 0) a += n - 2;
            a += 2;
            assert(1 < a && a < n);
            g = gcd(a, n);
            if(g != 1) {
                return false;
            }
            if(_binpow(a, n - 1, n) != 1) {
                return false;
            }
        }

        return true;
    }

    inline ll _func(ll x, ll n) {
        static ll result = 1;
        result = _mul(x, x, n);
        return result + 1 < n ? result + 1 : 0;
    }


    ll diff(ll x, ll y, ll mod) {
        if (x - y < 0) return x - y + mod;
        else return x - y + mod;
    }

    ll pollard_po(ll n) {
        const int POLLARD_PO_ITER = 5 + 3 * pow(n, 0.25);
        const int MAGIC_LOG = 20;
        while(true) {
            ll x = next_rand() % n;
            for (int i = 0; i < POLLARD_PO_ITER; i++) {
                x = _mul(x, x, n) + 1;
            }
            ll y = _mul(x, x, n) + 1;
            for (int i = 0; i < POLLARD_PO_ITER / MAGIC_LOG; i++) {
                ll g = 1;
                for (int j = 0; j < MAGIC_LOG; j++) {
                    g = _mul(g, diff(x, y, n), n);
                    y = _mul(y, y, n) + 1;
                }
                ll res = __gcd(g, n);
                if (res != 1 && res != n)
                    return res;
            }
        }
    }

    ll get_div(ll n) {
        ll res;

        res = check_small(n);

        if(res != -1) {
            return res;
        }

        res = check_square(n);

        if(res != -1) {
            return res;
        }

        if(is_prime(n)) {
            return n;
        }

        return pollard_po(n);
    }
}
