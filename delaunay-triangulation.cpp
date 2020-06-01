

#include "optimization.h"
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
    Node *u = 0;
    Node *v = 0;
    Face *f1 = 0;
    Face *f2 = 0;

    Node* to(Node *from) const {
        return u == from ? v : u;
    }

    void erase_face(Face *f) {
        if (f1 == f)
            f1 = 0;
        else if (f2 == f)
            f2 = 0;
    }

    void insert_face(Face *f) {
        if (f1 == 0)
            f1 = f;
        else if (f2 == 0)
            f2 = f;
    }

    void change_face(Face *was, Face *will) {
        if (f1 == was)
            f1 = will;
        else
            f2 = will;
    }

    void flip();

    bool is_inf() const {
        return !u || !v;
    }

    void visit() const;
};

Node* buff[6];
int TT, QQ;

struct Face {
    Edge *e1;
    Edge *e2;
    Edge *e3;

    vector<Node*> get_nodes() const {
        buff[0] = e1->u;
        buff[1] = e1->v;
        buff[2] = e2->u;
        buff[3] = e2->v;
        buff[4] = e3->u;
        buff[5] = e3->v;
        sort(buff, buff + 6);
        TT = unique(buff, buff + 6) - buff;
        return vector<Node*>(buff, buff + TT);
    }

    void change_edge(Edge *was, Edge *will) {
        if (e1 == was)
            e1 = will;
        else if (e2 == was)
            e2 = will;
        else
            e3 = will;
    }

    static bool check_edge(const Edge *e, Node *u, Node *v) {
        return (e->u == u && e->v == v) || (e->u == v && e->v == u);
    }

    Edge* get_edge(Node *u, Node *v) {
        if (check_edge(e1, u, v))
            return e1;
        if (check_edge(e2, u, v))
            return e2;
        if (check_edge(e3, u, v))
            return e3;
        return 0;
    }

    void erase_me_from_edges() {
        e1->erase_face(this);
        e2->erase_face(this);
        e3->erase_face(this);
    }

    pair<Edge*, Edge*> get_two_other_edges(Edge *e) {
        if (e == e1)
            return make_pair(e2, e3);
        else if(e == e2)
            return make_pair(e1, e3);
        else if (e == e3)
            return make_pair(e1, e2);
        else
            assert(false);
    }

    Node *get_one_other_node(Node *u, Node *v) {
        for (Node *x : get_nodes())
            if (x != u && x != v)
                return x;
        return 0;
    }

    bool is_inf() const {
        return e1->is_inf() || e2->is_inf() || e3->is_inf();
    }
};

/// nullptr is the infinity point
struct Node {
    pt p{};
    unordered_set<Edge*> edges;
    Node *down = 0; // link to the node in previous layer

    Node() = default;

    Node(const pt &_p) : p(_p) {}

    bool onEdge(const Edge *e) const {
        if (!e)
            return false;

        if (!e->u || !e->v)
            return false; // nothing is lying on edge (*, inf)

        return p.onSegment(e->u->p, e->v->p);
    }

    bool inFace(Face *f, bool strictly = false) const {
        if (!f)
            return false;

        vector<pt> points;
        for (const Node *v : f->get_nodes()) {
            if (v)
                points.push_back(v->p);
            else
                return false; // !!!
        }

        return p.inPolygon(std::vector<pt>(points), strictly);
    }

    void insert_edge(Edge *e) {
        edges.insert(e);
    }

    void erase_edge(Edge *e) {
        edges.erase(e);
    }
};

/// Connect all important links
void create_face_links(Face *f, Edge *e1, Edge *e2, Edge *e3) {
    e1->insert_face(f);
    e2->insert_face(f);
    e3->insert_face(f);

    f->e1 = e1;
    f->e2 = e2;
    f->e3 = e3;
}

void create_edge_links(Edge *e) {
    if (e->u)
        e->u->insert_edge(e);
    if (e->v)
        e->v->insert_edge(e);
}

void destroy_edge_links(Edge *e) {
    if (e->u)
        e->u->erase_edge(e);
    if (e->v)
        e->v->erase_edge(e);
}

