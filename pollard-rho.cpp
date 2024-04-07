constexpr uint64_t mmul(uint64_t a, uint64_t b,
                        uint64_t mod) {
  return (static_cast<__uint128_t>(a) * b) % mod;
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
inline bool miller_rabin_det(uint64_t n) {
  //    static const ll bases[] = {
  //            2, 3, 5, 7, 11,
  //            13, 17, 19, 23}; // works for n
  //            < 3.8e18
  static const int bases[] = {
    2,  3,  5,  7,  11, 13,
    17, 19, 23, 29, 31, 37}; // n < 3.1e23
  if (n <= 2) return (n == 2);
  if (n % 2 == 0) return false;
  uint64_t d = n - 1;
  while (!(d & 1))
    d >>= 1;
  for (uint64_t a : bases) {
    if (a == n) return true;
    a = mpow(a, d, n);
    if (a == 1) continue;
    for (uint64_t nd = d;
         nd != n - 1 && a != n - 1; nd <<= 1)
      a = mmul(a, a, n);
    if (a != n - 1) return false;
  }
  return true;
}
inline uint64_t pollard(uint64_t n) {
  static std::mt19937_64 gen;
  static const int LOG = 64;
  if (n <= 1 || miller_rabin_det(n)) return 1;
  if (!(n & 1)) return 2;
  auto f = [n](uint64_t x) {
    return mmul(x, x, n) + 1;
  }; // it is ok if 0 == n
  for (int st = 2, lg = 0;; st = gen() % n) {
    uint64_t cur = 1;
    for (uint64_t x = st, y = f(st); x != y;
         x = f(x), y = f(f(y))) {
      if (uint64_t c =
            mmul(cur, x > y ? x - y : y - x, n);
          c)
        cur = c;
      if (lg++ == LOG) {
        lg = 0;
        if (uint64_t val = std::__gcd(cur, n);
            val != 1)
          return val;
      }
    }
  }
  return 1;
}
