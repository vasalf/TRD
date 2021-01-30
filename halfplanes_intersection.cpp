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
    enum TYPE {INF, FINITE} type; // inf maybe in one or two directions
    vector<Line> lines; // lines in counter-clockwise order
};

vector<Line> build_down_convex_hull(vector<Line> halfs) {
    auto half_to_pair = [&](const Line& h) -> pair<Q<ll>, Q<ll>> {
        return {Q(-h.a, h.b), {-h.c, h.b}};
    };
    sort(halfs.begin(), halfs.end(), [&](const Line& h1, const Line& h2) {
        return half_to_pair(h1) < half_to_pair(h2);
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
        up.back().get_angle<ll>() <= down.front().get_angle<ll>() ||
        up.back().get_x<__int128>(down.front()) < LE.value().vertical_line_to_x<__int128>()))
        result.push_back(LE.value());
    for (auto l : down)
        result.push_back(l);
    if (RI.has_value() &&
        (up.empty() || down.empty() ||
        up.front().get_angle<ll>() >= down.back().get_angle<ll>() ||
        up.front().get_x<__int128>(down.back()) > RI.value().vertical_line_to_x<__int128>()))
        result.push_back(RI.value());
    return result;
}

// ax + by + c >= 0, a^2 + b^2 > 0
// be careful with overfloating (|a,b,c| <= 10*9 are ok)
// TODO: check for empty (does now work with empty yet)
Response halfplane_intersection(vector<Line> halfs) {
    for (Line h : halfs)
        assert(h.a != 0 || h.b != 0);

    optional<Line> LE, RI;
    vector<Line> up, down;
    for (Line h : halfs) {
        if (h.b == 0) { // vertical
            if (h.a > 0) { // to the right
                if (!LE.has_value() || LE.value().vertical_line_to_x<ll>() < h.vertical_line_to_x<ll>())
                    LE = h;
            } else { // to the left
                if (!RI.has_value() || RI.value().vertical_line_to_x<ll>() > h.vertical_line_to_x<ll>())
                    RI = h;
            }
        } else { // non-vertical
            if (h.b > 0)
                down.push_back(h);
            else
                up.push_back(h);
        }
    }

    down = build_down_convex_hull(down);
    for (auto& l : up)
        l = l.mirror_y();
    up = build_down_convex_hull(up);
    for (auto& l : up)
        l = l.mirror_y();

    for (int phase = 0; phase < 2; phase++) {
        for (int iter = 0; iter < 2; iter++) {
            if (phase == 1) {
                if (LE.has_value()) {
                    left_cut_hull<__int128>(down, LE.value().vertical_line_to_x<__int128>());
                    left_cut_hull<__int128>(up, LE.value().vertical_line_to_x<__int128>());
                }
            } else {
                if (!up.empty()) {
                    int i = 0;
                    while (i + 1 < (int) down.size() && down[i + 1].get_angle<ll>() < up[0].get_angle<ll>() &&
                       up[0].get_x<__int128>(down[i]) >= down[i].get_x<__int128>(down[i + 1]))
                        i++;
                    down = vector<Line>(down.begin() + i, down.end());
                }
                if (!down.empty()) {
                    int i = 0;
                    while (i + 1 < (int) up.size() && up[i + 1].get_angle<ll>() > down[0].get_angle<ll>() &&
                           down[0].get_x<__int128>(up[i]) >= up[i].get_x<__int128>(up[i + 1]))
                        i++;
                    up = vector<Line>(up.begin() + i, up.end());
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

    cout << "UP\n";
    for (auto l : up)
        cout << l.a << " " << l.b << " " << l.c << "\n";
    cout << "DOWN\n";
    for (auto l : down)
        cout << l.a << " " << l.b << " " << l.c << "\n";
    cout << "\n";

    vector<Line> result = concat_hulls(up, down, LE, RI);
    if (up.empty() || down.empty()) {
        return {Response::TYPE::INF, result};
    }

    if ((int) up.size() == 1 && (int) down.size() == 1 && up[0].parallel(down[0])) {
        return {
            LE.has_value() && RI.has_value() ? Response::TYPE::FINITE : Response::TYPE::INF,
            result
        };
    }

    auto type = Response::TYPE::FINITE;
    if (!LE.has_value() && down.front().get_angle<ll>() >= up.front().get_angle<ll>())
        type = Response::TYPE::INF;
    if (!RI.has_value() && down.back().get_angle<ll>() <= down.front().get_angle<ll>())
        type = Response::TYPE::INF;

    return {type, result};
}

int main() {

#ifdef DEBUG
    freopen("input.txt", "r", stdin);
#endif

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<Line> halfs(n);
    for (int i = 0; i < n; i++)
        cin >> halfs[i].a >> halfs[i].b >> halfs[i].c;

    Response response = halfplane_intersection(halfs);

    cout << response.type << "\n";
    for (auto l : response.lines)
        cout << l.a << " " << l.b << " " << l.c << "\n";

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
