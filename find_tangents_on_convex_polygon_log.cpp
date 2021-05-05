pair<pt, pt> find_tangents_on_convex_polygon(const vector<pt>& P, pt q) {
    // q is strictly outside P
    // P in counter-clockwise order
    // P.size >= 3
    // P is strictly convex
    int n = (int) P.size();
    auto is_visible_edge = [&](int i) -> bool {
        int j = (i + 1) % n;
        ld x = (P[i] - q).vector_mul(P[j] - P[i]);
        return x < -eps;
    };
    auto is_visible = [&](int i) -> bool {
        return is_visible_edge(i) || is_visible_edge((i - 1 + n) % n);
    };
    auto is_on_right = [&](int i) -> bool {
        return (P[0] - q).vector_mul(P[i] - q) < -eps;
    };
    int bl, br, bm;
    int A = -1, B = -1;
    if (is_visible(0)) {
        bl = 0, br = n;
        while (br - bl > 1) {
            bm = (bl + br) >> 1;
            if (is_visible(bm) && is_on_right(bm)) bl = bm;
            else br = bm;
        }
        B = bl;

        bl = 0, br = n;
        while (br - bl > 1) {
            bm = (bl + br) >> 1;
            int i = (B + n - bm) % n;
            if (is_visible(i)) bl = bm;
            else br = bm;
        }
        A = (B + n - bl) % n;
    } else {
        bl = 0, br = n;
        while (br - bl > 1) {
            bm = (bl + br) >> 1;
            if (!is_visible(bm) && !is_on_right(bm)) bl = bm;
            else br = bm;
        }
        A = (bl + 1) % n;

        bl = 0, br = n;
        while (br - bl > 1) {
            bm = (bl + br) >> 1;
            int i = (A + bm) % n;
            if (is_visible(i)) bl = bm;
            else br = bm;
        }
        B = (A + bl) % n;
    }
    return {P[A], P[B]};
}
