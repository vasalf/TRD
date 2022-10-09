bool check_point_in_polygon(vector<pt> P, pt q) {
    int n = (int) P.size();
    for (int i = 0; i < n; i++) {
        if ((P[i] - q).norm() < eps) return true;
        int j = (i + 1) % n;
        if (check_point_on_segment(P[i], P[j], q)) return true;
    }
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        pt A = P[i];
        pt B = P[j];
        if (A.y > B.y) swap(A, B);
        if (abs(A.y - B.y) < eps) continue;
        if (q.y + eps <= A.y || q.y + eps > B.y) continue;
        if ((B - A).vector_mul(q - A) + eps <= 0) continue;
        cnt ^= 1;
    }
    return cnt == 1;
}