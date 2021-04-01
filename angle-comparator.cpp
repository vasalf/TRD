struct comparator {
  pll center;
  comparator(pll p) : center(p) {}
  bool operator()(const pll& p,
                  const pll& q) const {
    pll start(1, 0);
    if (p == q) return false;
    auto op = vect(center, p);
    auto oq = vect(center, q);
    if (cp(op, oq) == 0 && dp(op, oq) > 0)
      return false;
    ll sop = cp(start, op), soq = cp(start, oq);
    if (sop == 0) {
      if (dp(start, op) > 0) return true;
      return soq < 0;
    }
    if (soq == 0) {
      if (dp(start, oq) > 0) return false;
      return sop > 0;
    }
    if ((sop > 0 && soq > 0) ||
        (sop < 0 && soq < 0)) {
      return cp(op, oq) > 0;
    }
    return sop > 0;
  }
};
