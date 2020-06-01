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

struct pt;
struct Node;
struct Face;
struct Edge;

vector<pair<ll, pii>> edges;
std::mt19937 gen(42);

const int N = 3.5e5 + 5;

struct pt {
    int x = 0, y = 0, id = 0;

    pt() = default;

    pt operator-(const pt &o) const {
        return {x - o.x, y - o.y};
    }

    ll vector_mul(const pt &o) const {
        return 1ll * x * o.y - 1ll * o.x * y;
    }

    ll scalar_mul(const pt &o) const {
        return 1ll * x * o.x + 1ll * y * o.y;
    }

    bool onSegment(const pt &a, const pt &b) const {
        if (x < min(a.x, b.x) || x > max(a.x, b.x))
            return false;

        if (y < min(a.y, b.y) || y > max(a.y, b.y))
            return false;

        return (b - a).vector_mul(*this - a) == 0;
    }

    /// polygon should be defined with clockwise or counter-clockwise bypass
    ///
    /// checks whether point lies in polygon
    bool inPolygon(const vector<pt> &polygon, bool strictly = false) const {
        int sz = (int) polygon.size();
        bool positive = false;
        bool negative = false;
        bool zero = false;
        for (int i = 0; i < sz; i++) {
            int j = i + 1 < sz ? i + 1 : 0;
            ll vm = (polygon[j] - polygon[i]).vector_mul(*this - polygon[i]);
            if (vm > 0) positive = true;
            if (vm < 0) negative = true;
            if (vm == 0) zero = true;
        }
        return !(positive && negative) && (!strictly || !zero);
    }

    ll sqr_norm() const {
        return this->scalar_mul(*this);
    }
};

struct Edge {
    int u = -1;
    int v = -1;
    int f1 = -1;
    int f2 = -1;
    int id = -1;

    int to(int from) const {
        return u == from ? v : u;
    }

    void erase_face(int f) {
        if (f1 == f)
            f1 = -1;
        else if (f2 == f)
            f2 = -1;
    }

    void insert_face(int f) {
        if (f1 == -1)
            f1 = f;
        else if (f2 == -1)
            f2 = f;
    }

    void change_face(int was, int will) {
        if (f1 == was)
            f1 = will;
        else
            f2 = will;
    }

    void flip();

    bool is_inf() const {
        return u == -1 || v == -1;
    }

    void visit() const;
};

Edge EDGES[N];
int edges_sz = 0;

int new_edge(int u = -1, int v = -1, int f1 = -1, int f2 = -1) {
    EDGES[edges_sz] = {u, v, f1, f2, edges_sz};
    return edges_sz++;
}

int buff[6], TT, QQ;

struct Face {
    int e1 = -1;
    int e2 = -1;
    int e3 = -1;
    int id = -1;

    vector<int> get_nodes() const {
        buff[0] = EDGES[e1].u;
        buff[1] = EDGES[e1].v;
        buff[2] = EDGES[e2].u;
        buff[3] = EDGES[e2].v;
        buff[4] = EDGES[e3].u;
        buff[5] = EDGES[e3].v;
        sort(buff, buff + 6);
        TT = unique(buff, buff + 6) - buff;
        return vector<int>(buff, buff + TT);
    }

    void change_edge(int was, int will) {
        if (e1 == was)
            e1 = will;
        else if (e2 == was)
            e2 = will;
        else
            e3 = will;
    }

    static bool check_edge(int e, int u, int v) {
        return (EDGES[e].u == u && EDGES[e].v == v)
            || (EDGES[e].u == v && EDGES[e].v == u);
    }

    int get_edge(int u, int v) {
        if (check_edge(e1, u, v))
            return e1;
        if (check_edge(e2, u, v))
            return e2;
        if (check_edge(e3, u, v))
            return e3;
        return -1;
    }

    void erase_me_from_edges() {
        EDGES[e1].erase_face(id);
        EDGES[e2].erase_face(id);
        EDGES[e3].erase_face(id);
    }

    pii get_two_other_edges(int e) {
        if (e == e1)
            return make_pair(e2, e3);
        else if(e == e2)
            return make_pair(e1, e3);
        else if (e == e3)
            return make_pair(e1, e2);
        else
            assert(false);
    }

    int get_one_other_node(int u, int v) {
        for (int x : get_nodes())
            if (x != u && x != v)
                return x;
        return -1;
    }

    bool is_inf() const {
        return EDGES[e1].is_inf() || EDGES[e2].is_inf() || EDGES[e3].is_inf();
    }
};

Face FACES[N];
int faces_sz = 0;

