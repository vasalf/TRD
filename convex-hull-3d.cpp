const ld eps = 1e-9;
struct plane;
struct pt {
    ld x = 0;
    ld y = 0;
    ld z = 0;
    int id = 0;

    pt operator - (const pt& o) const {
        return {x - o.x, y - o.y, z - o.z};
    }

    pt operator + (const pt& o) const {
        return {x + o.x, y + o.y, z + o.z};
    }

    pt operator - () const {
        return {-x, -y, -z};
    }

    pt operator * (ld coef) const {
        return {x * coef, y * coef, z * coef};
    }

    ld scalar_mul(const pt& o) const {
        return x * o.x + y * o.y + z * o.z;
    }

    ld sqr_norm() const {
        return scalar_mul(*this);
    }

    ld norm() const {
        return sqrtl(max(ld(0), sqr_norm()));
    }

    static ld det(ld a, ld b, ld c, ld d) {
        return a * d - b * c;
    }

    pt vector_mul(const pt& o) const {
        return {det(y, z, o.y, o.z), -det(x, z, o.x, o.z), det(x, y, o.x, o.y)};
    }

    tuple<ld, ld, ld> to_tuple() const {
        return {x, y, z};
    }

    pt any_ord() const {
        if (abs(x) >= eps || abs(y) >= eps) {
            return {-y, x, 0};
        } else if (abs(z) >= eps) {
            return {0, z, -y};
        } else {
            return {1, 0, 0};
        }
    }

    string to_string() const {
        stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }

    pt normalize() const {
        ld len = norm();
        return *this * (1 / len);
    }
};

struct plane {
    pt O;
    pt v;
    vector<int> ps;
};

ld sign(const plane& w, const pt& q) {
    return (q - w.O).scalar_mul(w.v);
}

bool on_plane(const plane& w, const pt& q) {
    return abs(sign(w, q)) < eps;
}

plane get_plane(ld a, ld b, ld c, ld d) {
    // ax + by + cz + d >= 0 <=> <v, {x, y, z} - O> >= 0
    pt O;
    if (abs(a) > eps) {
        O = {-d / a, 0, 0};
    } else if (abs(b) > eps) {
        O = {0, -d / b, 0};
    } else if (abs(c) > eps) {
        O = {0, 0, -d / c};
    } else {
        throw;
    }
    pt v = {a, b, c};
    return plane{O, v, {}};
}

plane get_plane(const pt& a, const pt& b, const pt& c) {
    return plane{a, (b - a).vector_mul(c - a), {}};
}

vector<plane> convex_hull_3d(vector<pt> a) {
    int n = (int) a.size();
    if (n == 0) return {};
    vector<plane> hull;
    {
        for (int i = 1; i < n; i++) {
            if ((a[0] - a[i]).norm() >= eps) {
                swap(a[i], a[1]);
                break;
            }
        }
        if (1 >= n || (a[0] - a[1]).norm() < eps) return {};
        for (int i = 2; i < n; i++) {
            if ((a[1] - a[0]).vector_mul(a[i] - a[0]).norm() >= eps) {
                swap(a[i], a[2]);
                break;
            }
        }
        if (2 >= n || (a[1] - a[0]).vector_mul(a[2] - a[0]).norm() < eps) return {};
        plane w = get_plane(a[0], a[1], a[2]);
        w.ps = {0, 1, 2};
        for (int i = 3; i < n; i++) {
            if (!on_plane(w, a[i])) {
                swap(a[i], a[3]);
                break;
            }
        }
        if (3 >= n || on_plane(w, a[3])) {
            for (int i = 1; i + 1 < (int) a.size(); i++) {
                plane h = get_plane(a[0], a[i], a[i + 1]);
                h.ps = {a[0].id, a[i].id, a[i + 1].id};
                hull.push_back(h);
            }
            return hull;
        }
        if (sign(w, a[3]) <= -eps) w.v = -w.v;
        hull.push_back(w);
        for (int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            int k = 3 ^ i ^ j;
            plane h = get_plane(a[i], a[j], a[3]);
            if (sign(h, a[k]) <= -eps) h.v = -h.v;
            h.ps = {i, j, 3};
            hull.push_back(h);
        }
    }
    auto in_hull = [&](const pt& q) -> bool {
        for (const auto& w : hull) if (sign(w, q) <= -eps) return false;
        return true;
    };
    vector<vector<int>> last_see(a.size(), vector<int>(a.size())), last_not_see(a.size(), vector<int>(a.size()));
    int timer = 10;
    auto add = [&](int nid) {
        pt q = a[nid];
        timer++;
        for (const auto& w : hull) {
            if (sign(w, q) <= -eps) {
                // see
                for (int i = 0; i < 3; i++) {
                    int j = (i + 1) % 3;
                    int id_i = w.ps[i];
                    int id_j = w.ps[j];
                    last_see[id_i][id_j] = last_see[id_j][id_i] = timer;
                }
            } else {
                // not see
                for (int i = 0; i < 3; i++) {
                    int j = (i + 1) % 3;
                    int id_i = w.ps[i];
                    int id_j = w.ps[j];
                    last_not_see[id_i][id_j] = last_not_see[id_j][id_i] = timer;
                }
            }
        }
        vector<plane> next_hull;
        for (const auto& w : hull) {
            if (sign(w, q) <= -eps) {
                const auto& ar = w.ps;
                int sz = (int) ar.size();
                assert(sz == 3);
                for (int i = 0; i < sz; i++) {
                    int j = (i + 1) % sz;
                    int id_i = ar[i];
                    int id_j = ar[j];
                    int id_k = ar[3 ^ i ^ j];
                    if (last_see[id_i][id_j] == timer && last_not_see[id_i][id_j] == timer) {
                        plane add_plane = get_plane(q, a[id_i], a[id_j]);
                        add_plane.ps = {nid, id_i, id_j};
                        if (sign(add_plane, a[id_k]) <= -eps) add_plane.v = -add_plane.v;
                        next_hull.push_back(add_plane);
                    }
                }
            } else {
                next_hull.push_back(w);
            }
        }
        swap(hull, next_hull);
    };
    for (int i = 0; i < n; i++)
        if (!in_hull(a[i])) add(i);
    for (auto& w : hull) {
        for (auto& id : w.ps)
            id = a[id].id;
    }
    return hull;
}
