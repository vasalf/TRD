struct circle {
    pt c;
    ld r = 0;

    ld area() const {
        return pi * r * r;
    }
};

vector<pair<pt, pt>> get_tangents(circle w1, circle w2) {
    if (w1.r > w2.r) // IMPORTANT!!!
        swap(w1, w2);
    ld D = (w1.c - w2.c).norm();
    if (D < abs(w1.r - w2.r) + eps) // ONE INSIDE ANOTHER!!!
        return {};
    ld cos_alpha = abs(w1.r - w2.r) / D;
    ld sin_alpha = sqrtl(max(ld(0), 1 - cos_alpha * cos_alpha));
    vector<pair<pt, pt>> ts;
    pt v1 = (w1.c - w2.c).rotate(cos_alpha, +sin_alpha);
    pt v2 = (w1.c - w2.c).rotate(cos_alpha, -sin_alpha);
    ts.emplace_back(w1.c + (v1 * (w1.r / v1.norm())), w2.c + (v1 * (w2.r / v1.norm())));
    ts.emplace_back(w1.c + (v2 * (w1.r / v2.norm())), w2.c + (v2 * (w2.r / v2.norm())));
    return ts;
}
