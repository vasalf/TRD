ll rec(ll pos, ll left_len, ll left_cost,
       ll right_len, ll right_cost, ll k) {
  if (!k || !right_len) return pos;
  if (pos >= right_len) {
    ll t = (left_len - pos + right_len - 1) /
           right_len;
    if (t * right_cost + left_cost > k)
      return pos;
    pos += t * right_len - left_len;
    k -= (t * right_cost + left_cost);
  }
  ll nxt_left_len = left_len % right_len;
  ll nxt_left_cost =
    (left_len / right_len) * right_cost +
    left_cost;
  if (nxt_left_len == 0) return pos;
  {
    ll t = pos / nxt_left_len;
    if (t * nxt_left_cost > k)
      return pos -
             nxt_left_len * (k / nxt_left_cost);
    k -= t * nxt_left_cost;
    pos -= t * nxt_left_len;
  }
  return rec(pos, nxt_left_len, nxt_left_cost,
             right_len % nxt_left_len,
             (right_len / nxt_left_len) *
                 nxt_left_cost +
               right_cost,
             k);
}
// finds (nw_st + step * x) % mod --> min, 0 <= x
// <= bound
ll euclid(ll nw_st, ll step, ll mod, ll bound) {
  return rec(nw_st, mod, 0, step, 1, bound);
}
