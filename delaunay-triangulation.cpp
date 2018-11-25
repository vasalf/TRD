#ifdef debug
#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>
#include "optimization.h"

using namespace std;

#define ALL(x) (x).begin(), (x).end()
#define vec vector

typedef unsigned int ui;
typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< long long, long long > pll;

const int inf = 1e9;
const ll inf64 = 1e18;

namespace RANDOM {
    template< class T >
    T next_rand(T l, T r, mt19937& rnd) {
        return rnd() % (r - l + 1) + l;
    }
}

namespace geom {

    struct pt {
        enum SortType {ANGLE_SORT, CRD_SORT};

        ll x, y;
        int id;
        SortType sort_type;

        pt():
            x(0), y(0), id(-1), sort_type(CRD_SORT)
        { }

        pt(ll _x, ll _y, int _id = -1, SortType _sort_type = CRD_SORT):
            x(_x), y(_y), id(_id), sort_type(_sort_type)
        { }

        pt(const pt& o):
            x(o.x), y(o.y), id(o.id), sort_type(o.sort_type)
        { }

        tuple< ll, ll, int > get_tuple() const {
            return make_tuple(x, y, id);
        }

        tuple< double, ll, ll, ll, int > get_angle_tuple() const {
            return make_tuple(get_angle(), module_sqr(), x, y, id);
        }

        bool operator<(const pt& o) const {
            bool res;
            switch(sort_type) {
            case CRD_SORT:
                res = (get_tuple() < o.get_tuple());
                break;
            case ANGLE_SORT:
                res = (get_angle_tuple() < o.get_angle_tuple());
                break;
            }
            return res;
        }

        bool operator==(const pt& o) const {
            return get_tuple() == o.get_tuple();
        }

        pt operator-() const {
            return pt(-x, -y, id);
        }

        pt operator+(const pt& o) const {
            return pt(x + o.x, y + o.y);
        }

        pt operator-(const pt& o) const {
            return pt(x - o.x, y - o.y);
        }

        ll module_sqr() const {
            return sqr(x) + sqr(y);
        }

        double module() const {
            return sqrt(module_sqr());
        }

        ll dist_sqr(const pt& o) const {
            return (*this - o).module_sqr();
        }

        double dist(const pt& o) const {
            return (*this - o).module();
        }

        ll scalar_mul(const pt& o) const {
            return x * o.x + y * o.y;
        }

        ll vector_mul(const pt& o) const {
            return x * o.y - o.x * y;
        }

        void read(int _id = -1) {
            id = _id;
            x = readInt();
            y = readInt();
        }

        double get_angle() const {
            return atan2(y, x);
        }

        double get_angle(const pt& o) const {
            double tmp = 1.0 * scalar_mul(o) / (module() * o.module());
            if(tmp > 1) tmp = 1.0;
            if(tmp < -1) tmp = -1.0;
            return acos(tmp);
        }

        static pt gen_point(mt19937& rnd, int lx, int rx, int ly, int ry) {
            pt res;
            res.x = RANDOM::next_rand(lx, rx, rnd);
            res.y = RANDOM::next_rand(ly, ry, rnd);
            return res;
        }

    private:

        ll sqr(ll vl) const {
            return vl * vl;
        }

    };

    const double eps = 1e-9;

    ostream& operator<<(ostream& os, const pt& p) {
        os << p.x << " " << p.y;
        return os;
    }

