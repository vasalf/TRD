template <class T> struct Q {
  T u = T(0);
  T v = T(1);
  // u / v
  // v > 0, gcd(|u|, |v|) = 1
  T gcd(T x, T y) {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (x) {
      // x, y -> y % x, x
      y %= x;
      swap(x, y);
    }
    return y;
  }
  Q() = default;
  Q(T uu, T vv = T(1)) {
    u = uu;
    v = vv;
    T g = gcd(uu, vv);
    u /= g;
    v /= g;
    if (v < 0) v = -v, u = -u;
  }
  Q operator+(const Q& o) const {
    return Q(u * o.v + o.u * v, v * o.v);
  }
  Q operator-(const Q& o) const {
    return Q(u * o.v - o.u * v, v * o.v);
  }
  Q operator*(const Q& o) const {
    return Q(u * o.u, v * o.v);
  }
  Q operator/(const Q& o) const {
    return Q(u * o.v, v * o.u);
  }
  bool operator==(const Q& o) const {
    return u * o.v == o.u * v;
  }
  bool operator<(const Q& o) const {
    return u * o.v < o.u * v;
  }
  bool operator>(const Q& o) const {
    return u * o.v > o.u * v;
  }
  bool operator<=(const Q& o) const {
    return u * o.v <= o.u * v;
  }
  bool operator>=(const Q& o) const {
    return u * o.v >= o.u * v;
  }
  ld to_ld() const { return ld(u) / ld(v); }
};
struct Line {
  ll a = 0;
  ll b = 0;
  ll c = 0;
  Line() = default;
  Line(ll aa, ll bb, ll cc) {
    a = aa;
    b = bb;
    c = cc;
    assert(a != 0 || b != 0);
  }
  template <class T>
  Q<T> vertical_line_to_x() const {
    return Q<T>(-c, a); // ax + c == 0, x = -c / a
  }
  bool parallel(const Line& o) const {
    return __int128(a) * o.b == __int128(b) * o.a;
  }
  template <class T>
  Q<T> get_x(const Line& o)
    const { // should not be parallel
    assert(!parallel(o));
    if (b == 0) return o.get_x<T>(*this);
    return Q<T>(
      T(o.b) * T(c) - T(o.c) * T(b),
      T(b) * T(o.a) -
        T(o.b) * T(a)); // (B2 * C1 - C2 * B1) /
                        // (B1 * A2 - B2 * A1)
  }
  ld get_y_by_x(ld x) const {
    return (-c - a * x) / b;
  }
  pair<ld, ld> intersect(const Line& o) const {
    ld x = get_x<__int128>(o).to_ld(), y;
    if (b)
      y = get_y_by_x(x);
    else
      y = o.get_y_by_x(x);
    return {x, y};
  }
  template <class T> Q<T> get_angle() const {
    return Q<T>(-a, b);
  }
  template <class T> Q<T> get_bias() const {
    return Q<T>(-c, b);
  }
  Line mirror_x() const { return {-a, b, c}; }
  Line mirror_y() const { return {a, -b, c}; }
};
struct Response {
  enum TYPE {
    INF,
    FINITE,
    EMPTY
  } type; // inf maybe in one or two directions
  vector<Line>
    lines; // lines in counter-clockwise order
};
vector<Line>
build_down_convex_hull(vector<Line> halfs) {
  sort(halfs.begin(), halfs.end(),
       [&](const Line& h1, const Line& h2) {
         __int128 hlp =
           __int128(-h1.a) * __int128(h2.b) -
           __int128(-h2.a) * __int128(h1.b);
         if (hlp == 0) {
           __int128 value =
             __int128(-h1.c) * __int128(h2.b) -
             __int128(-h2.c) * __int128(h1.b);
           if (h1.b < 0) value = -value;
           if (h2.b < 0) value = -value;
           return value < 0;
         }
         if (h1.b < 0) hlp = -hlp;
         if (h2.b < 0) hlp = -hlp;
         return hlp < 0;
       });
  vector<Line> st;
  for (Line L : halfs) {
    if ((int)st.size() >= 1 &&
        st.back().parallel(L))
      st.pop_back();
    while ((int)st.size() >= 2) {
      Line L1 = st[(int)st.size() - 2];
      Line L2 = st[(int)st.size() - 1];
      auto x1 = L1.get_x<__int128>(L2);
      auto x2 = L2.get_x<__int128>(L);
      if (x1 < x2) break;
      st.pop_back();
    }
    st.push_back(L);
  }
  return st;
}
template <class T>
void left_cut_hull(vector<Line>& hull, Q<T> LE) {
  int i = 0;
  while (i + 1 < (int)hull.size() &&
         hull[i].get_x<T>(hull[i + 1]) <= LE)
    i++;
  hull =
    vector<Line>(hull.begin() + i, hull.end());
}
vector<Line> concat_hulls(vector<Line> up,
                          vector<Line> down,
                          optional<Line> LE,
                          optional<Line> RI) {
  reverse(up.begin(), up.end());
  vector<Line> result;
  for (auto l : up)
    result.push_back(l);
  if (LE.has_value() &&
      (up.empty() || down.empty() ||
       up.back().get_angle<__int128>() <=
         down.front().get_angle<__int128>() ||
       up.back().get_x<__int128>(down.front()) <
         LE.value()
           .vertical_line_to_x<__int128>()))
    result.push_back(LE.value());
  for (auto l : down)
    result.push_back(l);
  if (RI.has_value() &&
      (up.empty() || down.empty() ||
       up.front().get_angle<__int128>() >=
         down.back().get_angle<__int128>() ||
       up.front().get_x<__int128>(down.back()) >
         RI.value()
           .vertical_line_to_x<__int128>()))
    result.push_back(RI.value());
  return result;
}
// ax + by + c >= 0, a^2 + b^2 > 0
// be careful with overfloating (|a,b,c| <= 10*9
// are ok, you can define __int128 ld) builds
// STRICTLY convex area (all unnecessary
// halfplanes will be ignored)
Response
halfplanes_intersection(vector<Line> halfs) {
  for (Line h : halfs)
    assert(h.a != 0 || h.b != 0);
  optional<Line> LE, RI;
  vector<Line> up, down;
  for (Line h : halfs) {
    if (h.b == 0) {  // vertical
      if (h.a > 0) { // to the right
        if (!LE.has_value() ||
            LE.value()
                .vertical_line_to_x<__int128>() <
              h.vertical_line_to_x<__int128>())
          LE = h;
      } else { // to the left
        if (!RI.has_value() ||
            RI.value()
                .vertical_line_to_x<__int128>() >
              h.vertical_line_to_x<__int128>())
          RI = h;
      }
    } else { // non-vertical
      if (h.b > 0)
        down.push_back(h);
      else
        up.push_back(h);
    }
  }
  if (LE.has_value() && RI.has_value() &&
      LE.value().vertical_line_to_x<__int128>() >
        RI.value().vertical_line_to_x<__int128>())
    return {Response::TYPE::EMPTY, {}};
  down = build_down_convex_hull(down);
  //    return {Response::TYPE::INF, {}};
  for (auto& l : up)
    l = l.mirror_y();
  up = build_down_convex_hull(up);
  for (auto& l : up)
    l = l.mirror_y();
  for (int phase = 0; phase < 2; phase++) {
    for (int iter = 0; iter < 2; iter++) {
      if (phase == 0) {
        if (LE.has_value()) {
          left_cut_hull<__int128>(
            down,
            LE.value()
              .vertical_line_to_x<__int128>());
          left_cut_hull<__int128>(
            up,
            LE.value()
              .vertical_line_to_x<__int128>());
        }
      } else {
        while (1) {
          int any = 0;
          if (!up.empty()) {
            int i = 0;
            while (
              i + 1 < (int)down.size() &&
              down[i + 1].get_angle<__int128>() <
                up[0].get_angle<__int128>() &&
              up[0].get_x<__int128>(down[i]) <=
                up[0].get_x<__int128>(
                  down[i + 1]))
              i++;
            any |= i > 0;
            down = vector<Line>(down.begin() + i,
                                down.end());
          }
          if (!down.empty()) {
            int i = 0;
            while (
              i + 1 < (int)up.size() &&
              up[i + 1].get_angle<__int128>() >
                down[0].get_angle<__int128>() &&
              down[0].get_x<__int128>(up[i]) <=
                down[0].get_x<__int128>(
                  up[i + 1]))
              i++;
            any |= i > 0;
            up = vector<Line>(up.begin() + i,
                              up.end());
          }
          if (!any) break;
        }
      }
      for (auto& l : up)
        l = l.mirror_x();
      for (auto& l : down)
        l = l.mirror_x();
      reverse(up.begin(), up.end());
      reverse(down.begin(), down.end());
      swap(LE, RI);
      if (LE.has_value())
        LE = LE.value().mirror_x();
      if (RI.has_value())
        RI = RI.value().mirror_x();
    }
  }
  vector<Line> result =
    concat_hulls(up, down, LE, RI);
  if (up.empty() || down.empty()) {
    return {Response::TYPE::INF, result};
  }
  if ((int)up.size() == 1 &&
      (int)down.size() == 1) {
    if (up[0].parallel(down[0])) {
      if (down[0].get_bias<__int128>() >
          up[0].get_bias<__int128>())
        return {Response::TYPE::EMPTY, {}};
      return {LE.has_value() && RI.has_value()
                ? Response::TYPE::FINITE
                : Response::TYPE::INF,
              result};
    } else {
      auto x0 = up[0].get_x<__int128>(down[0]);
      if (up[0].get_angle<__int128>() <
          down[0].get_angle<__int128>()) {
        if (LE.has_value() &&
            x0 <
              LE.value()
                .vertical_line_to_x<__int128>())
          return {Response::TYPE::EMPTY, {}};
      } else {
        if (RI.has_value() &&
            x0 >
              RI.value()
                .vertical_line_to_x<__int128>())
          return {Response::TYPE::EMPTY, {}};
      }
    }
  }
  bool is_empty = false;
  for (int iter = 0; iter < 2; iter++) {
    if ((int)down.size() >= 2 &&
        up[0].get_angle<__int128>() >
          down[0].get_angle<__int128>() &&
        up[0].get_x<__int128>(down[0]) >=
          down[0].get_x<__int128>(down[1]))
      is_empty = true;
    if ((int)down.size() >= 2 &&
        up[(int)up.size() - 1]
            .get_angle<__int128>() <
          down[(int)down.size() - 1]
            .get_angle<__int128>() &&
        up[(int)up.size() - 1].get_x<__int128>(
          down[(int)down.size() - 1]) <=
          down[(int)down.size() - 1]
            .get_x<__int128>(
              down[(int)down.size() - 2]))
      is_empty = true;
    for (auto& l : up)
      l = l.mirror_y();
    for (auto& l : down)
      l = l.mirror_y();
    swap(up, down);
  }
  if (is_empty)
    return {Response::TYPE::EMPTY, {}};
  auto type = Response::TYPE::FINITE;
  if (!LE.has_value() &&
      down.front().get_angle<__int128>() >=
        up.front().get_angle<__int128>())
    type = Response::TYPE::INF;
  if (!RI.has_value() &&
      down.back().get_angle<__int128>() <=
        up.back().get_angle<__int128>())
    type = Response::TYPE::INF;
  return {type, result};
}
ld halfplanes_intersection_area(
  Response response) {
  if (response.type == Response::TYPE::EMPTY)
    return 0;
  assert(response.type != Response::TYPE::INF);
  vector<pair<ld, ld>> p;
  auto lines = response.lines;
  int sz = (int)lines.size();
  ld area = 0;
  if (sz > 0) {
    for (int i = 0; i < sz; i++) {
      int j = (i + 1) % sz;
      p.push_back(lines[i].intersect(lines[j]));
    }
    for (int i = 0; i < sz; i++) {
      int j = (i + 1) % sz;
      auto [x1, y1] = p[i];
      auto [x2, y2] = p[j];
      area += x1 * y2 - x2 * y1;
    }
    area = max(area, ld(0));
  }
  return area / 2;
}