int new_face(int e1 = -1, int e2 = -1, int e3 = -1) {
    FACES[faces_sz] = {e1, e2, e3, faces_sz};
    return faces_sz++;
}

/// nullptr is the infinity point
struct Node {
    pt p{};
    unordered_set<int> edges;
    int down = -1; // link to the node in previous layer

    Node() = default;

    Node(const pt &_p) : p(_p) {}

    bool onEdge(int e) const;

    bool inFace(int f, bool strictly) const;

    void insert_edge(int e) {
        edges.insert(e);
    }

    void erase_edge(int e) {
        edges.erase(e);
    }
};

Node NODES[N];
int nodes_sz = 0;

int new_node(pt p) {
    NODES[nodes_sz] = {p};
    return nodes_sz++;
}

bool Node::onEdge(int e) const {
    if (e == -1)
        return false;

    int x = EDGES[e].u;
    int y = EDGES[e].v;

    if (x == -1 || y == -1)
        return false; // nothing is lying on edge (*, inf)

    return p.onSegment(NODES[x].p, NODES[y].p);
}

bool Node::inFace(int f, bool strictly = false) const {
    if (f == -1)
        return false;

    vector<pt> points;
    for (int v : FACES[f].get_nodes()) {
        if (v != -1)
            points.push_back(NODES[v].p);
        else
            return false; // !!!
    }

    return p.inPolygon(std::vector<pt>(points), strictly);
}

/// Connect all important links
void create_face_links(int f, int e1, int e2, int e3) {
    EDGES[e1].insert_face(f);
    EDGES[e2].insert_face(f);
    EDGES[e3].insert_face(f);

    FACES[f].e1 = e1;
    FACES[f].e2 = e2;
    FACES[f].e3 = e3;
}

void create_edge_links(int e) {
    int x = EDGES[e].u;
    int y = EDGES[e].v;
    if (x != -1)
        NODES[x].insert_edge(e);
    if (y != -1)
        NODES[y].insert_edge(e);
}

void destroy_edge_links(int e) {
    int x = EDGES[e].u;
    int y = EDGES[e].v;
    if (x != -1)
        NODES[x].erase_edge(e);
    if (y != -1)
        NODES[y].erase_edge(e);
}

void Edge::flip() {
    // TODO: what to do with inf?
    // now, we can consider that edge connected to the inf is good
    // but we should add points from convex hull first
    if (is_inf() || f1 == -1 || f2 == -1)
        return;

    int A = FACES[f1].get_one_other_node(u, v);
    int B = FACES[f2].get_one_other_node(u, v);
    if (A == -1 || B == -1)
        return;

    ld vc_a = abs((NODES[u].p - NODES[A].p).vector_mul(NODES[v].p - NODES[A].p));
    ld sc_a = (NODES[u].p - NODES[A].p).scalar_mul(NODES[v].p - NODES[A].p);

    ld vc_b = abs((NODES[u].p - NODES[B].p).vector_mul(NODES[v].p - NODES[B].p));
    ld sc_b = (NODES[u].p - NODES[B].p).scalar_mul(NODES[v].p - NODES[B].p);

    if (vc_a * sc_b + sc_a * vc_b >= 0) // sin(alpha + beta) >= 0, OVERFLOWING, but long double should does job
        return;

    int z1, z2, z3, z4;

    z1 = FACES[f1].get_edge(u, A);
    z2 = FACES[f1].get_edge(v, A);
    z3 = FACES[f2].get_edge(u, B);
    z4 = FACES[f2].get_edge(v, B);

    FACES[f1].change_edge(z2, z3);
    FACES[f2].change_edge(z3, z2);

    EDGES[z2].change_face(f1, f2);
    EDGES[z3].change_face(f2, f1);

    NODES[u].erase_edge(id);
    NODES[v].erase_edge(id);

    NODES[A].insert_edge(id);
    NODES[B].insert_edge(id);

    u = A;
    v = B;

    EDGES[z1].flip();
    EDGES[z2].flip();
    EDGES[z3].flip();
    EDGES[z4].flip();
}

void Edge::visit() const {
    if (u != -1 && v != -1) {
        ll dist = (NODES[u].p - NODES[v].p).sqr_norm();
        edges.push_back({dist, {NODES[u].p.id, NODES[v].p.id}});
    }
}

struct Delaunay {
    vector<vector<int>> layers; // localization structure

