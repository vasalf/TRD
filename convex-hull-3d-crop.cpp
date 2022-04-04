vector<plane> convex_hull_3d(vector<pt> a) {
    int n = (int) a.size(), timer = 10;
    vector<plane> hull;
    { /** NB: initial corner cases (find first 4 no coplonar points) **/ }
    auto in_hull = [&](const pt& q) -> bool {
        for (const auto& w : hull) if (sign(w, q) <= -eps) return false;
        return true;
    };
    vector<vector<int>> last_see(n, vector<int>(n)), last_not_see(n, vector<int>(n));
    auto add = [&](int nid) {
        pt q = a[nid]; timer++;
        for (const auto& w : hull) {
            auto& ar = sign(w, q) <= -eps ? last_see : last_not_see;
            for (int i = 0; i < 3; i++) {
                int j = (i + 1) % 3, id_i = w.ps[i], id_j = w.ps[j];
                ar[id_i][id_j] = ar[id_j][id_i] = timer;
            }
        }
        vector<plane> next_hull;
        for (const auto& w : hull) {
            if (sign(w, q) <= -eps) {
                const auto& ar = w.ps;
                int sz = (int) ar.size();
                assert(sz == 3);
                for (int i = 0; i < sz; i++) {
                    int j = (i + 1) % sz, id_i = ar[i], id_j = ar[j], id_k = ar[3 ^ i ^ j];
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
    for (int i = 0; i < n; i++) if (!in_hull(a[i])) add(i);
    for (auto& w : hull) for (auto& id : w.ps) id = a[id].id;
    return hull;
}