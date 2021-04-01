// _sum, _sub, _mul — arithmetic operations
void xor_fwht(vector<int>& a,
              bool inverse = false) {
  for (int x, y, len = 1; len < (int)a.size();
       len <<= 1) {
    for (int i = 0; i < (int)a.size();
         i += len << 1) {
      for (int j = 0; j < len; j++) {
        x = a[i + j], y = a[i + j + len];
        a[i + j] = _sum(x, y);
        a[i + j + len] = _sub(x, y);
      }
    }
  }
  if (inverse) {
    int rn = _binpow((int)a.size(), mod - 2);
    for (int& x : a)
      x = _mul(x, rn);
  }
}
void or_fwht(vector<int>& a,
             bool inverse = false) {
  for (int x, y, len = 1; len < (int)a.size();
       len <<= 1) {
    for (int i = 0; i < (int)a.size();
         i += len << 1) {
      for (int j = 0; j < len; j++) {
        x = a[i + j], y = a[i + j + len];
        a[i + j] = x,
              a[i + j + len] =
                inverse ? _sub(y, x) : _sum(y, x);
      }
    }
  }
}
void and_fwht(vector<int>& a,
              bool inverse = false) {
  for (int x, y, len = 1; len < (int)a.size();
       len <<= 1) {
    for (int i = 0; i < (int)a.size();
         i += len << 1) {
      for (int j = 0; j < len; j++) {
        x = a[i + j], y = a[i + j + len];
        a[i + j] =
          inverse ? _sub(x, y) : _sum(x, y),
              a[i + j + len] = y;
      }
    }
  }
}