    // hull should be in clockwise or in counter-clockwise order, has at least three points and strictly positive area
    vector<int> add_convex_hull(int j, const vector<pt> &hull) {
        int n = (int) hull.size();
        assert(n >= 3);
        int last_edge = -1;
        vector<int> nodes(n);
        vector<int> edges(n);
        for (int i = 0; i < n; i++)
            nodes[i] = new_node(hull[i]);
        {
            int e1 = new_edge(nodes[0], nodes[1]);
            create_edge_links(e1);
            int e2 = new_edge(nodes[1], nodes[2]);
            create_edge_links(e2);
            int e3 = new_edge(nodes[2], nodes[0]);
            create_edge_links(e3);
            int f = new_face();
            create_face_links(f, e1, e2, e3);
            last_edge = e3;

            edges[0] = e1;
            edges[1] = e2;
            edges[2] = e3;
        }
        for (int i = 3; i < n; i++) {
            int e1 = new_edge(nodes[i - 1], nodes[i]);
            create_edge_links(e1);
            int e2 = new_edge(nodes[i], nodes[0]);
            create_edge_links(e2);
            int f = new_face();
            create_face_links(f, e1, e2, last_edge);
            EDGES[last_edge].flip();
            last_edge = e2;
            edges[i] = last_edge;
        }
        vector<int> inf_edges(n);
        for (int i = 0; i < n; i++) {
            inf_edges[i] = new_edge(nodes[i]);
            create_edge_links(inf_edges[i]);
        }
        for (int i = 0; i < n; i++) {
            int ii = (i + 1) % n;
            int f = new_face();
            create_face_links(f, inf_edges[i], inf_edges[ii], edges[i]);
        }
        while ((int) layers.size() <= j)
            layers.emplace_back();
        layers[j].clear();
        for (int i = 0; i < n; i++)
            layers[j].push_back(nodes[i]);
        return nodes;
    }

    void add_point_into_face(int node, int face) {
        // insert in triangle

        vector<int> vs = FACES[face].get_nodes();
        int v1, v2, v3;

        v1 = vs[0], v2 = vs[1], v3 = vs[2];

        int e1, e2, e3;
        int f1, f2, f3;

        e1 = new_edge(node, v1);
        e2 = new_edge(node, v2);
        e3 = new_edge(node, v3);

        f1 = face;
        f2 = new_face();
        f3 = new_face();

        int t1 = FACES[face].get_edge(v1, v2);
        int t2 = FACES[face].get_edge(v2, v3);
        int t3 = FACES[face].get_edge(v1, v3);

        EDGES[t1].erase_face(face);
        EDGES[t2].erase_face(face);
        EDGES[t3].erase_face(face);

        create_edge_links(e1);
        create_edge_links(e2);
        create_edge_links(e3);

        create_face_links(f1, e1, e2, t1);
        create_face_links(f2, e2, e3, t2);
        create_face_links(f3, e1, e3, t3);

        EDGES[t1].flip();
        EDGES[t2].flip();
        EDGES[t3].flip();
    }

    void add_point_into_edge(int node, int e) {
        // insert in edge
        // we assume that insertion point is strictly inside the edge

        assert(e != -1);

        int f1 = EDGES[e].f1;
        int f2 = EDGES[e].f2;

        assert(f1 != -1);
        assert(f2 != -1);

        int z1, z2, z3, z4;
        tie(z1, z2) = FACES[f1].get_two_other_edges(e);
        tie(z3, z4) = FACES[f2].get_two_other_edges(e);

        if (EDGES[z1].u != EDGES[e].u && EDGES[z1].v != EDGES[e].u)
            swap(z1, z2);
        if (EDGES[z3].u != EDGES[e].u && EDGES[z3].v != EDGES[e].u)
            swap(z3, z4);

        int g = new_edge(node, EDGES[e].v);

        int A = FACES[f1].get_one_other_node(EDGES[e].u, EDGES[e].v);
        int B = FACES[f2].get_one_other_node(EDGES[e].u, EDGES[e].v);

        FACES[f1].erase_me_from_edges();
        FACES[f2].erase_me_from_edges();

        int f3 = new_face();
        int f4 = new_face();

        int t1 = new_edge(node, A);
        int t2 = new_edge(node, B);

        create_edge_links(t1);
        create_edge_links(t2);

        destroy_edge_links(e);
        EDGES[e].v = node;
        create_edge_links(e);
        create_edge_links(g);

        create_face_links(f1, z1, e, t1);
        create_face_links(f3, z2, g, t1);
        create_face_links(f2, z3, e, t2);
        create_face_links(f4, z4, g, t2);

        EDGES[z1].flip();
        EDGES[z2].flip();
        EDGES[z3].flip();
        EDGES[z4].flip();
    }

