struct pt {
    ll x = 0, y = 0;
    pt() = default;
    pt(ll x, ll y): x(x), y(y) {}
    void read() {
        cin >> x >> y;
    }
    pt operator - (const pt &o) const {
        return pt(x - o.x, y - o.y);
    }
    pt operator + (const pt &o) const {
        return pt(x + o.x, y + o.y);
    }
    ll vector_mul(const pt &o) const {
        return x * o.y - o.x * y;
    }
    ll scalar_mul(const pt &o) const {
        return x * o.x + y * o.y;
    }
};

vector<pt> convex_hull(vector<pt> a) {
    sort(a.begin(), a.end(), [&](pt p1, pt p2) {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    });
    pt p0 = a[0];
    sort(a.begin() + 1, a.end(), [&](pt p1, pt p2) {
        return (p1 - p0).vector_mul(p2 - p0) > 0;
    });
    vector<pt> st;
    st.push_back(p0);
    for (int i = 1; i < (int)a.size(); i++) {
        pt p = a[i];
        while ((int)st.size() >= 2) {
            pt p1 = st[(int)st.size() - 2];
            pt p2 = st[(int)st.size() - 1];
            if ((p - p2).vector_mul(p1 - p2) > 0) {
                break;
            }
            st.pop_back();
        }
        st.push_back(p);
    }
    return st;
}

// line (a, b)
// segment (c, d)
bool intersect(pt a, pt b, pt c, pt d, ld &xx, ld &yy) {
    ll lx = (b - a).vector_mul(c - a);
    ll rx = (b - a).vector_mul(d - a);
    if ((lx > 0 && rx > 0) || (lx < 0 && rx < 0)) return false;
    if (lx == 0) {
        xx = c.x;
        yy = c.y;
        return true;
    }
    if (rx == 0) {
        xx = d.x;
        yy = d.y;
        return true;
    }
    ll x2 = d.x - c.x;
    ll y2 = d.y - c.y;
    pt v = b - a;
    if (v.x * y2 + x2 * v.y == 0) return false;
    ld k = ld(x2 * (a.y - c.y) - y2 * (a.x - c.x)) / ld(v.x * y2 - x2 * v.y);
    xx = v.x * k + a.x;
    yy = v.y * k + a.y;
    return true;
}