    // ! tested !
    // get_neigh by angle from vector <stop - center>
    pt get_neigh(const pt& center, const pt& stop, set< pt >& neigh, bool is_prev) {
        bool is_next = !is_prev;

        pt tmp = stop - center;
        tmp.sort_type = pt::ANGLE_SORT;
        set< pt >::iterator fnd, nxt, prv;

        fnd = neigh.lower_bound(tmp);

        double res_angle = 1e9;
        pt res;

        auto relax = [&](const pt& candidate) {
            double tmp_angle = candidate.get_angle(tmp);
            ll vec_prod = candidate.vector_mul(tmp);
            if(((is_prev && vec_prod > 0) || (is_next && vec_prod < 0)) &&
                (
                 res.id == -1 ||
                 fabs(tmp_angle) < res_angle ||
                 (fabs(fabs(tmp_angle) - res_angle) < eps && res.module_sqr() > candidate.module_sqr())
                )
            ) {
                res_angle = fabs(tmp_angle);
                res = candidate;
            }
        };

        if(!neigh.empty()) {
            relax(*neigh.begin());
            nxt = next(neigh.begin());
            if(nxt != neigh.end()) {
                relax(*nxt);
            }
            relax(*(--neigh.end()));
            prv = --neigh.end();
            if(prv != neigh.begin()) {
                prv = prev(prv);
                relax(*prv);
            }
        }

        if(fnd != neigh.end()) {
            relax(*fnd);
            nxt = fnd != neigh.end() ? next(fnd) : neigh.end();
            prv = fnd != neigh.begin() && fnd != neigh.end() ? prev(fnd) : neigh.end();
            if(nxt != neigh.end()) {
                relax(*nxt);
                nxt = next(nxt);
                if(nxt != neigh.end()) {
                    relax(*nxt);
                }
            }
            if(prv != neigh.end()) {
                relax(*prv);
                if(prv != neigh.end() && prv != neigh.begin()) {
                    prv = prev(prv);
                    relax(*prv);
                }
            }
        }

        return res;
    }

    void test_neigh() {

        mt19937 rnd(42);
        const int MINN = 100;
        const int MAXN = 200;
        const int MINX = 0;
        const int MAXX = 30000;

        while(1) {
            int n = RANDOM::next_rand(MINN, MAXN, rnd);

            set< pt > have;
            vec< pt > res;

            for(int it = 0;it < n;it++) {
                while(1) {
                    pt tmp = pt::gen_point(rnd, MINX, MAXX, MINX, MAXX);
                    if(have.count(tmp)) {
                        continue;
                    }
                    have.insert(tmp);
                    res.push_back(tmp);
                    break;
                }
            }

            for(int i = 0;i < (int)res.size();i++) {
                for(int j = 0;j < (int)res.size();j++) {
                    if(i == j) continue;
                    assert(!(res[i] == res[j]));
                }
            }

            int ch = 1;

            for(int i = 0;i < (int)res.size() && ch;i++) {
                for(int j = i + 1;j < (int)res.size() && ch;j++) {
                    for(int o = j + 1;o < (int)res.size();o++) {
                        if((res[o] - res[j]).vector_mul(res[i] - res[j]) == 0) {
                            ch = 0;
                            break;
                        }
                    }
                }
            }

            if(!ch) continue;
//            cout << ch << "\n";
//            continue;

            have.clear();

            for(int i = 1;i < n;i++) {
                pt tmp = res[i] - res[0];
                tmp.id = res[i].id = i + 1;
                tmp.sort_type = pt::ANGLE_SORT;
                have.insert(tmp);
            }

            int ok = 1;

            for(int i = 0;i < n;i++) {
                pt tmp = pt::gen_point(rnd, MINX, MAXX, MINX, MAXX);
                int best = -1;
                double ang = 1e9;
                ll md = inf64;
                pt best_pt;
                for(pt p : have) {
                    double tang = p.get_angle(tmp - res[0]);
                    ll vec_prod = p.vector_mul(tmp - res[0]);
                    if(vec_prod < 0 && (best == -1 || ang > tang || fabs(ang - tang) < 1e-9 && md > p.module_sqr())) {
                        ang = tang;
                        best_pt = p + res[0];
                        best = p.id;
                        md = p.module_sqr();
                    }
                }
                pt fnd = get_neigh(res[0], tmp, have, false);
                if(1 || fnd.id != best) {
                    cout << "WA !\n";
                    cout << "expected = " << best << "\n";
                    cout << "found = " << fnd.id << "\n";
                    cout << "founded angle = " << ang << "\n";
                    cout << best_pt.x << " " << best_pt.y << " wtf\n";
                    cout << "md = " << md << "\n";
                    cout << tmp.x << " " << tmp.y << " q\n";
                    ok = 0;
                    get_neigh(res[0], tmp, have, true);
                    break;
                }
            }

            if(ok) {
                cout << "OK\n";
                continue;
            }

            for(int i = 0;i < n;i++) {
                cout << res[i] << " " << i + 1 << "\n";
            }
//            cout << "-------\n";

            for(int i = 1;i < n;i++) {
                pt tmp = res[i] - res[0];
                tmp.id = res[i].id = i + 1;
                tmp.sort_type = pt::ANGLE_SORT;
                have.insert(tmp);
            }

            for(pt p : have) {
                cout << "Segment " << (p + res[0]).x << " " << (p + res[0]).y << " " << res[0].x << " " << res[0].y << "\n";
            }

            for(pt p : have) {
                cout << p.id << " ";
            }

            cout << "\n";

            break;
        }
    }
}