void Edge::flip() {
    // TODO: what to do with inf?
    // now, we can consider that edge connected to the inf is good
    // but we should add points from convex hull first
    if (is_inf() || !f1 || !f2)
        return;

    Node *A = f1->get_one_other_node(u, v);
    Node *B = f2->get_one_other_node(u, v);
    if (!A || !B)
        return;

    ld vc_a = abs((u->p - A->p).vector_mul(v->p - A->p));
    ld sc_a = (u->p - A->p).scalar_mul(v->p - A->p);

    ld vc_b = abs((u->p - B->p).vector_mul(v->p - B->p));
    ld sc_b = (u->p - B->p).scalar_mul(v->p - B->p);

    if (vc_a * sc_b + sc_a * vc_b >= 0) // sin(alpha + beta) >= 0, OVERFLOWING, but long double should does job
        return;

    Edge *z1, *z2, *z3, *z4;

    z1 = f1->get_edge(u, A);
    z2 = f1->get_edge(v, A);
    z3 = f2->get_edge(u, B);
    z4 = f2->get_edge(v, B);

    f1->change_edge(z2, z3);
    f2->change_edge(z3, z2);

    z2->change_face(f1, f2);
    z3->change_face(f2, f1);

    u->erase_edge(this);
    v->erase_edge(this);
    A->insert_edge(this);
    B->insert_edge(this);

    u = A;
    v = B;

    z1->flip();
    z2->flip();
    z3->flip();
    z4->flip();
}

void Edge::visit() const {
//    if (u) u->p.visit();
//    else cout << "inf";
//    cout << " ";
//    if (v) v->p.visit();
//    else cout << "inf";
//    cout << "\n";
    if (u && v) {
        ll dist = (u->p - v->p).sqr_norm();
        edges.push_back({dist, {u->p.id, v->p.id}});
    }
}

struct Delaunay {
    vector<vector<Node*>> layers; // localization structure

    // hull should be in clockwise or in counter-clockwise order, has at least three points and strictly positive area
    vector<Node*> add_convex_hull(int j, const vector<pt> &hull) {
        int n = (int) hull.size();
        assert(n >= 3);
        Edge *last_edge = 0;
        vector<Node*> nodes(n);
        vector<Edge*> edges(n);
        for (int i = 0; i < n; i++)
            nodes[i] = new Node(hull[i]);
        {
            Edge *e1 = new Edge{nodes[0], nodes[1], 0, 0};
            create_edge_links(e1);
            Edge *e2 = new Edge{nodes[1], nodes[2], 0, 0};
            create_edge_links(e2);
            Edge *e3 = new Edge{nodes[2], nodes[0], 0, 0};
            create_edge_links(e3);
            Face *f = new Face();
            create_face_links(f, e1, e2, e3);
            last_edge = e3;

            edges[0] = e1;
            edges[1] = e2;
            edges[2] = e3;
        }
        for (int i = 3; i < n; i++) {
            Edge *e1 = new Edge{nodes[i - 1], nodes[i], 0, 0};
            create_edge_links(e1);
            Edge *e2 = new Edge{nodes[i], nodes[0], 0, 0};
            create_edge_links(e2);
            Face *f = new Face();
            create_face_links(f, e1, e2, last_edge);
            last_edge->flip();
            last_edge = e2;
            edges[i] = last_edge;
        }
        vector<Edge*> inf_edges(n);
        for (int i = 0; i < n; i++) {
            inf_edges[i] = new Edge{nodes[i], 0, 0, 0};
            create_edge_links(inf_edges[i]);
        }
        for (int i = 0; i < n; i++) {
            int ii = (i + 1) % n;
            Face *f = new Face();
            create_face_links(f, inf_edges[i], inf_edges[ii], edges[i]);
        }
        while ((int) layers.size() <= j)
            layers.emplace_back();
        layers[j].clear();
        for (int i = 0; i < n; i++)
            layers[j].push_back(nodes[i]);
        return nodes;
    }

    void add_point_into_face(Node *node, Face *face) {
        // insert in triangle

        vector<Node*> vs = face->get_nodes();
        Node *v1, *v2, *v3;

        v1 = vs[0], v2 = vs[1], v3 = vs[2];

        Edge *e1, *e2, *e3;
        Face *f1, *f2, *f3;

        e1 = new Edge{node, v1, 0, 0};
        e2 = new Edge{node, v2, 0, 0};
        e3 = new Edge{node, v3, 0, 0};

        f1 = face;
        f2 = new Face();
        f3 = new Face();

        Edge *t1 = face->get_edge(v1, v2);
        Edge *t2 = face->get_edge(v2, v3);
        Edge *t3 = face->get_edge(v1, v3);

        t1->erase_face(face);
        t2->erase_face(face);
        t3->erase_face(face);

        create_edge_links(e1);
        create_edge_links(e2);
        create_edge_links(e3);

        create_face_links(f1, e1, e2, t1);
        create_face_links(f2, e2, e3, t2);
        create_face_links(f3, e1, e3, t3);

        t1->flip();
        t2->flip();
        t3->flip();
    }

