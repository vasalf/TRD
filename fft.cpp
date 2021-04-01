typedef complex<ld> base;
const int LOG = 20;
const int N = 1 << LOG;
int rev[N];
vec<base> PW[LOG + 1];
void precalc() {
  for (int i = 1; i < N; i++) {
    rev[i] =
      (rev[i >> 1] >> 1) | ((i & 1) << (LOG - 1));
  }
  for (int lvl = 0; lvl <= LOG; lvl++) {
    int sz = 1 << lvl;
    ld alpha = 2 * pi / sz;
    base root(cos(alpha), sin(alpha));
    base cur = 1;
    PW[lvl].resize(sz);
    for (int j = 0; j < sz; j++) {
      PW[lvl][j] = cur;
      cur *= root;
    }
  }
}
void fft(base* a, bool invert = 0) {
  for (int j, i = 0; i < N; i++) {
    if ((j = rev[i]) > i) swap(a[i], a[j]);
  }
  base u, v;
  for (int lvl = 0; lvl < LOG; lvl++) {
    int len = 1 << lvl;
    for (int i = 0; i < N; i += (len << 1)) {
      for (int j = 0; j < len; j++) {
        u = a[i + j];
        v =
          a[i + j + len] *
          (invert
             ? PW[lvl + 1][j ? (len << 1) - j : 0]
             : PW[lvl + 1][j]);
        a[i + j] = u + v;
        a[i + j + len] = u - v;
      }
    }
  }
  if (invert) {
    for (int i = 0; i < N; i++) {
      a[i] /= N;
    }
  }
}
