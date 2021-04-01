struct Tandem {
  int l, r, k;
  // [l, l + 2 * k) [l + 1, l + 1 + 2 * k) [l
  // + 2, l + 2 + 2 * k), ..., [r, r + 2 * k)
};
vector<int> z_func(const string& s) {
  int n = (int)s.size();
  vector<int> z(n);
  for (int l = 0, r = -1, i = 1; i < n; i++) {
    int k = i > r ? 0 : min(z[i - l], r - i + 1);
    while (i + k < n && s[i + k] == s[k])
      k++;
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
  if (R - L + 1 <= 1) { return; }
  int M = (L + R) / 2;
  rec(s, L, M);
  rec(s, M + 1, R);
  int nu = M - L + 1;
  int nv = R - M;
  string vu =
    s.substr(M + 1, nv) + "#" + s.substr(L, nu);
  string urvr = vu;
  reverse(urvr.begin(), urvr.end());
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
        tandems[tsz++] = {x - rsh + 1 - k,
                          x - lsh + 1 - k, k};
      }
    }
  }
}
void compress() { // O(n*log(n)*log(n)) can be
                  // replace with count sort
                  // (O(n*log(n))) BE careful with
                  // ML !!!
  // O(n*log(n)) --> O(n)
  sort(tandems, tandems + tsz,
       [](const Tandem& t1, const Tandem& t2) {
         return t1.k < t2.k ||
                (t1.k == t2.k && t1.l < t2.l);
       });
  int hlp_sz = 0;
  for (int i = 0; i < tsz; i++) {
    int j = i;
    while (j + 1 < tsz &&
           tandems[i].k == tandems[j + 1].k &&
           tandems[j].r + 1 == tandems[j + 1].l) {
      j++;
    }
    hlp[hlp_sz++] = {tandems[i].l, tandems[j].r,
                     tandems[j].k};
    i = j;
  }
  memcpy(tandems, hlp, sizeof(Tandem) * hlp_sz);
  tsz = hlp_sz;
}
void main_lorentz(const string& s) {
  // n = 10^6  time = 1.8 sec MEM = nlog(n) * 12
  // bytes
  int n = (int)s.size();
  tsz = 0;
  rec(s, 0, n - 1);
  compress();
}