    int find_face(int j, int nearest_node, int node) {
        while (1) {
            int v = -1, ee = -1;
            for (int edge : NODES[nearest_node].edges) {
                int to = EDGES[edge].to(nearest_node);
                if (to == -1)
                    continue;
                if ((NODES[node].p - NODES[nearest_node].p).vector_mul(NODES[to].p - NODES[nearest_node].p) <= 0) {
                    if (v == -1
                    || (NODES[to].p - NODES[nearest_node].p).vector_mul(NODES[v].p - NODES[nearest_node].p) < 0)
                        v = to, ee = edge;
                }
            }
            nearest_node = v;
            int x = EDGES[ee].f1;
            int y = EDGES[ee].f2;
            if (x != -1 && NODES[node].inFace(x))
                return x;
            if (y != -1 && NODES[node].inFace(y))
                return y;
        }
    }

    // point should be inside the convex hull of all points which will be added
    int add_point_into_layer(const pt &q, int j, int nearest_node) {
        // find face, insert and flip
        int node = new_node(q);
        int face = find_face(j, nearest_node, node);

        if (NODES[node].inFace(face, true)) { // strictly inside
            add_point_into_face(node, face);
        } else {
            int e = -1;

            if (NODES[node].onEdge(FACES[face].e1))
                e = FACES[face].e1;
            else if (NODES[node].onEdge(FACES[face].e2))
                e = FACES[face].e2;
            else if (NODES[node].onEdge(FACES[face].e3))
                e = FACES[face].e3;

            add_point_into_edge(node, e);
        }

        layers[j].push_back(node);
        return node;
    }

    void update_nearest_node(int j, const pt &q, int &nearest_node) {
        if (NODES[nearest_node].down != -1)
            nearest_node = NODES[nearest_node].down;
        else
            nearest_node = layers[j - 1].front();
        ll nearest_dist = (NODES[nearest_node].p - q).sqr_norm();
        while (1) {
            ll best_dist = nearest_dist;
            int best_node = nearest_node;
            for (int edge : NODES[nearest_node].edges) {
                int candidate = EDGES[edge].to(nearest_node);
                if (candidate == -1)
                    continue;
                ll candidate_dist = (NODES[candidate].p - q).sqr_norm();
                if (candidate_dist < best_dist) {
                    best_dist = candidate_dist;
                    best_node = candidate;
                }
            }
            if (nearest_node == best_node)
                break;
            nearest_dist = best_dist;
            nearest_node = best_node;
        }
    }

    /// returns the highest node
    int add_point(const pt &q, int n_layers) {
        int nearest_node = -1;
        ll nearest_dist = 4 * inf64;

        vector<int> added_nodes;

        for (int candidate : layers.back()) {
            ll candidate_dist = (NODES[candidate].p - q).sqr_norm();
            if (candidate_dist < nearest_dist) {
                nearest_dist = candidate_dist;
                nearest_node = candidate;
            }
        }

        for (int j = (int) layers.size() - 1; j >= 0; j--) {
            if (j < n_layers) {
                int node = add_point_into_layer(q, j, nearest_node);
                added_nodes.push_back(node);
            }

            if (j > 0) { // let's go to the lower layer
                update_nearest_node(j, q, nearest_node);
            }
        }

        reverse(added_nodes.begin(), added_nodes.end());

        // update links to the previous layers
        for (int j = 1; j < (int) added_nodes.size(); j++) {
            NODES[added_nodes[j]].down = added_nodes[j - 1];
        }

        return added_nodes.back();
    }

    void push_links_down() {
        unordered_map<int, int> last_link;
        for (int j = 0; j < (int) layers.size(); j++) {
            for (int node : layers[j]) {
                auto fnd = last_link.find(NODES[node].p.id);
                if (fnd != last_link.end())
                    NODES[node].down = fnd->second;
                last_link[NODES[node].p.id] = node;
            }
        }
    }
};

/// returns vector of indices
vector<int> build_convex_hull(const vector<pt> &ps) {
    int n = (int) ps.size();
    vector<int> h(n);
    for (int i = 0; i < n; i++)
        h[i] = i;
    {
        int j = 0;
        for (int i = 1; i < n; i++) {
            if (ps[i].x < ps[j].x || (ps[i].x == ps[j].x && ps[i].y < ps[j].y))
                j = i;
        }
        swap(h[j], h[0]);
    }
    sort(h.begin() + 1, h.end(), [&](int i, int j) {
        pt v1 = ps[i] - ps[h[0]];
        pt v2 = ps[j] - ps[h[0]];
        ll value = v1.vector_mul(v2);
        if (value != 0)
            return value > 0;
        return v1.sqr_norm() < v2.sqr_norm();
    });
    vector<int> st = {h[0]};
    int nn = -1;
    for (int iter = 1; iter < n; iter++) {
        int i = h[iter];
        const pt &p1 = ps[i];
        const pt &p2 = ps[h[n - 1]];
        if ((p1 - ps[h[0]]).vector_mul(p2 - ps[h[0]]) == 0) {
            nn = iter;
            break;
        }
    }
    reverse(h.begin() + nn,  h.end());
    for (int iter = 1; iter <= nn; iter++) {
        int i = h[iter];
        while ((int) st.size() >= 2) {
            const pt &p1 = ps[st[(int) st.size() - 2]];
            const pt &p2 = ps[st[(int) st.size() - 1]];
            const pt &p3 = ps[i];
            if ((p3 - p2).vector_mul(p1 - p2) >= 0)
                break;
            st.pop_back();
        }
        st.push_back(i);
    }
    for (int iter = nn + 1; iter < n; iter++)
        st.push_back(h[iter]);
    st.insert(st.begin(), st.back());
    st.pop_back();
    return st;
}

