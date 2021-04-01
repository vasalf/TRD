pair<vector<int>, vector<int>>
manacker(const string& s) {
  // -> {d0, d1}. RUN test!
  int n = (int)s.size();
  vector<int> d0(n), d1(n);
  for (int l = 0, r = -1, i = 0; i < n; i++) {
    d1[i] =
      i <= r ? min(r - i, d1[l + r - i]) : 0;
    while (i >= d1[i] && i + d1[i] < n &&
           s[i - d1[i]] == s[i + d1[i]])
      d1[i]++;
    d1[i]--;
    if (i + d1[i] > r)
      l = i - d1[i], r = i + d1[i];
  }
  for (int l = 0, r = -1, i = 0; i < n; i++) {
    d0[i] =
      i < r ? min(r - i, d0[l + r - i - 1]) : 0;
    while (i >= d0[i] && i + d0[i] + 1 < n &&
           s[i - d0[i]] == s[i + d0[i] + 1])
      d0[i]++;
    if (d0[i] > 0 && i + d0[i] > r)
      l = i - d0[i] + 1, r = i + d0[i];
  }
  return {d0, d1};
}
