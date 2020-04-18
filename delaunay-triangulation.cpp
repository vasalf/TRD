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
struct Face;
struct Edge;
struct Node;

struct pt {
    int x = 0, y = 0;

    pt() = default;

    pt operator-(const pt &o) const {
        return {x - o.x, y - o.y};
    }

    ll vector_mul(const pt &o) const {
        return 1ll * x * o.y - 1ll * o.x * y;
    }

    bool isOn(const pt &a, const pt &b) const {
        if (x < min(a.x, b.x) || x > max(a.x, b.x))
            return false;

        if (y < min(a.y, b.y) || y > max(a.y, b.y))
            return false;

        return (b - a).vector_mul(*this - a) == 0;
    }

    /// polygon should be defined with clockwise or counter-clockwise bypass
    /// polygon should be strictly convex
    ///
    /// checks whether point lies in polygon
    bool isIn(const vector<pt> &polygon, bool strictly = false) const {
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
        return 1ll * x * x + 1ll * y * y;
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

    void flip() {
        // check and flip if needed
        // TODO
    }
};

struct Face {
    Edge *e1;
    Edge *e2;
    Edge *e3;

    vector<Node*> get_nodes() const {
        unordered_set<Node*> nodes;

        nodes.insert(e1->u);
        nodes.insert(e1->v);

        nodes.insert(e2->u);
        nodes.insert(e2->v);

        nodes.insert(e3->u);
        nodes.insert(e3->v);

        return vector<Node*>(nodes.begin(), nodes.end());
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

    pair<Edge*, Edge*> get_two_others(Edge *e) {
        if (e == e1)
            return make_pair(e2, e3);
        else if(e == e2)
            return make_pair(e1, e3);
        else if (e == e3)
            return make_pair(e1, e2);
        else
            assert(false);
    }
};

/// nullptr is the infinity point
struct Node {
    pt p{};
    unordered_set<Edge*> edges;
    Node *down = 0; // link to the node in previous layer
    int id = -1;

    Node() = default;

    Node(const pt &p, int id) : p(p), id(id) {}

    bool isOn(Edge *e) const {
        if (!e)
            return false;

        if (!e->u || !e->v)
            return false; // nothing is lying on edge (*, inf)

        return p.isOn(e->u->p, e->v->p);
    }

    bool isIn(Face *f, bool strictly = false) const {
        if (!f)
            return false;

        vector<pt> points;
        for (const Node *v : f->get_nodes()) {
            if (v)
                points.push_back(v->p);
            else
                return true;
        }

        return p.isIn(std::vector<pt>(points), strictly);
    }

    void insert_edge(Edge *e) {
        edges.insert(e);
    }