    void add_point_into_edge(Node *node, Edge *e) {
        // insert in edge
        // we assume that insertion point is strictly inside the edge

        assert(e);

        Face *f1 = e->f1;
        Face *f2 = e->f2;

        assert(f1);
        assert(f2);

        Edge *z1, *z2, *z3, *z4;
        tie(z1, z2) = f1->get_two_other_edges(e);
        tie(z3, z4) = f2->get_two_other_edges(e);

        if (z1->u != e->u && z1->v != e->u)
            swap(z1, z2);
        if (z3->u != e->u && z3->v != e->u)
            swap(z3, z4);

        Edge *g = new Edge{node, e->v, 0, 0};

        Node *A = f1->get_one_other_node(e->u, e->v);
        Node *B = f2->get_one_other_node(e->u, e->v);

        f1->erase_me_from_edges();
        f2->erase_me_from_edges();

        Face *f3 = new Face();
        Face *f4 = new Face();

        Edge *t1 = new Edge{node, A};
        Edge *t2 = new Edge{node, B};

        create_edge_links(t1);
        create_edge_links(t2);

        destroy_edge_links(e);
        e->v = node;
        create_edge_links(e);
        create_edge_links(g);

        create_face_links(f1, z1, e, t1);
        create_face_links(f3, z2, g, t1);
        create_face_links(f2, z3, e, t2);
        create_face_links(f4, z4, g, t2);

        z1->flip();
        z2->flip();
        z3->flip();
        z4->flip();
    }

    Face *find_face(int j, Node *nearest_node, Node *node) {
//        int tmp = 0;
        while (1) {
            Node *v = 0;
            for (const Edge *edge : nearest_node->edges) {
                if (edge->f1 && node->inFace(edge->f1))
                    return edge->f1;
                if (edge->f2 && node->inFace(edge->f2))
                    return edge->f2;
                Node *to = edge->to(nearest_node);
                if (!to)
                    continue;
                if ((node->p - nearest_node->p).vector_mul(to->p - nearest_node->p) < 0) {
                    if (!v || (to->p - nearest_node->p).vector_mul(v->p - nearest_node->p) < 0)
                        v = to;
                }
            }
            nearest_node = v;
//            tmp++;
//            if (tmp > 2)
//                cout << "find face steps: " << tmp << "\n";
        }
    }

    // point should be inside the convex hull of all points which will be added
    Node* add_point_into_layer(const pt &q, int j, Node *nearest_node) {
        // find face, insert and flip
        Node *node = new Node(q);
        Face *face = find_face(j, nearest_node, node);

        if (node->inFace(face, true)) { // strictly inside
            add_point_into_face(node, face);
        } else {
            Edge *e = 0;

            if (node->onEdge(face->e1))
                e = face->e1;
            else if (node->onEdge(face->e2))
                e = face->e2;
            else if (node->onEdge(face->e3))
                e = face->e3;

            add_point_into_edge(node, e);
        }

        layers[j].push_back(node);
        return node;
    }

    void update_nearest_node(int j, const pt &q, Node *& nearest_node) {
        if (nearest_node->down)
            nearest_node = nearest_node->down;
        else
            nearest_node = layers[j - 1].front();
        ll nearest_dist = (nearest_node->p - q).sqr_norm();
        while (1) {
            ll best_dist = nearest_dist;
            Node *best_node = nearest_node;
            for (const Edge *edge : nearest_node->edges) {
                Node *candidate = edge->to(nearest_node);
                if (!candidate)
                    continue;
                ll candidate_dist = (candidate->p - q).sqr_norm();
                if (candidate_dist < best_dist) {
                    best_dist = candidate_dist;
                    best_node = candidate;
                }
            }
            if (nearest_node == best_node)
                break;
            nearest_dist = best_dist;
            nearest_node = best_node;
//            tmp++;
        }
//        if (tmp > 4)
//            cout << "find nearest steps: " << tmp << "\n";
    }

