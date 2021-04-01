// ALL in Z-ring
// T, k > 0 && return (T - k) + (T - 2 * k) + ...
// last, last > 0
ll f(ll T, ll k) {
  ll cnt = T / k;
  return T * cnt - k * cnt * (cnt + 1) / 2;
}
// A, B, C > 0
// |{(x, y): x, y > 0 && Ax + By <= C}|
ll count_triangle(ll A, ll B, ll C) {
  if (A + B > C) return 0;
  if (A > B) swap(A, B);
  ll k = B / A;
  return f(k * C / B, k) +
         count_triangle(A, B - A * k,
                        C - A * (k * C / B));
}
// A, B, C, cx, cy > 0
// |{(x,y) : 1 <= x <= cx && 1 <= y <= cy && Ax +
// By <= C }|
ll count_solutions(ll A, ll B, ll C, ll cx,
                   ll cy) {
  assert(A > 0);
  assert(B > 0);
  if (C <= 0 || cx <= 0 || cy <= 0) return 0;
  if (A * cx + B * cy <= C) return cx * cy;
  if (cx >= C / A && cy >= C / B)
    return count_triangle(A, B, C);
  return count_triangle(A, B, C) -
         count_triangle(A, B, C - B * cy) -
         count_triangle(A, B, C - A * cx);
}
