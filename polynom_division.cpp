// let A = series and A[0] != 0 in Z/pZ, p is
// prime finds (A^{-1}) % x^n
vector<int>
series_inverse(const vector<int>& series, int n,
               ll p) {
  vector<int> current = {_div(1, series[0], p)};
  vector<int> A = {};
  int l = 0;
  while ((int)current.size() < n) {
    while (l < 2 * (int)current.size()) {
      A.push_back(
        l < (int)series.size() ? series[l] : 0);
      l++;
    }
    vector<int> next = multiply(A, current);
    for (int& x : next)
      x = (-x % p + p) % p;
    next[0] = _sum(2 % p, next[0], p);
    next = multiply(next, current);
    for (int& x : next)
      x = (x % p + p) % p;
    next.resize(2 * current.size());
    current = next;
  }
  current.resize(n);
  return current;
}
// calculates a / b
vector<int> division(const vector<int>& a,
                     const vector<int>& b,
                     int p) {
  int n = (int)a.size() - 1; // deg(a)
  int m = (int)b.size() - 1; // deg(b)
  if (n < m) { return {0}; }
  vector<int> ar = a, br = b;
  reverse(ar.begin(), ar.end());
  reverse(br.begin(), br.end());
  ar.resize(n - m + 1);
  br.resize(n - m + 1);
  vector<int> qr =
    series_inverse(br, n - m + 1, p);
  qr = multiply(qr, ar);
  qr.resize(n - m + 1);
  for (int& x : qr)
    x = (x % p + p) % p;
  reverse(qr.begin(), qr.end()); // q = q^r
  return qr;
}
// calculates a - bQ
vector<int> module(const vector<int>& a,
                   const vector<int>& b,
                   const vector<int>& Q, int p) {
  vector<int> r = multiply(b, Q);
  r.resize(b.size());
  for (int i = 0; i < (int)r.size(); i++) {
    int ai = i < (int)a.size() ? a[i] : 0;
    int ri = (r[i] % p + p) % p;
    r[i] = _sub(ai, ri, p);
  }
  return r;
}