/// there should not be two equal points at the same time
/// there should be at least three points
/// convex hull of points should have positive area
///
/// probability for node to be pushed to the upper layer
///
/// let n be the number of points then ids should be in integers in [0, n)
void build_delanay(const vector<pt> &ps, double probability = 0.5) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    Delaunay delaunay;

    int n = (int) ps.size();
    vector<int> n_layers(n);
    vector<vector<pt>> layers;

    for (int i = 0; i < n; i++) {
        n_layers[ps[i].id] = 1;
        while (dis(gen) < probability)
            n_layers[ps[i].id]++;
        while ((int) layers.size() < n_layers[ps[i].id])
            layers.emplace_back();
        for (int j = 0; j < n_layers[ps[i].id]; j++)
            layers[j].push_back(ps[i]);
    }

    while ((int) layers.back().size() <= 2) {
        for (const pt &q : layers.back())
            n_layers[q.id]--;
        layers.pop_back();
    }

    vector<int> downest_link_among_convex_hulls(n, -1);

    for (int j = (int) layers.size() - 1; j >= 0; j--) {
        vector<int> hull = build_convex_hull(layers[j]);
        for (int i : hull)
            n_layers[layers[j][i].id]--;
        vector<pt> hull_pts(hull.size());
        for (int it = 0; it < (int) hull.size(); it++)
            hull_pts[it] = layers[j][hull[it]];
        vector<int> helper = delaunay.add_convex_hull(j, hull_pts);
        for (int i = 0; i < (int) helper.size(); i++) {
            downest_link_among_convex_hulls[NODES[helper[i]].p.id] = helper[i];
        }
    }
    delaunay.push_links_down();

    for (int i = 0; i < n; i++) {
        if (n_layers[ps[i].id] == 0)
            continue;
        if (downest_link_among_convex_hulls[ps[i].id] == -1)
            continue;
        int top = delaunay.add_point(ps[i], n_layers[ps[i].id]);
        NODES[downest_link_among_convex_hulls[ps[i].id]].down = top;
    }

    for (int i = 0; i < n; i++) {
        if (n_layers[ps[i].id] == 0)
            continue;
        if (downest_link_among_convex_hulls[ps[i].id] != -1)
            continue;
        delaunay.add_point(ps[i], n_layers[ps[i].id]);
    }

    for (int v : delaunay.layers[0]) {
        for (int edge : NODES[v].edges) {
            if (!EDGES[edge].is_inf() && EDGES[edge].u == v)
                EDGES[edge].visit();
        }
    }
}

struct DSU {
    int n; // [0, n)
    vector<int> p;

    DSU(int nn) {
        n = nn;
        p.resize(n);
        for (int i = 0; i < n; i++)
            p[i] = i;
    }

    int find(int x) {
        return x == p[x] ? x : find(p[x]);
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y)
            return false;
        p[x] = y;
        return true;
    }
};

int main() {

#ifdef DEBUG
    freopen("input.txt", "r", stdin);
#endif

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<pt> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i].x >> p[i].y;
        p[i].id = i;
    }

//    build_delanay(p, 0.2);
    build_delanay(p, n <= 10000 ? 0.5 : n <= 30000 ? 0.05 : 0.1);

    sort(edges.begin(), edges.end());

    DSU dsu(n);

    ld res = 0;
    vector<pii> es;

    for (auto t : edges) {
        int u, v;
        tie(u, v) = t.second;
        if (dsu.merge(u, v)) {
            res += sqrt(t.first);
            es.push_back({u, v});
        }
    }

    cout.precision(20);
    cout << fixed;

    cout << res << "\n";

    for (auto t : es) {
        cout << t.first + 1 << " " << t.second + 1 << "\n";
    }

    return 0;
}
