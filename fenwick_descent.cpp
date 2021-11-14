struct Processor {
  int n = 0; // [0, n)
  vector<int> a;
  Processor() = default;
  Processor(int nn) {
    n = nn;
    a.assign(n, 0);
  }
  void increase(int i, int x) {
    for (int cur = i; cur < n; cur |= (cur + 1))
      a[cur] += x;
  }
  int descent(int lb) {
    int pos = 0;
    for (int pw = 1 << 19; pw > 0; pw >>= 1) {
      if (pos + pw <= n && a[pos + pw - 1] < lb) {
        lb -= a[pos + pw - 1];
        pos += pw;
      }
    }
    return pos;
  }
};
