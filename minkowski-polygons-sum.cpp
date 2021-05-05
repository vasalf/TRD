vector<pt> minkowski_polygons_sum(vector<pt> a,
                                  vector<pt> b) {
  // a and b have counter-clock wise order
  auto cmp = [](const pt& p1,
                const pt& p2) -> bool {
    return make_pair(p1.x, p1.y) <
           make_pair(p2.x, p2.y);
  };
  rotate(a.begin(),
         min_element(a.begin(), a.end(), cmp),
         a.end());
  rotate(b.begin(),
         min_element(b.begin(), b.end(), cmp),
         b.end());
  pt q = a[0] + b[0];
  int n = (int)a.size();
  int m = (int)b.size();
  vector<pt> result = {q};
  for (int i = 0, j = 0; i < n || j < m;) {
    pt vi, vj;
    if (i < n)
      vi = a[i + 1 < n ? i + 1 : 0] - a[i];
    if (j < m)
      vj = b[j + 1 < m ? j + 1 : 0] - b[j];
    if (i < n &&
        (j == m || vi.vector_mul(vj) > eps))
      q = q + vi, i++;
    else
      q = q + vj, j++;
    result.push_back(q);
  }
  result.pop_back();
  return result;
}
