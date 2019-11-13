#include <bits/stdc++.h>

using namespace std;

typedef unsigned int ui;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef long double ld;

const int inf = 1e9;
const ll inf64 = 1e18;

struct Tandem {
    int l, r, k; // [l, l + 2 * k) [l + 1, l + 1 + 2 * k) [l + 2, l + 2 + 2 * k), ..., [r, r + 2 * k)
};

vector<int> z_func(const string& s) {
    int n = (int) s.size();
    vector<int> z(n);
    for (int l = 0, r = -1, i = 1; i < n; i++) {
        int k = i > r ? 0 : min(z[i - l], r - i + 1);
        while (i + k < n && s[i + k] == s[k]) k++;
        z[i] = k;
        if (i + k - 1 > r) {
            r = i + k - 1;
            l = i;
        }
    }
    return z;
}

const int SIZE = (1000006) * 30;
const int MAXL = (1000006) * 4;

Tandem tandems[SIZE], hlp[MAXL];
int tsz;

void rec(const string& s, int L, int R) {
    if (R - L + 1 <= 1) {
        return;
    }

    int M = (L + R) / 2;
    
    rec(s, L, M);
    rec(s, M + 1, R);

    int nu = M - L + 1;
    int nv = R - M;

    string vu = s.substr(M + 1, nv) + "#" + s.substr(L, nu);
    string urvr = vu; reverse(urvr.begin(), urvr.end());

    vector<int> z1 = z_func(urvr);
    vector<int> z2 = z_func(vu);

    for (int x = L; x <= R; x++) {
        if (x <= M) {
            int k = M + 1 - x;
            int k1 = L < x ? z1[nu - x + L] : 0;
            int k2 = z2[nv + 1 + x - L];
            int lsh = max(0, k - k2);
            int rsh = min(k1, k - 1);
            if (lsh <= rsh) {
                tandems[tsz++] = {x - rsh, x - lsh, k};
            }
        } else {
            int k = x - M;
            int k1 = x < R ? z2[x - M] : 0;
            int k2 = z1[nu + nv - x + M + 1];
            int lsh = max(1, k - k1);
            int rsh = min(k2, k - 1);
            if (lsh <= rsh) {
                tandems[tsz++] = {x - rsh + 1 - k, x - lsh + 1 - k, k};
            }
        }
    }
}

void compress() { // O(n*log(n)*log(n)) can be replace with count sort (O(n*log(n))) BE careful with ML !!!
    // O(n*log(n)) --> O(n)
    sort(tandems, tandems + tsz, [](const Tandem& t1, const Tandem& t2) {
        return t1.k < t2.k || (t1.k == t2.k && t1.l < t2.l);
    });
    int hlp_sz = 0;
    for (int i = 0; i < tsz; i++) {
        int j = i;
        while (j + 1 < tsz && tandems[i].k == tandems[j + 1].k && tandems[j].r + 1 == tandems[j + 1].l) {
            j++;
        }
        hlp[hlp_sz++] = {tandems[i].l, tandems[j].r, tandems[j].k};
        i = j;
    }
    memcpy(tandems, hlp, sizeof(Tandem) * hlp_sz);
    tsz = hlp_sz;
}

void main_lorentz(const string &s) {
    // n = 10^6  time = 1.8 sec MEM = nlog(n) * 12 bytes
    int n = (int) s.size();
    tsz = 0;
    rec(s, 0, n - 1);
    compress();
}

void WORK() {
    string s(1000000, 'a');
    main_lorentz(s);
}

vector<Tandem> brute(const string& s) {
    vector<Tandem> tandems;
    int n = (int) s.size();
    for (int l = 0; l < n; l++) {
        for (int k = 1; l + 2 * k <= n; k++) {
            if (s.substr(l, k) == s.substr(l + k, k)) {
                tandems.push_back({l, l, k});
            }
        }
    }
    return tandems;
}

void TEST() {
    auto get_segs = [](const vector<Tandem>& ts) -> vector<pii> {
        vector<pii> res;
        for (const auto& t : ts) {
            for (int l = t.l; l <= t.r; l++) {
                res.emplace_back(l, l + 2 * t.k - 1);
            }
        }
        sort(res.begin(), res.end());
        int s1 = (int) res.size();
        int s2 = unique(res.begin(), res.end()) - res.begin();
        assert(s1 == s2);
        return res;
    };
    mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
    while (1) {
        int n = rnd() % 100 + 1;
        string s(n, '?');
        for (int i = 0; i < n; i++) {
            s[i] = rnd() % 26 + 'a';
        }
        main_lorentz(s);
        vector<Tandem> fs(tandems, tandems + tsz);
        vector<Tandem> sl = brute(s);

        vector<pii> fs_segs = get_segs(fs);
        vector<pii> sl_segs = get_segs(sl);

        if (fs_segs == sl_segs) {
            cout << "OK (" << s << ") " << (int) fs_segs.size() << "\n";
        } else {
            cout << "WA\n";
            cout << "exp\n"; for (pii p : sl_segs) cout << "[" << p.first << " .. " << p.second << "]\n";
            cout << "fnd\n"; for (pii p : fs_segs) cout << "[" << p.first << " .. " << p.second << "]\n";
            cout << "Fast Tandems\n";
            for (const auto& t : fs) {
                cout << "[" << t.l << ".." << t.r << "], k = " << t.k << "\n";
            }
            cout << "\n\n" << s << "\n";
            break;
        }
    }
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    // WORK();
    TEST();

    return 0;
}