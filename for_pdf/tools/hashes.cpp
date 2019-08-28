const int P1 = 37;
const int P2 = 43;
const int mod1 = inf + 7;
const int mod2 = inf + 21;
const pii pp = {P1, P2};
const pii zz = {0, 0};
const pii ee = {1, 1};

inline pii _sum(pii p1, pii p2) {
    return make_pair(
        p1.first  + p2.first  < mod1 ? p1.first  + p2.first  : p1.first  + p2.first  - mod1,
        p1.second + p2.second < mod2 ? p1.second + p2.second : p1.second + p2.second - mod2
    );
}

inline pii _sub(pii p1, pii p2) {
    return make_pair(
        p1.first  >= p2.first  ? p1.first  - p2.first  : p1.first  - p2.first  + mod1,
        p1.second >= p2.second ? p1.second - p2.second : p1.second - p2.second + mod2
    );
}

inline pii _mul(pii p1, pii p2) {
    return make_pair(
        (1ll * p1.first  * p2.first)  % mod1,
        (1ll * p1.second * p2.second) % mod2
    );
}

inline pii _code(char c) {
    return make_pair(
        c - 'a' + 1,
        c - 'a' + 1
    );
}

const int N = 1e6 + 5;

char s[N];
char t[N];
pii pw[N];
int n, m;
pii ht[N];

inline pii get_hash(int l, int r) {
    return _sub(ht[r], _mul(l ? ht[l - 1] : zz, pw[r - l + 1]));
}

int main() {

    pw[0] = ee;
    for(int i = 1;i < N;i++) {
        pw[i] = _mul(pw[i - 1], pp);
    }

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    scanf("%s %s", s, t);

    n = strlen(s);
    m = strlen(t);

    if(s[0] == '1') {
        for(int i = 0;i < n;i++) {
            s[i] = s[i] == '0' ? '1' : '0';
        }
    }

    for(int i = 0;i < m;i++) {
        if(i) ht[i] = ht[i - 1];
        ht[i] = _sum(_mul(ht[i], pp), _code(t[i]));
    }
    