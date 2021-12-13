const ld eps = 1e-9;
struct pt {
    ld x = 0, y = 0;
    pt operator + (const pt& o) const { return {x + o.x, y + o.y}; }
    pt operator - (const pt& o) const { return {x - o.x, y - o.y}; }
    pt operator * (ld coef) const { return {x * coef, y * coef}; }
    ld vector_mul(const pt& o) const { return x * o.y - o.x * y; }
    ld scalar_mul(const pt& o) const { return x * o.x + y * o.y; }
    ld sqr_norm() const { return scalar_mul(*this); }
    ld norm() const { return sqrtl(max(ld(0), sqr_norm())); }
    int quadrant() const {
        if (x >= eps && y > -eps) return 1;
        else if (x < eps && y >= eps) return 2;
        else if (x <= -eps && y < eps) return 3;
        else return 4;
    }
    bool operator < (const pt& o) const {
        int q1 = quadrant();
        int q2 = o.quadrant();
        if (q1 != q2)
            return q1 < q2;
        return vector_mul(o) >= eps;
    }
};
struct Line {
    pt a, b;
    pt dir() const { return b - a; }
};
pair<bool, pt> intersect_lines(const Line& L1, const Line& L2) {
    ld vm = L1.dir().vector_mul(L2.dir());
    if (abs(vm) < eps) return {false, pt{}};
    ld t = L2.dir().vector_mul(L1.a - L2.a) / vm;
    return {true, L1.a + (L1.b - L1.a) * t};
}
struct Response {
    enum TYPE {EMPTY, INF, FINITE};
    TYPE type;
    vector<Line> halfs;
};
bool is_line_good(Line L, Line L1, Line L2, bool strictly = false) {
    int any_colinear = 0;
    for (Line L_hat : {L1, L2}) {
        ld vm = L.dir().vector_mul(L_hat.dir());
        ld sm = L.dir().scalar_mul(L_hat.dir());
        if (abs(vm) < eps) {
            any_colinear = 1;
            if (sm >= eps) {
                if (L.dir().vector_mul(L_hat.b - L.a) >= (strictly ? eps : -eps))
                    return false;
            } else {
                if (L.dir().vector_mul(L_hat.b - L.a) <= -eps)
                    return false;
            }
        }
    }
    if (any_colinear)
        return true;
    ld vm1 = L.dir().vector_mul(L1.dir());
    ld vm2 = L.dir().vector_mul(L2.dir());
    int t1 = vm1 >= eps ? +1 : -1;
    int t2 = vm2 >= eps ? +1 : -1;
    if (t1 == t2)
        return true;
    if (t1 > t2)
        swap(t1, t2), swap(vm1, vm2), swap(L1, L2);
    pt p1 = intersect_lines(L, L1).second;
    pt p2 = intersect_lines(L, L2).second;
    return (p2 - p1).scalar_mul(L.dir()) > -eps;
}
bool check_empty(Line L1, Line L2, Line L3) {
    return
            !is_line_good(L1, L2, L3, true) &&
            !is_line_good(L2, L1, L3, true) &&
            !is_line_good(L3, L1, L2, true);
}
Response intersect_halfs(vector<Line> halfs) {
    sort(halfs.begin(), halfs.end(), [](const Line& l1, const Line& l2) {
        return l1.dir() < l2.dir();
    });
    int n = (int) halfs.size(), is_inf = 0, any_positive_vm = 0;
    deque<Line> hull;
    Line L1, L2, L3;
    ld vm, sm;
    for (int i = 0; i < n; i++) {
        vm = halfs[i].dir().vector_mul(halfs[i + 1 < n ? i + 1 : 0].dir());
        sm = halfs[i].dir().scalar_mul(halfs[i + 1 < n ? i + 1 : 0].dir());
        any_positive_vm |= vm >= eps;
        if (vm <= -eps || (vm < eps && sm <= -eps))
            is_inf = 1;
        hull.push_back(halfs[i]);
        for (int sz; (sz = (int) hull.size()) >= 3;) {
            L1 = hull[0], L2 = hull[1], L3 = hull[sz - 1];
            if (check_empty(L1, L2, L3))
                return Response{Response::TYPE::EMPTY, {}};
            if (!is_line_good(L1, L2, L3)) {
                hull.pop_front();
                continue;
            }
            L1 = hull[sz - 1], L2 = hull[0], L3 = hull[sz - 2];
            if (check_empty(L1, L2, L3))
                return Response{Response::TYPE::EMPTY, {}};
            if (!is_line_good(L1, L2, L3)) {
                hull.pop_back();
                continue;
            }
            L1 = hull[sz - 2], L2 = hull[sz - 1], L3 = hull[(2 * sz - 3) % sz];
            if (check_empty(L1, L2, L3))
                return Response{Response::TYPE::EMPTY, {}};
            if (!is_line_good(L1, L2, L3)) {
                swap(hull[sz - 1], hull[sz - 2]);
                hull.pop_back();
                continue;
            }
            break;
        }
        if ((int) hull.size() == 2 && check_empty(hull[0], hull[1], hull[1]))
            return Response{Response::TYPE::EMPTY, {}};
    }
    is_inf |= !any_positive_vm;
    vector<Line> res(hull.begin(), hull.end());
    return Response{is_inf ? Response::TYPE::INF : Response::TYPE::FINITE, res};
}
ld calculate_area(Response response) {
    assert(response.type != Response::TYPE::INF);
    if (response.type == Response::TYPE::EMPTY)
        return 0;
    const auto& halfs = response.halfs;
    int n = (int) halfs.size();
    ld area = 0;
    vector<pt> ps;
    for (int i = 0; i < n; i++) {
        int j = i + 1 < n ? i + 1 : 0;
        if (abs(halfs[i].dir().vector_mul(halfs[j].dir())) < eps)
            continue;
        ps.push_back(intersect_lines(halfs[i], halfs[j]).second);
    }
    n = (int) ps.size();
    for (int i = 0; i < n; i++) {
        int j = i + 1 < n ? i + 1 : 0;
        area += ps[i].vector_mul(ps[j]);
    }
    return abs(area) / 2;
}
Line get_line(ld A, ld B, ld C) { // Ax + By + C >= 0
    pt v = {B, -A}, a, b;
    assert(A * A + B * B > eps);
    if (abs(v.x) >= eps) {
        a = {0, -C / B};
    } else {
        a = {-C / A, 0};
    }
    b = a + v;
    return Line{a, b};
}
