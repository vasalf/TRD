// https://codeforces.com/gym/102823/problem/L
#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef long long ll;
typedef unsigned int ui;
typedef unsigned long long ull;
typedef long double ld;

const int inf = 1e9;
const ll inf64 = 1e18;

template <class T>
struct Q {
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

        if (v < 0)
            v = -v, u = -u;
    }

    Q operator + (const Q& o) const {
        return Q(u * o.v + o.u * v, v * o.v);
    }

    Q operator - (const Q& o) const {
        return Q(u * o.v - o.u * v, v * o.v);
    }

    Q operator * (const Q& o) const {
        return Q(u * o.u, v * o.v);
    }

    Q operator / (const Q& o) const {
        return Q(u * o.v, v * o.u);
    }

    bool operator == (const Q& o) const {
        return u * o.v == o.u * v;
    }

    bool operator < (const Q& o) const {
        return u * o.v < o.u * v;
    }

    bool operator > (const Q& o) const {
        return u * o.v > o.u * v;
    }

    bool operator <= (const Q& o) const {
        return u * o.v <= o.u * v;
    }

    bool operator >= (const Q& o) const {
        return u * o.v >= o.u * v;
    }

    ld to_ld() const {
        return ld(u) / ld(v);
    }
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

    template<class T>
    Q<T> vertical_line_to_x() const {
        return Q<T>(-c, a); // ax + c == 0, x = -c / a
    }

    bool parallel(const Line& o) const {
        return __int128(a) * o.b == __int128(b) * o.a;
    }

    template <class T>
    Q<T> get_x(const Line& o) const { // should not be parallel
        assert(!parallel(o));
        if (b == 0)
            return o.get_x<T>(*this);
        return Q<T>(
                T(o.b) * T(c) - T(o.c) * T(b),
                T(b) * T(o.a) - T(o.b) * T(a)
        ); // (B2 * C1 - C2 * B1) / (B1 * A2 - B2 * A1)
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

    template <class T>
    Q<T> get_angle() const {
        return Q<T>(-a, b);
    }

    template <class T>
    Q<T> get_bias() const {
        return Q<T>(-c, b);
    }

    Line mirror_x() const {
        return {-a, b, c};
    }

    Line mirror_y() const {
        return {a, -b, c};
    }
};

struct Response {
    enum TYPE {INF, FINITE, EMPTY} type; // inf maybe in one or two directions
    vector<Line> lines; // lines in counter-clockwise order
};

vector<Line> build_down_convex_hull(vector<Line> halfs) {
    sort(halfs.begin(), halfs.end(), [&](const Line& h1, const Line& h2) {
        __int128 hlp = __int128(-h1.a) * __int128(h2.b) - __int128(-h2.a) * __int128(h1.b);
        if (hlp == 0) {
            __int128 value = __int128(-h1.c) * __int128(h2.b) - __int128(-h2.c) * __int128(h1.b);
            if (h1.b < 0)
                value = -value;
            if (h2.b < 0)
                value = -value;
            return value < 0;
        }
        if (h1.b < 0)
            hlp = -hlp;
        if (h2.b < 0)
            hlp = -hlp;
        return hlp < 0;
    });
    vector<Line> st;
    for (Line L : halfs) {
        if ((int) st.size() >= 1 && st.back().parallel(L))
            st.pop_back();
        while ((int) st.size() >= 2) {
            Line L1 = st[(int) st.size() - 2];
            Line L2 = st[(int) st.size() - 1];
            auto x1 = L1.get_x<__int128>(L2);
            auto x2 = L2.get_x<__int128>(L);
            if (x1 < x2)
                break;
            st.pop_back();
        }
        st.push_back(L);
    }
    return st;
}

template <class T>
void left_cut_hull(vector<Line>& hull, Q<T> LE) {
    int i = 0;
    while (i + 1 < (int) hull.size() && hull[i].get_x<T>(hull[i + 1]) <= LE)
        i++;
    hull = vector<Line>(hull.begin() + i, hull.end());
}

