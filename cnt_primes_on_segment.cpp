const int X = 1.5e7;
const int MEM_K = 20;
const int MEM_N = 1e5;
int d[X];
vector<int> ps;
int mem[MEM_K][MEM_N];
void precalc() {
  for (int p = 2; p < X; p++) {
    if (!d[p]) ps.push_back(d[p] = p);
    for (int x : ps) {
      if (x > d[p] || x * p >= X) break;
      d[x * p] = x;
    }
    d[p] = d[p - 1] + (d[p] == p);
  }
}
ll rec(ll n, int k) {
  if (n <= 1) return 0;
  if (k == 0) return n - 1;
  if (ps[k - 1] > n) return 0;
  if (n < X && 1ll * ps[k] * ps[k] > n)
    return d[n] - k;
  if (k < MEM_K && n < MEM_N && mem[k][n])
    return mem[k][n] - 1;
  ll res =
    rec(n, k - 1) - rec(n / ps[k - 1], k - 1) - 1;
  if (k < MEM_K && n < MEM_N) mem[k][n] = res + 1;
  return res;
}
ll get_cnt_primes(
  ll n) { // # primes on [1, n], n <= 10^11, 10
          // queries, ~500ms
  ll m = 1;
  while (m * m < n)
    m++;
  assert(m <= n);
  int k = d[m];
  return k + rec(n, k);
}