    /// returns the highest node
    Node* add_point(const pt &q, int n_layers) {
        Node *nearest_node = 0;
        ll nearest_dist = 4 * inf64;

        vector<Node*> added_nodes;

        for (Node *candidate : layers.back()) {
            ll candidate_dist = (candidate->p - q).sqr_norm();
            if (candidate_dist < nearest_dist) {
                nearest_dist = candidate_dist;
                nearest_node = candidate;
            }
        }

        for (int j = (int) layers.size() - 1; j >= 0; j--) {
            if (j < n_layers) {
                Node *node = add_point_into_layer(q, j, nearest_node);
                added_nodes.push_back(node);
            }

            if (j > 0) { // let's go to the lower layer
                update_nearest_node(j, q, nearest_node);
            }
        }

        reverse(added_nodes.begin(), added_nodes.end());

        // update links to the previous layers
        for (int j = 1; j < (int) added_nodes.size(); j++) {
            added_nodes[j]->down = added_nodes[j - 1];
        }

        return added_nodes.back();
    }

    void push_links_down() {
        unordered_map<int, Node*> last_link;
        for (int j = 0; j < (int) layers.size(); j++) {
            for (Node *node : layers[j]) {
                node->down = last_link[node->p.id];
                last_link[node->p.id] = node;
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

    vector<Node*> downest_link_among_convex_hulls(n);

    for (int j = (int) layers.size() - 1; j >= 0; j--) {
        vector<int> hull = build_convex_hull(layers[j]);
        for (int i : hull)
            n_layers[layers[j][i].id]--;
        vector<pt> hull_pts(hull.size());
        for (int it = 0; it < (int) hull.size(); it++)
            hull_pts[it] = layers[j][hull[it]];
        vector<Node*> helper = delaunay.add_convex_hull(j, hull_pts);
        for (int i = 0; i < (int) helper.size(); i++) {
            downest_link_among_convex_hulls[helper[i]->p.id] = helper[i];
        }
    }
    delaunay.push_links_down();

    for (int i = 0; i < n; i++) {
        if (n_layers[ps[i].id] == 0)
            continue;
        if (downest_link_among_convex_hulls[ps[i].id] == 0)
            continue;
        Node *top = delaunay.add_point(ps[i], n_layers[ps[i].id]);
        if (downest_link_among_convex_hulls[ps[i].id]) {
            downest_link_among_convex_hulls[ps[i].id]->down = top;
        }
    }

    for (int i = 0; i < n; i++) {
        if (n_layers[ps[i].id] == 0)
            continue;
        if (downest_link_among_convex_hulls[ps[i].id] != 0)
            continue;
        Node *top = delaunay.add_point(ps[i], n_layers[ps[i].id]);
        if (downest_link_among_convex_hulls[ps[i].id]) {
            downest_link_among_convex_hulls[ps[i].id]->down = top;
        }
    }

    for (Node *v : delaunay.layers[0]) {
        for (Edge *edge : v->edges) {
            if (!edge->is_inf() && edge->u == v)
                edge->visit();
        }
    }
//    for (int j = 0; j < (int) layers.size(); j++) {
//        visit(delaunay.layers[j][0]);
//        DX += 50;
//        break;
//    }
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

//    ios_base::sync_with_stdio(0);
//    cin.tie(0);

    int n;
//    cin >> n;
    n = readInt();

    vector<pt> p(n);
    for (int i = 0; i < n; i++) {
//        cin >> p[i].x >> p[i].y;
        p[i].x = readInt();
        p[i].y = readInt();
        p[i].id = i;
    }

    build_delanay(p, n >= 40000 ? 0.02 : n >= 20000 ? 0.05 : 0.5);

    sort(edges.begin(), edges.end());

    DSU dsu(n);

    ld res = 0;
    vector<pii> es;

    for (auto p : edges) {
        int u, v;
        tie(u, v) = p.second;
        if (dsu.merge(u, v)) {
            res += sqrt(p.first);
            es.push_back({u, v});
        }
    }

//    cout.precision(20);
//    cout << fixed;

    writeDouble(res, 20);
    writeChar('\n');

    for (auto t : es) {
//        cout << t.first + 1 << " " << t.second + 1 << "\n";
        writeInt(t.first + 1, ' ');
        writeInt(t.second + 1, '\n');
    }

    return 0;
}