    void erase_edge(Edge *e) {
        edges.erase(e);
    }
};

/// Connect all important links
void create_face(Face *f, Edge *e1, Edge *e2, Edge *e3) {
    e1->insert_face(f);
    e2->insert_face(f);
    e3->insert_face(f);

    f->e1 = e1;
    f->e2 = e2;
    f->e3 = e3;
}

void create_edge(Edge *e) {
    if (e->u)
        e->u->insert_edge(e);
    if (e->v)
        e->v->insert_edge(e);
}

void destroy_edge(Edge *e) {
    if (e->u)
        e->u->erase_edge(e);
    if (e->v)
        e->v->erase_edge(e);
}

/// there should not be two equal points at the same time
/// there should be at least two points
///
/// probability for node to be pushed to the upper layer
void build_delanay(const vector<pt> &ps, double probability = 0.5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    int n = (int) ps.size();

    vector<vector<Node*>> layers; // localization structure

    for (int i = 0; i < n; i++) {
        const pt &q = ps[i];

        int n_layers = 1;
        while (dis(gen) < probability)
            n_layers++;

        Node *nearest_node = 0;
        ll nearest_dist = 4 * inf64;

        vector<Node*> added_nodes;

        for (int j = (int) layers.size() - 1; j >= 0; j--) {

            if (j < n_layers) {
                // find face, insert and flip
                Node *node = new Node(q, i);
                if (layers[j].size() == 1) {
                    Node *second_node = layers[j][0];

                    Edge *e1 = new Edge{node, 0, 0, 0};
                    Edge *e2 = new Edge{second_node, 0, 0, 0};
                    Edge *edge = new Edge{node, second_node, 0, 0};

                    Face *f1 = new Face();
                    Face *f2 = new Face();

                    create_face(f1, e1, e2, edge);
                    create_face(f2, e1, e2, edge);
                } else {
                    // how to find face? TODO

                    Face *face = 0;

                    for (Node *v : layers[j]) {
                        for (Edge *e : v->edges) {
                            if (node->isIn(e->f1))
                                face = e->f1;
                            if (node->isIn(e->f2))
                                face = e->f2;
                        }
                    }

                    assert(face);

                    // TODO
                    if (node->isIn(face, true)) {
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

                        create_edge(e1);
                        create_edge(e2);
                        create_edge(e3);

                        create_face(f1, e1, e2, t1);
                        create_face(f2, e2, e3, t2);
                        create_face(f3, e1, e3, t3);

                        t1->flip();
                        t2->flip();
                        t3->flip();
                    } else {
                        // insert in edge

                        Edge *e = 0;

                        if (node->isOn(face->e1))
                            e = face->e1;
                        else if (node->isOn(face->e2))
                            e = face->e2;
                        else if (node->isOn(face->e3))
                            e = face->e3;

                        assert(e);

                        Face *f1 = e->f1;
                        Face *f2 = e->f2;

                        assert(f1);
                        assert(f2);

                        Edge *g = new Edge{node, e->v, 0, 0};

                        Node *A, *B;
                        for (Node *x : f1->get_nodes()) {
                            if (x != e->u && x != e->v)
                                A = x;
                        }
                        for (Node *x : f2->get_nodes()) {
                            if (x != e->u && x != e->v)
                                B = x;
                        }

                        f1->erase_me_from_edges();
                        f2->erase_me_from_edges();

                        Face *f3 = new Face();
                        Face *f4 = new Face();

                        Edge *t1 = new Edge{node, A};
                        Edge *t2 = new Edge{node, B};

                        create_edge(t1);
                        create_edge(t2);

                        destroy_edge(e);
                        e->v = node;
                        create_edge(e);
                        create_edge(g);

                        Edge *z1, *z2, *z3, *z4;

                        tie(z1, z2) = f1->get_two_others(e);
                        tie(z3, z4) = f2->get_two_others(e);

                        create_face(f1, z1, e, t1);
                        create_face(f3, z2, g, t1);
                        create_face(f2, z3, e, t2);
                        create_face(f4, z4, g, t2);

                        z1->flip();
                        z2->flip();
                        z3->flip();
                        z4->flip();
                    }
                }

                layers[j].push_back(node);
                added_nodes.push_back(node);
            }

            if (j > 0) { // let's go to the lower layer
                if (!nearest_node) {
                    for (Node *candidate : layers.back()) {
                        ll candidate_dist = (candidate->p - q).sqr_norm();
                        if (candidate_dist < nearest_dist) {
                            nearest_dist = candidate_dist;
                            nearest_node = candidate;
                        }
                    }
                } else {
                    nearest_node = nearest_node->down;
                    while (1) {
                        ll best_dist = nearest_dist;
                        Node *best_node = nearest_node;
                        for (const Edge *edge : nearest_node->edges) {
                            Node *candidate = edge->to(nearest_node);
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
                    }
                }
            }
        }

        reverse(added_nodes.begin(), added_nodes.end());

        while ((int) layers.size() < n_layers) {
            Node *node = new Node(q, i);
            layers.push_back({node});
            added_nodes.push_back(node);
        }

        for (int j = 1; j < (int) added_nodes.size(); j++) {
            added_nodes[j]->down = added_nodes[j - 1];
        }
    }
}

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
        // TODO: id
    }

    return 0;
}
