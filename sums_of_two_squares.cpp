pll find_sums_of_two_squares_for_prime(ll p) {
    // O(poly log p) a^2 + b^2 = p, 1 <= a < b <= sqrt(p)
    assert(p % 4 == 1); // p mod 4 = 1 (Ferma th)
    ll x = 1; // find x: x^2=-1(mod p)
    while (BINPOW(x, (p - 1) / 2, p) != p - 1) x++;
    x = BINPOW(x, (p - 1) / 4, p);
    assert(MUL(x, x, p) == p - 1);
    ll y = BINPOW(x, p - 2, p), sqrt_p = find_sqrt(p);
    // 0 <= b' < sqrt_p  ; (y + y * b') % p -> min ; b = b' + 1
    ll a = euclid(y, y, p, sqrt_p - 1);
    ll b = (__int128(a) * x) % p;
    assert(a != b); if (a > b) swap(a, b);
    return {a, b};}
vector<pll> find_all_sums_of_two_squares(ll n) {
    assert(n >= 1);
    if (n == 1) return {{0, 1}};
    // find all (0 <= a <= b <= n: a^2+b^2=n^2)
    // gaussian numbers multiplication
    // (complex values with integer coordinates)
    vector<pll> ds = factorize(n);
    for (auto& [p, c] : ds)
        if (p % 4 == 3 && c % 2 == 1) return {};
    vector<pll> res, add, nxt;
    for (auto [p, c] : ds) {
        add.clear();
        if (p == 2) while (c--) add.emplace_back(1, 1);
        else if (p % 4 == 3) {
            c /= 2; while (c--) add.emplace_back(0, p); } else {
            auto [a, b] = find_sums_of_two_squares_for_prime(p);
            while (c--) add.emplace_back(a, b); }
        for (auto [aa, bb] : add) {
            if (res.empty()) { res.emplace_back(aa, bb);
            } else { nxt.clear(); for (auto [cc, dd]: res)
                    for (int it = 0; it < 2; it++, swap(cc, dd)) {
                        ll A = abs(aa * cc - bb * dd), B = abs(aa * dd + bb * cc);
                        if (A > B) swap(A, B); nxt.emplace_back(A, B);}
                for (auto [cc, dd] : nxt) res.emplace_back(cc, dd);
                swap(res, nxt); sort(res.begin(), res.end());
                res.erase(unique(res.begin(), res.end()), res.end());
            }}}return res;}