vector<Line> concat_hulls(vector<Line> up, vector<Line> down, optional<Line> LE, optional<Line> RI) {
    reverse(up.begin(), up.end());
    vector<Line> result;
    for (auto l : up)
        result.push_back(l);
    if (LE.has_value() &&
        (up.empty() || down.empty() ||
         up.back().get_angle<__int128>() <= down.front().get_angle<__int128>() ||
         up.back().get_x<__int128>(down.front()) < LE.value().vertical_line_to_x<__int128>()))
        result.push_back(LE.value());
    for (auto l : down)
        result.push_back(l);
    if (RI.has_value() &&
        (up.empty() || down.empty() ||
         up.front().get_angle<__int128>() >= down.back().get_angle<__int128>() ||
         up.front().get_x<__int128>(down.back()) > RI.value().vertical_line_to_x<__int128>()))
        result.push_back(RI.value());
    return result;
}

// ax + by + c >= 0, a^2 + b^2 > 0
// be careful with overfloating (|a,b,c| <= 10*9 are ok, you can define __int128 ld)
// builds STRICTLY convex area (all unnecessary halfplanes will be ignored)
Response halfplanes_intersection(vector<Line> halfs) {
    for (Line h : halfs)
        assert(h.a != 0 || h.b != 0);

//    for (auto l : halfs) {
//        cout << l.a << "*x";
//        if (l.b >= 0)
//            cout << "+";
//        cout << l.b << "*y";
//        if (l.c >= 0)
//            cout << "+";
//        cout << l.c << "=0\n";
//    }
//    return {Response::TYPE::INF, {}};

    optional<Line> LE, RI;
    vector<Line> up, down;
    for (Line h : halfs) {
        if (h.b == 0) { // vertical
            if (h.a > 0) { // to the right
                if (!LE.has_value() || LE.value().vertical_line_to_x<__int128>() < h.vertical_line_to_x<__int128>())
                    LE = h;
            } else { // to the left
                if (!RI.has_value() || RI.value().vertical_line_to_x<__int128>() > h.vertical_line_to_x<__int128>())
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
        LE.value().vertical_line_to_x<__int128>() > RI.value().vertical_line_to_x<__int128>())
        return {Response::TYPE::EMPTY, {}};

    down = build_down_convex_hull(down);
//    return {Response::TYPE::INF, {}};
    for (auto& l : up)
        l = l.mirror_y();
    up = build_down_convex_hull(up);
    for (auto& l : up)
        l = l.mirror_y();

//    cout << "UP\n";
//    for (auto l : up)
//        cout << l.a << " " << l.b << " " << l.c << "\n";
//    cout << "DOWN\n";
//    for (auto l : down)
//        cout << l.a << " " << l.b << " " << l.c << "\n";
//    cout << "\n";
//
//    if (LE.has_value())
//        cout << "LE = " << LE.value().vertical_line_to_x<__int128>().to_ld() << "\n";
//    if (RI.has_value())
//        cout << "RI = " << RI.value().vertical_line_to_x<__int128>().to_ld() << "\n";
//    return {Response::TYPE::INF, {}};

    for (int phase = 0; phase < 2; phase++) {
        for (int iter = 0; iter < 2; iter++) {
            if (phase == 0) {
                if (LE.has_value()) {
                    left_cut_hull<__int128>(down, LE.value().vertical_line_to_x<__int128>());
                    left_cut_hull<__int128>(up, LE.value().vertical_line_to_x<__int128>());
                }
            } else {
                while (1) {
                    int any = 0;
                    if (!up.empty()) {
                        int i = 0;
                        while (i + 1 < (int) down.size() &&
                               down[i + 1].get_angle<__int128>() < up[0].get_angle<__int128>() &&
                               up[0].get_x<__int128>(down[i]) <= up[0].get_x<__int128>(down[i + 1]))
                            i++;
                        any |= i > 0;
                        down = vector<Line>(down.begin() + i, down.end());
                    }
                    if (!down.empty()) {
                        int i = 0;
                        while (i + 1 < (int) up.size() &&
                               up[i + 1].get_angle<__int128>() > down[0].get_angle<__int128>() &&
                               down[0].get_x<__int128>(up[i]) <= down[0].get_x<__int128>(up[i + 1]))
                            i++;
                        any |= i > 0;
                        up = vector<Line>(up.begin() + i, up.end());
                    }
                    if (!any)
                        break;
                }
            }
            for (auto &l : up) l = l.mirror_x();
            for (auto &l : down) l = l.mirror_x();
            reverse(up.begin(), up.end());
            reverse(down.begin(), down.end());
            swap(LE, RI);
            if (LE.has_value()) LE = LE.value().mirror_x();
            if (RI.has_value()) RI = RI.value().mirror_x();
        }
    }

//    cout << "UP\n";
//    for (auto l : up)
//        cout << l.a << " " << l.b << " " << l.c << "\n";
//    cout << "DOWN\n";
//    for (auto l : down)
//        cout << l.a << " " << l.b << " " << l.c << "\n";
//    cout << "\n";

//    if (LE.has_value())
//        cout << "LE = " << LE.value().vertical_line_to_x<__int128>().to_ld() << "\n";
//    if (RI.has_value())
//        cout << "RI = " << RI.value().vertical_line_to_x<__int128>().to_ld() << "\n";

    vector<Line> result = concat_hulls(up, down, LE, RI);
    if (up.empty() || down.empty()) {
        return {Response::TYPE::INF, result};
    }

    if ((int) up.size() == 1 && (int) down.size() == 1) {
        if (up[0].parallel(down[0])) {
            if (down[0].get_bias<__int128>() > up[0].get_bias<__int128>())
                return {Response::TYPE::EMPTY, {}};
            return {
                    LE.has_value() && RI.has_value() ? Response::TYPE::FINITE : Response::TYPE::INF,
                    result
            };
        } else {
            auto x0 = up[0].get_x<__int128>(down[0]);
            if (up[0].get_angle<__int128>() < down[0].get_angle<__int128>()) {
                if (LE.has_value() && x0 < LE.value().vertical_line_to_x<__int128>())
                    return {Response::TYPE::EMPTY, {}};
            } else {
                if (RI.has_value() && x0 > RI.value().vertical_line_to_x<__int128>())
                    return {Response::TYPE::EMPTY, {}};
            }
        }
    }

    bool is_empty = false;
    for (int iter = 0; iter < 2; iter++) {
        if ((int) down.size() >= 2 &&
                up[0].get_angle<__int128>() > down[0].get_angle<__int128>() &&
                up[0].get_x<__int128>(down[0]) >= down[0].get_x<__int128>(down[1]))
            is_empty = true;
        if ((int) down.size() >= 2 &&
                up[(int) up.size() - 1].get_angle<__int128>() <
                down[(int) down.size() - 1].get_angle<__int128>() &&
                up[(int) up.size() - 1].get_x<__int128>(down[(int) down.size() - 1]) <=
                down[(int) down.size() - 1].get_x<__int128>(down[(int) down.size() - 2]))
            is_empty = true;
        for (auto &l : up) l = l.mirror_y();
        for (auto &l : down) l = l.mirror_y();
        swap(up, down);
    }

    if (is_empty)
        return {Response::TYPE::EMPTY, {}};

    auto type = Response::TYPE::FINITE;
    if (!LE.has_value() && down.front().get_angle<__int128>() >= up.front().get_angle<__int128>())
        type = Response::TYPE::INF;
    if (!RI.has_value() && down.back().get_angle<__int128>() <= up.back().get_angle<__int128>())
        type = Response::TYPE::INF;

    return {type, result};
}

ld halfplanes_intersection_area(Response response) {
    if (response.type == Response::TYPE::EMPTY)
        return 0;
    assert(response.type != Response::TYPE::INF);

    vector<pair<ld, ld>> p;
    auto lines = response.lines;
    int sz = (int) lines.size();
    ld area = 0;

    if (sz > 0) {

        for (int i = 0; i < sz; i++) {
            int j = (i + 1) % sz;
            p.push_back(lines[i].intersect(lines[j]));
        }
        for (int i = 0; i < sz; i++) {
            int j = (i + 1) % sz;
            auto[x1, y1] = p[i];
            auto[x2, y2] = p[j];
            area += x1 * y2 - x2 * y1;
        }

        area = max(area, ld(0));
    }

    return area / 2;
}

void work() {
    cout.precision(20);
    cout << fixed;

    int T;
    cin >> T;

    struct pt {
        ll x = 0;
        ll y = 0;

        void read() {
            cin >> x >> y;
        }

        pt operator - (const pt& o) const {
            return {x - o.x, y - o.y};
        }

        pt operator + (const pt& o) const {
            return {x + o.x, y + o.y};
        }

        pt operator * (ll coef) const {
            return {x * coef, y * coef};
        }

        ll scalar_mul(const pt& o) const {
            return x * o.x + y * o.y;
        }

        ll sqr_norm() const {
            return scalar_mul(*this);
        }

        ld norm() const {
            return sqrtl(max(0ll, sqr_norm()));
        }

        ll vector_mul(const pt& o) const {
            return x * o.y - o.x * y;
        }

        void show() {
            cout << "(" << x << "," << y << ")";
        }

        pll to_pair() const {
            return {x, y};
        }
    };;

    const ld eps = 1e-9;

    for (int t = 1; t <= T; t++) {
        cout << "Case " << t << ": ";
        pt w1, w2, b1, b2;
        w1.read();
        w2.read();
        b1.read();
        b2.read();

        if ((w1 - w2).norm() < eps) {
            cout << 0.0 << "\n";
            continue;
        }

        vector<Line> halfs;
        int ok = 1;

        vector<pt> good_bs;

        for (pt b : {b1, b2}) {
            auto [x1, y1] = w1;
            auto [x2, y2] = w2;
            if ((w2 - w1).vector_mul(b - w1) < -eps) {
                halfs.emplace_back(
                        -(y2 - y1),
                        -(x1 - x2),
                        -(1ll * (y1 - y2) * x1 + 1ll * y1 * (x2 - x1))
                );
                good_bs.push_back(b);
            } else if ((w1 - w2).vector_mul(b - w2) < -eps) {
                halfs.emplace_back(
                        (y2 - y1),
                        (x1 - x2),
                        (1ll * (y1 - y2) * x1 + 1ll * y1 * (x2 - x1))
                );
                good_bs.push_back(b);
            } else {
                if (min(w1.x, w2.x) <= b.x + eps && b.x <= max(w1.x, w2.x) + eps &&
                    min(w1.y, w2.y) <= b.y + eps && b.y <= max(w1.y, w2.y) + eps) {
                } else {
                    ok = 0;
                    break;
                }
            }
        }

        if (!ok) {
            cout << 0.0 << "\n";
            continue;
        }

        for (pt b : good_bs) {
            for (pt w : {w1, w2}) {
                if ((w - b).vector_mul(w1 - b) < -eps || (w - b).vector_mul(w2 - b) < -eps) {
                    auto[x1, y1] = w;
                    auto[x2, y2] = b;
                    halfs.emplace_back(
                            -(y2 - y1),
                            -(x1 - x2),
                            -(1ll * (y1 - y2) * x1 + 1ll * y1 * (x2 - x1))
                    );
                }
                else {
                    auto[x1, y1] = b;
                    auto[x2, y2] = w;
                    halfs.emplace_back(
                            -(y2 - y1),
                            -(x1 - x2),
                            -(1ll * (y1 - y2) * x1 + 1ll * y1 * (x2 - x1))
                    );
                }
            }
        }

//        for (auto [p1, p2] : lines) {
//            p1.show(); cout << " -> "; p2.show(); cout << "\n";
//        }

        Response response = halfplanes_intersection(halfs);

        if (response.type == Response::TYPE::INF) {
            cout << "inf\n";
        } else {
            cout << halfplanes_intersection_area(response) << "\n";
        }
    }
}

namespace SLOW {

    struct point {
        ld x = 0;
        ld y = 0;
    };

    struct line {
        ld a = 0;
        ld b = 0;
        ld c = 0;
    };

    point intersect(line l1, line l2) {
        ld tmp = l1.a * l2.b - l2.a * l1.b;
        ld x = (l1.b * l2.c - l2.b * l1.c) / tmp;
        ld y = (l2.a * l1.c - l1.a * l2.c) / tmp;
        return {x, y};
    }

    ld cp(point p1, point p2) {
        return p1.x * p2.y - p2.x * p1.y;
    }

    ld halfplanes_intersection_area_slow(vector<point> init, vector<line> init_lines, vector<line> lines) {
        for (line l : lines) {
            int cnt = 0;
            for (point p : init) {
                if (l.a * p.x + l.b * p.y + l.c >= 0) {
                    cnt++;
                }
            }
            if (cnt == 0) {
                return 0;
            }
            if (cnt == (int) init.size()) {
                continue;
            }
            for (int i = 1; i < (int) init.size(); i++) {
                if (l.a * init[i].x + l.b * init[i].y + l.c >= 0 &&
                    l.a * init[i - 1].x + l.b * init[i - 1].y + l.c < 0) {
                    rotate(init.begin(), init.begin() + i, init.end());
                    rotate(init_lines.begin(), init_lines.begin() + i, init_lines.end());
                    break;
                }
            }
            int j = 0;
            while (l.a * init[j].x + l.b * init[j].y + l.c >= 0)
                j++;

            point a = intersect(init_lines[j - 1], l);
            point b = intersect(l, init_lines.back());
            line last_line = init_lines.back();
            init.erase(init.begin() + j, init.end());
            init_lines.erase(init_lines.begin() + j, init_lines.end());
            init.push_back(a);
            init.push_back(b);
            init_lines.push_back(l);
            init_lines.push_back(last_line);
        }

        ld sq = 0;
        for (int i = 0; i < (int) init.size(); i++)
        {
            sq += cp(init[i], init[(i + 1) % init.size()]);
        }
        return fabsl(sq) / 2;
    }

}

void test() {
    cout.precision(20);
    cout << fixed;
    mt19937 rnd(21);
    const int MAXX = 1e5;
    const ld eps = 1e-5;
    const int N = 100;
    while (1) {
        int n = rnd() % N + 1;
        vector<Line> to_fast(n);
        vector<SLOW::line> to_slow(n);
        set<tuple<int, int, int>> was;
        for (int i = 0; i < n; i++) {
            int a, b, c;
            while (1) {
                a = rnd() % (2 * MAXX + 1) - MAXX;
                b = rnd() % (2 * MAXX + 1) - MAXX;
                c = rnd() % (2 * MAXX + 1) - MAXX;
                if (was.count({a, b, c}))
                    continue;
                if (a != 0 || b != 0)
                    break;
            }
            was.insert({a, b, c});
            to_fast[i] = {ll(a), ll(b), ll(c)};
            to_slow[i] = {ld(a), ld(b), ld(c)};
        }
        ld X = 1e5;
        vector<SLOW::point> init = {{-X, -X},
                                    {X,  -X},
                                    {X,  X},
                                    {-X, X}};
        vector<SLOW::line> init_lines = {{0,  1,  X},
                                         {-1, 0,  X},
                                         {0,  -1, X},
                                         {1,  0,  X}};
        ld sl = SLOW::halfplanes_intersection_area_slow(init, init_lines, to_slow);

        for (auto l : init_lines) {
            to_fast.emplace_back(ll(l.a), ll(l.b), ll(l.c));
        }
        ld fs = halfplanes_intersection_area(halfplanes_intersection(to_fast));

        if (fabsl(sl - fs) < eps) {
            cout << "OK(" << fs << ")" << "\n";
        } else {
            cout << "WA\n";
            cout << "exp = " << sl << "\n";
            cout << "fnd = " << fs << "\n";
            cout << "\n";
            cout << (int) to_fast.size() << "\n";
            for (auto l : to_fast)
                cout << l.a << " " << l.b << " " << l.c << "\n";
            cout << "\n";
            for (auto l : to_fast) {
                cout << l.a << "*x";
                if (l.b >= 0)
                    cout << "+";
                cout << l.b << "*y";
                if (l.c >= 0)
                    cout << "+";
                cout << l.c << "=0\n";
            }
            break;
        }
    }
}

void help() {
    int n;
    cin >> n;
    vector<Line> halfs(n);
    for (auto& h : halfs)
        cin >> h.a >> h.b >> h.c;
    ld area = halfplanes_intersection_area(halfplanes_intersection(halfs));
    cout << area << "\n";
}

int main() {

#ifdef DEBUG
    freopen("input.txt", "r", stdin);
#endif

    ios_base::sync_with_stdio(0);
    cin.tie(0);

//    test();
//    help();

    work();

    return 0;
}

/*
9
2 1 5
-1 2 5
-3 1 5
-1 -2 4
3 1 9
-4 1 7
1 0 2
-2 0 3
-3 -1 -1
 * */