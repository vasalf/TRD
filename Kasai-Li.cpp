vector<int> get_lcp(const string& s,
                    const vector<int>& suf) {
  int n = (int)suf.size();
  vector<int> back(n);
  for (int i = 0; i < n; i++)
    back[suf[i]] = i;
  vector<int> lcp(n - 1);
  for (int i = 0, k = 0; i < n; i++) {
    int x = back[i];
    k = max(0, k - 1);
    if (x == n - 1) {
      k = 0;
      continue;
    }
    while (s[suf[x] + k] == s[suf[x + 1] + k])
      k++;
    lcp[x] = k;
  }
  return lcp;
}