const int N = 5e4 + 5;

void change_directed_edge( // p1 ---> p2
    geom::pt p1, geom::pt p2, char type,
    vec< set< geom::pt > >& g, /* graph-result */
    vec< set< geom::pt > > sorted_by_angle_g // sorted_by_angle of vector (v, to) graph-result
) {
    geom::pt tmp;

    {
        tmp = p2;
        tmp.sort_type = geom::pt::CRD_SORT;
        tmp.id = p2.id;
        if(type == '+') {
            g[p1.id].insert(tmp);
        }else {
            g[p1.id].erase(tmp);
        }
    }

    {
        tmp = p2 - p1;
        tmp.sort_type = geom::pt::ANGLE_SORT;
        tmp.id = p2.id;
        if(type == '+') {
            sorted_by_angle_g[p1.id].insert(tmp);
        }else {
            sorted_by_angle_g[p1.id].erase(tmp);
        }
    }
}

void change_edge(
    geom::pt p1, geom::pt p2, char type,
    vec< set< geom::pt > >& g, /* graph-result */
    vec< set< geom::pt > > sorted_by_angle_g // sorted_by_angle of vector (v, to) graph-result
) {
    change_directed_edge(p1, p2, type, g, sorted_by_angle_g);
    change_directed_edge(p2, p1, type, g, sorted_by_angle_g);
}

void delaunay(
    const vec< geom::pt >& db, /* global data base of points */
    vec< geom::pt > pts, /* points in current stage */
    vec< set< geom::pt > >& g, /* graph-result */
    vec< set< geom::pt > > sorted_by_angle_g // sorted_by_angle of vector (v, to) graph-result
) {

    int n = (int)pts.size();

    if(n <= 3) {
        for(int j, i = 0;i < 3;i++) {
            j = i; if(++j == 3) j = 0;
            change_edge(
                pts[i], pts[j], '+',
                g, sorted_by_angle_g
            );
        }
        return;
    }

    sort(ALL(pts));

    int n1 = n / 2;
    int n2 = n - n1;

    vec< geom::pt > left_pts(pts.begin(), pts.begin() + n1);
    vec< geom::pt > right_pts(pts.begin() + n1, pts.end());

    assert((int)left_pts.size() == n1);
    assert((int)right_pts.size() == n2);

    delaunay(db, left_pts, g, sorted_by_angle_g);
    delaunay(db, right_pts, g, sorted_by_angle_g);

    int L = -1; // id of lowest-rightest of left part
    int R = -1; // id of lowest-leftest of right part

    // find L
    {
        for(geom::pt lpt : left_pts) {
            if((lpt.y < db[L].y) ||
               (lpt.y == db[L].y && lpt.x > db[L].x)) {
                L = lpt.id;
            }
        }
    }

    // find R
    {
        for(geom::pt rpt : right_pts) {
            if((rpt.y < db[R].y) ||
               (rpt.y == db[R].y && rpt.x < db[R].x)) {
                R = rpt.id;
            }
        }
    }

    while(L != -1 && R != -1) {
        change_edge(
            db[L], db[R], '+',
            g, sorted_by_angle_g
        );
    }
}

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

//    int n;

//    n = readInt();

//    vec< geom::pt > pts(n);
//    vec< set< int > > g(n);

//    for(int i = 0;i < n;i++) {
//        pts[i].read(i);
//    }

    geom::test_neigh();

    return 0;
}
