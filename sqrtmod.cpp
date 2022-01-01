ll MUL(ll a, ll b, ll mod) {
  static __int128 xa = 1;
  static __int128 xb = 1;
  static __int128 xm = 1;
  xa = a;
  xb = b;
  xm = mod;
  return ll((xa * xb) % xm);
}
ll SUM(ll a, ll b, ll mod) {
  return a + b < mod ? a + b : a + b - mod;
}
ll SUB(ll a, ll b, ll mod) {
  return a >= b ? a - b : a - b + mod;
}
ll BINPOW(ll x, ll pw, ll mod) {
  if (x == 0) return 0;
  ll res = 1 % mod, tmp = x;
  while (pw > 0) {
    if (pw & 1) res = MUL(res, tmp, mod);
    pw >>= 1;
    tmp = MUL(tmp, tmp, mod);
  }
  return res;
}
ll DIV(ll a, ll b, ll mod) {
  return MUL(a, BINPOW(b, mod - 2, mod), mod);
}
ll find_sqrt_by_mod(
  ll p,
  ll A) { // x^2 = a (mod p), x = ?, p is prime
  assert(0ll <= A && A < p);
  if (A == 0 || p == 2) return A;
  if (BINPOW(A, (p - 1) / 2, p) != 1) return -1ll;
  static mt19937_64 GEN(42);
  auto mult = [&](pll p1, pll p2) -> pll {
    auto [a, b] = p1;
    auto [c, d] = p2;
    ll k1 = SUM(MUL(a, d, p), MUL(b, c, p), p);
    ll k2 = SUM(MUL(MUL(a, c, p), A, p),
                MUL(b, d, p), p);
    return {k1, k2};
  };
  while (1) {
    ll i = GEN() % (p - 1) + 1;
    ll pw = (p - 1) / 2;
    pll res = {0, 1}, tmp = {1, i};
    while (pw > 0) {
      if (pw & 1) res = mult(res, tmp);
      pw >>= 1;
      tmp = mult(tmp, tmp);
    }
    if (res.first == 0) continue;
    res.second = SUB(res.second, 1, p);
    ll x = DIV(res.second, res.first, p);
    if (MUL(x, x, p) == A) return x;
  }
}