// Cipolla's algorithm

#include <bits/stdc++.h>

using namespace std;

#define vec vector
#define ALL(x) (x).begin(), (x).end()

typedef unsigned int ui;
typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< long long, long long > pll;

const int inf = 1e9;
const ll inf64 = 1e18;

inline ll _sum(ll a, ll b, ll m) {
    return a + b < m ? a + b : a + b - m;
}

inline ll _sub(ll a, ll b, ll m) {
    return a >= b ? a - b : a - b + m;
}

inline ll _mul(ll a, ll b, ll m) {
    return (a * b) % m;
}

struct gauss_number {
    ll w, p;
    ll x, y;

    gauss_number(): w(0), p(2), x(0), y(0) { }

    gauss_number(ll _w, ll _p, ll _x, ll _y): w(_w), p(_p), x(_x), y(_y) {
        assert(p > 0);
        w %= p;
        if(w < 0) w += p;
        x %= p;
        if(x < 0) x += p;
        y %= p;
        if(y < 0) y += p;
    }

    gauss_number(const gauss_number& o):
        w(o.w), p(o.p), x(o.x), y(o.y)
    { }

    gauss_number operator+(const gauss_number& o) const {
        return gauss_number(w, p, _sum(x, o.x, p), _sum(y, o.y, p));
    }

    gauss_number operator-() const {
        return gauss_number(w, p, !x ? x : p - x, !y ? y : p - y);
    }

    gauss_number operator-(const gauss_number& o) const {
        return *this + (-o);
    }

    gauss_number operator*(const gauss_number& o) const {
        return gauss_number(w, p,
            _sum(_mul(x, o.y, p), _mul(y, o.x, p), p),
            _sum(_mul(y, o.y, p), _mul(x, _mul(o.x, w, p), p), p)
        );
    }
};

ll binpow(ll x, ll p, ll m) {
    ll res = 1 % m;
    ll tmp = x % m;
    if(res < 0) res += m;
    if(tmp < 0) tmp += m;
    while(p > 0) {
        if(p & 1) {
            res = _mul(res, tmp, m);
        }
        tmp = _mul(tmp, tmp, m);
        p >>= 1;
    }
    return res;
}

gauss_number gauss_pow(gauss_number x, ll p) {
    gauss_number res(x.w, x.p, 0, 1);
    gauss_number tmp(x);
    while(p > 0) {
        if(p & 1) {
            res = res * tmp;
        }
        tmp = tmp * tmp;
        p >>= 1;
    }
    return res;
}

ll find_solution(ll p, ll a) { // x^2 = a (mod p), x = ?, p is prime
    assert(0ll <= a && a < p);
    if(a == 0 || p == 2) return a;
    if(binpow(a, (p - 1) / 2, p) == p - 1) return -1ll;

    mt19937_64 rnd(42);
    ll k;

    gauss_number e(a, p, 0, 1);

    while(1) {
        k = rnd() % p;
        if(k < 0) k += p;

        gauss_number y(a, p, 1, k);
        y = gauss_pow(y, (p - 1) / 2);
        y.y = _sub(y.y, 1, p);

        {
            ll re = _mul(y.y, binpow(y.x, p - 2, p), p);
            if(_mul(re, re, p) == a) {
                return re;
            }
        }
    }
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    vec< ll > arr = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 31, inf + 7, inf + 9, inf + 21,
        1630627907,
        1799892467,
        1602046021,
        1354656307,
        1367897779,
        1528537499,
        1720014367,
        1452760943,
        1383048377,
        1344775469,
        1433084459,
        1889636123,
        1207413281,
        1399712117,
        1947188819,
        1904647669,
        1029841451,
        1600554409,
        1206541447,
        1974992273
    };

    mt19937_64 rnd(42);

    for(ll p : arr) {
        ll x = rnd() % p;
        ll a = _mul(x, x, p);
        cout << "x^2 = " << a << " (mod " << p << "), x = " << find_solution(p, a) << "\n";
    }

    return 0;
}
