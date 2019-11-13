#include <bits/stdc++.h>

//#undef debug

using namespace std;

#define vec vector
#define ALL(x) (x).begin(), (x).end()

typedef pair< int, int > pii;

const int inf = 1e9;

class Taxi;
class Query;
class Field;

namespace statistics {
    int cnt_processed_queries = 0;
    int max_w0 = -inf;
    int min_w0 = +inf;
    int sum_w0 = 0;
    int max_wait = -inf;
    int min_wait = +inf;
    int sum_wait = 0;
    int max_pay = -inf;
    int min_pay = +inf;
    int sum_pay = 0;
    unordered_map< Taxi*, int > how_many_people;
    unordered_map< double, int > cnt_of_pay;

    void output_statistics() {
        printf("\n---statistics---\n");
        int mn = +inf;
        int mx = -inf;
        int sm = 0;
        for(pair< Taxi*, int > iter : how_many_people) {
            mn = min(mn, iter.second);
            mx = max(mx, iter.second);
            sm += iter.second;
        }
        printf("max cnt = %d\n", mx);
        printf("min cnt = %d\n", mn);
        printf("cnt of proccesd queries = %d\n", cnt_processed_queries);
        printf("avg cnt = %.4lf\n", 1.0 * sm / (int)how_many_people.size());
        printf("zero pays = %d\n", cnt_of_pay[0]);
        printf("\n");
        printf("max w0 = %d\n", max_w0);
        printf("min w0 = %d\n", min_w0);
        printf("sum w0 = %d\n", sum_w0);
        printf("avg w0 = %.4lf\n", 100 + 1.0 * sum_w0 / max(1, cnt_processed_queries));
        printf("\n");
        printf("max wait = %d\n", max_wait);
        printf("min wait = %d\n", min_wait);
        printf("sum wait = %d\n", sum_wait);
        printf("avg wait = %.4lf\n", 1.0 * sum_wait / cnt_processed_queries);
        printf("\n");
        printf("max pay = %d\n", max_pay);
        printf("min pay = %d\n", min_pay);
        printf("sum_pay = %d\n", sum_pay);
        printf("avg pay = %.4lf\n", 1.0 * sum_pay / cnt_processed_queries);
        printf("---end---\n");
    }

    double get_avg_pay() {
        return 100 + 1.0 * sum_w0 / max(1, cnt_processed_queries);
    }
}

namespace GEOM {

int dist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int dist(pii p1, pii p2) {
    return dist(p1.first, p1.second, p2.first, p2.second);
}

pii get_middle(pii p1, pii p2) {
    return make_pair(
        (p1.first + p2.first) / 2,
        (p1.second + p2.second) / 2
    );
}

double _sqr(double x) {
    return x * x;
}

}

class Taxi {
public:
    Taxi();
    Taxi(
        vec< pair< pii, pair< Query*, int > > > _cmds,
        set< Query*> _accepted,
        int _current_time,
        int _x, int _y,
        int _current_cmd, double _score
    );
    void read();
    void exec_cmds(int check_point_time);
    int get_time() const;
    pii get_crd() const;
    void add_score(double delt_score);
    void listen_cmd(vec< pair< pii, pair< Query*, int > > > new_cmds);
    int get_num_of_passengers() const;
    vec< pair< pii, pair< Query*, int > > > get_remaining_queries() const;
    int get_remaining_queries_size() const;
    double get_score() const;
    int get_remaining_time() const;
    int get_dist_after_all_cmds(int tox, int toy) const;
    int get_dist_after_all_cmds(pii pr) const;
    pii get_crd_after_all_cmds() const;
    void cut_sequence();
    double best_graw_score_after_insert(Query *q, bool do_insert);
    bool exist_query(Query *q) const;
    void erase_zero_pay();
    double expected_score() const;
    void shake_seq(); // do some improvement
    void append_query(Query * q);
    Query* pop_back();
private:
    bool check_is_last_point();
    void finish_cmd();
    void do_tick(int check_point_time);
    void move_point(int &x1, int &y1, int x2, int y2, int path_len);
    int get_dist_on_segment(int l, int r, bool from_now) const;
    bool can_swap(int i, int j);
private:
    vec< pair< pii, pair< Query*, int > > > cmds;
    set< Query* > accepted; // people in my car
    int current_time; // my rolex =)
    int x, y; // where i am ?
    int current_cmd; // what i am doing now ? (not finished yet)
    double score; // my money =) (for rolex of course)
};

class Query {
public:
    enum STATUS {WAITING, KNOW_CAR, IN_CAR, FINISH};
public:
    Query();
    Query(Query *other);
    int get_time_of_phone() const;
    pii get_start_crd() const;
    pii get_finish_crd() const;
    void get_in_car(Taxi *car);
    void get_out_of_car(Taxi *car);
    // read info and create a new Query
    static Query* read_query(int _id) {
        Query *res = new Query();
        res->id = _id;
        res->read();
        return res;
    }
    void get_msg_from_taxi();
    void cancel_taxi();
    void add_to_black_list();
    STATUS get_status() const;
    int get_id() const;
    int get_w0() const;
    double will_pay(int arrival_time, int time_on_road) const;
    int get_start_time() const;
    int get_waiting_time() const;
private:
    double calc_score() const;
    void read();
private:
    int sx, sy; // start crd
    int tx, ty; // finish crd
    int t0;     // time of phone
    int start_time;
    int finish_time;
    int waiting_time;
    STATUS status;
    int id;
};

class Field {
public:
    Field();
    void read();
    void exec_cmds(int check_point_time);
    bool read_query();
    void give_cmds();
    int get_total_score() const;
private:
    void print_one_cmd_seq(int id, const vec< pair< pii, pair< Query *, int > > > &seq);
private:
    int W; // 1 <= x <= W
    int H; // 1 <= y <= H
    vec< Taxi* > cars;
    vec< Query* > global_queries;
};

Taxi::Taxi():
    current_time(0),
    x(0), y(0),
    current_cmd(0),
    score(0)
{ }

Taxi::Taxi(
    vec< pair< pii, pair< Query*, int > > > _cmds,
    set< Query*> _accepted,
    int _current_time,
    int _x, int _y,
    int _current_cmd, double _score
) :
    cmds(_cmds),
    accepted(_accepted),
    current_time(_current_time),
    x(_x), y(_y),
    current_cmd(_current_cmd),
    score(_score)
{ }

void Taxi::read() {
    scanf("%d %d", &x, &y);
}

void Taxi::exec_cmds(int check_point_time) {
    for(;current_time < check_point_time;) {
        Taxi::do_tick(check_point_time);
    }
}

int Taxi::get_time() const {
    return current_time;
}

pii Taxi::get_crd() const {
    return make_pair(x, y);
}

void Taxi::add_score(double delt_score) { // earn money
    score += delt_score;

    { // for statistics
        statistics::how_many_people[this]++;
        statistics::cnt_of_pay[delt_score]++;
    }
}

// listen for command (i trust)
void Taxi::listen_cmd(vec< pair< pii, pair< Query*, int > > > new_cmds) {
    cmds = new_cmds;
    current_cmd = 0;
}

int Taxi::get_num_of_passengers() const {
    return (int)accepted.size();
}

vec< pair< pii, pair< Query*, int > > > Taxi::get_remaining_queries() const {
    if(current_cmd < (int)cmds.size()) {
        return vec< pair< pii, pair< Query*, int > > >(cmds.begin() + current_cmd, cmds.end());
    }else {
        return {};
    }
}

int Taxi::get_remaining_queries_size() const {
    return (int)cmds.size() - current_cmd;
}

double Taxi::get_score() const {
    return score;
}

int Taxi::get_remaining_time() const {
    int result = 0;
    int cx, cy;
    cx = x;
    cy = y;
    for(int nx, ny, i = current_cmd;i < (int)cmds.size();i++) {
        tie(nx, ny) = cmds[i].first;
        result += abs(cx - nx);
        result += abs(cy - ny);
        cx = nx;
        cy = ny;
    }
    return result;
}

int Taxi::get_dist_after_all_cmds(int tox, int toy) const {
    if(current_cmd < (int)cmds.size()) {
        return GEOM::dist(cmds.back().first, make_pair(tox, toy));
    }else {
        return GEOM::dist(x, y, tox, toy);
    }
}

int Taxi::get_dist_after_all_cmds(pii pr) const {
    return Taxi::get_dist_after_all_cmds(pr.first, pr.second);
}

pii Taxi::get_crd_after_all_cmds() const {
    if(current_cmd < (int)cmds.size()) {
        return cmds.back().first;
    }else {
        return make_pair(x, y);
    }
}

void Taxi::cut_sequence() {
    if(current_cmd >= (int)cmds.size()) {
        current_cmd = 0;
        cmds.clear();
        return;
    }
    if(current_cmd > 0) {
        cmds.erase(cmds.begin(), cmds.begin() + current_cmd);
        current_cmd = 0;
    }
    while((int)cmds.size() > 0 && cmds.back().second.second == 0) {
        cmds.pop_back();
    }
}

double Taxi::best_graw_score_after_insert(Query *q, bool do_insert) {
    if(current_cmd == 0) {} else throw -1;

    if(cmds.empty()) {
        double w0 = q->get_w0();
        double d1 = GEOM::dist(Taxi::get_crd(), q->get_start_crd()); // waiting time
        double d2 = 0; // delt between our path and ideal path
        double alpha = (1e7 - min(GEOM::_sqr(d1) + GEOM::_sqr(d2), 1e7)) / 1e7;
        if(do_insert) {
            cmds.push_back(make_pair(q->get_start_crd(), make_pair(q, +1)));
            cmds.push_back(make_pair(q->get_finish_crd(), make_pair(q, -1)));
        }
        return alpha * (100 + w0);
    }

    int best_i = (int)cmds.size() - 1;
    int best_j = (int)cmds.size() - 1;
    double best_score = -inf, tmp_score;
    double before = Taxi::expected_score();

    for(int i = (int)cmds.size() - 1;i >= -1;i--) {
        for(int j = i;j < (int)cmds.size();j++) {
            cmds.insert(cmds.begin() + j + 1, make_pair(q->get_finish_crd(), make_pair(q, -1)));
            cmds.insert(cmds.begin() + i + 1, make_pair(q->get_start_crd(), make_pair(q, +1)));
            try{
                tmp_score = Taxi::expected_score() - before;
            } catch (...) {
                tmp_score = -1e9;
            }
            if(tmp_score > best_score) {
                best_score = tmp_score;
                best_i = i;
                best_j = j;
            }
            cmds.erase(cmds.begin() + j + 2);
            cmds.erase(cmds.begin() + i + 1);
        }
    }

    if(do_insert) {
        cmds.insert(cmds.begin() + best_j + 1, make_pair(q->get_finish_crd(), make_pair(q, -1)));
        cmds.insert(cmds.begin() + best_i + 1, make_pair(q->get_start_crd(), make_pair(q, +1)));
    }

    return best_score;
}

bool Taxi::exist_query(Query *q) const {
    for(auto i : Taxi::cmds) {
        if(i.second.first == q) {
            return true;
        }
    }
    return false;
}

void Taxi::erase_zero_pay() {
    if(current_cmd != 0) throw -1;
    vec< int > pos;
    for(int i = 0;i < (int)cmds.size();i++) {
        if(Taxi::accepted.count(cmds[i].second.first)) {
            continue;
        }
        if(cmds[i].second.second != +1) continue;
        int j = -1;
        for(int z = i + 1;z < (int)cmds.size();z++) {
            if(cmds[z].second.second == -1 &&
               cmds[z].second.first == cmds[i].second.first) {
                j = z;
                break;
            }
        }
        if(j == -1) throw -1;
        int arrival_time = Taxi::get_time() + Taxi::get_dist_on_segment(0, i, true);
        int time_on_road = Taxi::get_dist_on_segment(i, j, false);
        if(abs(cmds[i].second.first->will_pay(arrival_time, time_on_road)) <= 1e-8) {
            pos.push_back(i);
            pos.push_back(j);
        }
    }
    sort(ALL(pos));
    reverse(ALL(pos));

    for(int i : pos) {
        cmds.erase(cmds.begin() + i);
    }
}

double Taxi::expected_score() const {

    if(current_cmd != 0) throw -1;

    int glob_capacity = 4 - Taxi::get_num_of_passengers();
    int globx, globy;
    tie(globx, globy) = Taxi::get_crd();
    double glob_score = Taxi::get_score();
    int glob_time = Taxi::get_time();
    unordered_map< Query*, int > glob_start_time, glob_waiting_time;

    for(Query *q : Taxi::accepted) {
        glob_start_time[q] = q->get_start_time();
        glob_waiting_time[q] = q->get_waiting_time();
    }

    for(int i = 0;i < (int)cmds.size();i++) {
        { // move here
            glob_time += GEOM::dist({globx, globy}, cmds[i].first);
            tie(globx, globy) = cmds[i].first;
            if(cmds[i].second.second == -1) {
                glob_capacity++;
                if(glob_capacity > 4) throw -1;
                { // ad_score
                    double w0 = cmds[i].second.first->get_w0();
                    double d1 = glob_waiting_time[cmds[i].second.first];
                    double d2 = (glob_time - glob_start_time[cmds[i].second.first]) - w0;
                    double alpha = (1e7 - min(GEOM::_sqr(d1) + GEOM::_sqr(d2), 1e7)) / 1e7;
                    double ad_score = alpha * (100 + w0);
                    glob_score += ad_score;
                }
            }else if(cmds[i].second.second == +1) {
                if(glob_capacity <= 0) throw -1;
                glob_capacity--;
                glob_start_time[cmds[i].second.first] = glob_time;
                glob_waiting_time[cmds[i].second.first] = glob_time - cmds[i].second.first->get_time_of_phone();
            }
        }
    }

    return glob_score;
}

void Taxi::shake_seq() {
    if((int)cmds.size() < 2) return;
    static mt19937 rnd(37);
    double current_score = Taxi::expected_score();
    for(int any, iter = 0;iter < 200;iter++) {
        any = 0;
        for(int it = 0;it < 5 * (int)cmds.size();it++) {
            int i = rnd() % ((int)cmds.size() - 1);
            int j = i + rnd() % ((int)cmds.size() - i - 1) + 1;
            if(Taxi::can_swap(i, j)) {
                swap(cmds[i], cmds[j]);
                double tmp_score = -1e9;
                try {
                    tmp_score = Taxi::expected_score();
                } catch(...) {
                    tmp_score = -1e9;
                }
                if(tmp_score > current_score) {
                    any = 1;
                    current_score = tmp_score;
                }else {
                    swap(cmds[i], cmds[j]);
                }
            }
        }
        if(!any) break;
    }
}

void Taxi::append_query(Query * q) {
    Taxi::cmds.push_back(make_pair(q->get_start_crd(), make_pair(q, +1)));
    Taxi::cmds.push_back(make_pair(q->get_finish_crd(), make_pair(q, -1)));
}

Query* Taxi::pop_back() {
    if((int)cmds.size() > 2 && current_cmd + 2 < (int)cmds.size()) {
        int sz = (int)cmds.size();
        if(cmds[sz - 2].second.first == cmds[sz - 1].second.first) {
            Query *q = cmds[sz - 1].second.first;
            cmds.pop_back();
            cmds.pop_back();
            return q;
        }else {
            return nullptr;
        }
    }else {
        return nullptr;
    }
}

bool Taxi::can_swap(int i, int j) {
    if(current_cmd != 0) throw -1;
    if(0 <= i && i < j && j < (int)cmds.size()) {}
    else return false;
    int capacity = Taxi::get_num_of_passengers();
    swap(cmds[i], cmds[j]);
    int ok = 1;
    for(int jj = 0;jj < (int)cmds.size();jj++) {
        if(cmds[jj].second.second == 1) {
            if(capacity <= 0) {
                ok = 0;
                break;
            }else {
                capacity--;
            }
        }else if(cmds[jj].second.second == -1) {
            capacity++;
        }
    }
    if(!ok) {
        swap(cmds[i], cmds[j]);
        return false;
    }
    for(int w = 0;w < (int)cmds.size();w++) {
        if(cmds[w].second.second != +1) continue;
        int hm = 0;
        for(int z = w + 1;z < (int)cmds.size();z++) {
            if(cmds[z].second.second == -1 && cmds[z].second.first == cmds[w].second.first) {
                hm = 1;
                break;
            }
        }
        if(!hm) {
            swap(cmds[i], cmds[j]);
            return false;
        }
    }
    swap(cmds[i], cmds[j]);
    return true;
}

bool Taxi::check_is_last_point() {
    return (
        current_cmd < (int)cmds.size() &&
        cmds[current_cmd].first == get_crd()
    );
}

// we are in place
// just finish command (don't touch 'current_cmd')
void Taxi::finish_cmd() {
    if(check_is_last_point()) {} else throw -1;
//    assert(check_is_last_point());
    Query *q = cmds[current_cmd].second.first;
    if(cmds[current_cmd].second.second == 1) {
        if(q) {} else throw -1;
//        assert(q);
        if(!accepted.count(q)) {} else throw -1;
//        assert(!accepted.count(q));
        if(q->get_status() == Query::STATUS::KNOW_CAR) {} else throw -1;
//        assert(q->get_status() == Query::STATUS::KNOW_CAR);
        accepted.insert(q);
        if((int)accepted.size() <= 4) {} else throw -1;
//        assert((int)accepted.size() <= 4);
        q->get_in_car(this);
    }else if(cmds[current_cmd].second.second == -1) {
        if(q) {} else throw -1;
//        assert(q);
        if(accepted.count(q)) {} else throw -1;
//        assert(accepted.count(q));
        if(q->get_status() == Query::STATUS::IN_CAR) {} else throw -1;
//        assert(q->get_status() == Query::STATUS::IN_CAR);
        accepted.erase(q);
        q->get_out_of_car(this);
    }else {
        if(cmds[current_cmd].second.second == 0) {} else throw -1;
//        assert(cmds[current_cmd].second.second == 0);
        if(!q) {} else throw -1;
//        assert(!q);
    }
}

// just do operations upto 'check_point_time' on your rolex
void Taxi::do_tick(int check_point_time) {
    while(current_cmd < (int)cmds.size()) {
        if(cmds[current_cmd].second.second == 1) {
            Query* q = cmds[current_cmd].second.first;
            if(q) {} else throw -1;
//            assert(q);
            if(q->get_status() == Query::STATUS::WAITING) {
                q->get_msg_from_taxi();
            }
        }
        if(get_time() <= check_point_time) {} else throw -1;
//        assert(get_time() <= check_point_time);
        int remaining_path_len = GEOM::dist(Taxi::get_crd(), cmds[current_cmd].first);
        if(Taxi::get_time() + remaining_path_len <= check_point_time) {
            Taxi::move_point(
                x, y, cmds[current_cmd].first.first, cmds[current_cmd].first.second,
                remaining_path_len
            );
            current_time += remaining_path_len;
            finish_cmd();
            current_cmd++;
        }else {
            Taxi::move_point(
                x, y,
                cmds[current_cmd].first.first, cmds[current_cmd].first.second,
                check_point_time - Taxi::get_time()
            );
            current_time += check_point_time - Taxi::get_time();
            break;
        }
    }
    if(get_time() <= check_point_time) {} else throw -1;
//    assert(get_time() <= check_point_time);
    current_time = check_point_time;
}

void Taxi::move_point(int &x1, int &y1, int x2, int y2, int path_len) {
    if(0 <= path_len && path_len <= GEOM::dist(x1, y1, x2, y2)) {} else throw -1;
//    assert(0 <= path_len && path_len <= GEOM::dist(x1, y1, x2, y2));
    int dist_after = GEOM::dist(x1, y1, x2, y2) - path_len;
    // (x1, y1) -> (x2, y1) -> (x2, y2)
    if(path_len <= GEOM::dist(x1, y1, x2, y1)) {
        if(x1 < x2) {
            x1 += path_len;
        }else {
            x1 -= path_len;
        }
    }else {
        path_len -= GEOM::dist(x1, y1, x2, y1);
        x1 = x2;
        if(y1 < y2) {
            y1 += path_len;
        }else {
            y1 -= path_len;
        }
    }
    if(dist_after == GEOM::dist(x1, y1, x2, y2)) {} else throw -1;
//    assert(dist_after == GEOM::dist(x1, y1, x2, y2));
}

int Taxi::get_dist_on_segment(int l, int r, bool from_now) const {
    int result = 0;
    if(from_now) {
        result += GEOM::dist(Taxi::get_crd(), cmds[l].first);
    }
    for(int i = l;i < r;i++) {
        result += GEOM::dist(cmds[i].first, cmds[i + 1].first);
    }
    return result;
}

Query::Query():
    sx(0), sy(0),
    tx(0), ty(0),
    t0(0),
    start_time(0),
    finish_time(0),
    waiting_time(0),
    status(Query::STATUS::WAITING),
    id(0)
{ }

Query::Query(Query *other):
    sx(other->sx), sy(other->sy),
    tx(other->tx), ty(other->ty),
    t0(other->t0),
    start_time(other->start_time),
    finish_time(other->finish_time),
    waiting_time(other->waiting_time),
    status(other->status),
    id(other->id)
{ }

int Query::get_time_of_phone() const {
    return t0;
}

pii Query::get_start_crd() const {
    return make_pair(sx, sy);
}

pii Query::get_finish_crd() const {
    return make_pair(tx, ty);
}

void Query::get_in_car(Taxi *car) {
    if(status == Query::STATUS::KNOW_CAR) {} else throw -1;
//    assert(status == Query::STATUS::KNOW_CAR);
    if(car) {} else throw -1;
//    assert(car); // is it car?
    if(car->get_crd() == get_start_crd()) {} else throw -1;
//    assert(car->get_crd() == get_start_crd()); // check start point
    start_time = car->get_time(); // what time is it?
    waiting_time = start_time - t0;
    if(waiting_time >= 0) {} else throw -1;
//    assert(waiting_time >= 0);
    status = Query::STATUS::IN_CAR;
}

void Query::get_out_of_car(Taxi *car) {
    if(status == Query::STATUS::IN_CAR) {} else throw -1;
//    assert(status == Query::STATUS::IN_CAR);
    if(car) {} else throw -1;
//    assert(car); // is it car?
    if(car->get_crd() == get_finish_crd()) {} else throw -1;
//    assert(car->get_crd() == get_finish_crd()); // check final point
    finish_time = car->get_time(); // what time is it?
    if(start_time <= finish_time) {} else throw -1;
//    assert(start_time <= finish_time); // is it real world?
    car->add_score(Query::calc_score()); // pay for trip
    status = Query::STATUS::FINISH;

    { // for statistics
        statistics::cnt_processed_queries++;

        statistics::max_w0 = max(statistics::max_w0, Query::get_w0());
        statistics::min_w0 = min(statistics::min_w0, Query::get_w0());
        statistics::sum_w0 += Query::get_w0();

        statistics::max_wait = max(statistics::max_wait, Query::waiting_time);
        statistics::min_wait = min(statistics::min_wait, Query::waiting_time);
        statistics::sum_wait += Query::waiting_time;

        statistics::max_pay = max(statistics::max_pay, (int)Query::calc_score());
        statistics::min_pay = min(statistics::min_pay, (int)Query::calc_score());
        statistics::sum_pay += Query::calc_score();
    }
}

void Query::get_msg_from_taxi() {
    if(status == Query::STATUS::WAITING) {} else throw -1;
//    assert(status == Query::STATUS::WAITING);
    status = Query::STATUS::KNOW_CAR;
}

void Query::cancel_taxi() {
    if(Query::get_status() == Query::STATUS::KNOW_CAR) {} else throw -1;
//    assert(Query::get_status() == Query::STATUS::KNOW_CAR);
    status = Query::STATUS::WAITING;
}

void Query::add_to_black_list() {
    status = Query::STATUS::FINISH;
}

Query::STATUS Query::get_status() const {
    return status;
}

int Query::get_id() const {
    return id;
}

int Query::get_w0() const {
    return GEOM::dist(sx, sy, tx, ty);
}

double Query::will_pay(int arrival_time, int time_on_road) const {
    double w0 = Query::get_w0();
    double d1 = arrival_time - Query::get_time_of_phone();
    double d2 = time_on_road - w0;
    double alpha = (1e7 - min(d1 * d1 + d2 * d2, 1e7)) / 1e7;
    return alpha * (100 + w0);
}

int Query::get_start_time() const {
    return Query::start_time;
}

int Query::get_waiting_time() const {
    return Query::waiting_time;
}

double Query::calc_score() const {
    double w0 = Query::get_w0();
    double d1 = waiting_time;
    double d2 = (finish_time - start_time) - w0;
    double alpha = (1e7 - min(d1 * d1 + d2 * d2, 1e7)) / 1e7;
    return alpha * (100 + w0);
}

void Query::read() {
    scanf("%d %d %d %d %d", &t0, &sx, &sy, &tx, &ty);
}

Field::Field():
    W(0), H(0)
{ }

void Field::read() {
    scanf("%d %d", &W, &H);
    int k;
    scanf("%d", &k);
    for(int i = 0;i < k;i++) {
        cars.push_back(new Taxi());
    }
    for(Taxi *car : cars) {
        car->read();
    }
}

// just simulate
void Field::exec_cmds(int check_point_time) {
    for(Taxi *car : cars) {
        car->exec_cmds(check_point_time);
    }
}

// read a new query
// time jump upto t
// return t != -1
bool Field::read_query() {
    int nid = (int)global_queries.size() + 1;
    global_queries.push_back(Query::read_query(nid));
    if(global_queries.back()->get_time_of_phone() == -1) {
        Field::give_cmds();
        Field::exec_cmds(1e7);
        return false;
    }
    Field::exec_cmds(global_queries.back()->get_time_of_phone());
    return global_queries.back()->get_time_of_phone() != -1;
}

vec< pii > find_best_matching(vec< pii > p1, vec< pii > p2) {
    int n = (int)p1.size();
    int m = (int)p2.size();

    reverse(ALL(p1));
    reverse(ALL(p2));

    p1.push_back({0, 0});
    p2.push_back({0, 0});

    reverse(ALL(p1));
    reverse(ALL(p2));

    vec< int > u(n + 1), v(m + 1), p(m + 1), way(m + 1);

    for(int i = 1;i <= n;i++) {
        p[0] = i;
        int j0 = 0;
        vec< int > minv(m + 1, inf);
        vec< char > used(m + 1);
        do {
            used[j0] = true;
            int i0 = p[j0], delta = inf, j1;
            for(int j = 1;j <= m;j++) {
                if (!used[j]) {
                    int cur = /*a[i0][j]*/GEOM::dist(p1[i0], p2[j]) - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0;j <= m;j++) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    vec< pii > matching;
    for(int j = 1;j <= m;j++) {
        if(p[j]) {
            matching.push_back({p[j] - 1, j - 1});
        }else {
//            throw -1;
        }
    }
    return matching;
}

void Field::give_cmds() {

    if(Field::global_queries.empty() || Field::global_queries.back()->get_start_crd().first == -1) {
#ifndef TEST
#ifndef debug
        printf("0\n");
        fflush(stdout);
#endif
#endif
        return;
    }

    //
    static vec< pii > balance_points;

    if(balance_points.empty()) {
        for(Taxi *car : cars) {
            balance_points.push_back(car->get_crd());
        }
    }else {
        balance_points.clear();
        while((int)cars.size() > (int)balance_points.size()) {
            int x0, y0;
            x0 = rand() % W + 1;
            y0 = rand() % H + 1;
            balance_points.push_back({x0, y0});
        }
    }

    static set< Query* > pending;

    pending.clear();
    pending.insert(Field::global_queries.back());

    for(Taxi *car : cars) {
        car->cut_sequence();
    }

    for(Taxi *car : cars) {
         car->erase_zero_pay();
         car->shake_seq();
    }

    vec< pair< int, vec< pair< pii, pair< Query*, int > > > > > out;
    vec< int > perm((int)cars.size());

    for(int i = 0;i < (int)cars.size();i++) {
        perm[i] = i;
    }

    for(Query *q : pending) {
        int best_car = -1;
        double best_val = -inf;

        for(int i : perm) {
            double tmp_val = cars[i]->best_graw_score_after_insert(q, false);
            if(tmp_val > best_val) {
                best_car = i;
                best_val = tmp_val;
            }
        }

        cars[best_car]->best_graw_score_after_insert(q, true);
    }

    for(Taxi *car : cars) {
         car->erase_zero_pay();
    }

    vec< int > best_pt((int)cars.size());
    vec< vec< pair< pii, pair< Query*, int > > > > mem((int)cars.size());
    vec< pii > p1((int)cars.size());

    for(int i = 0;i < (int)cars.size();i++) {
        Taxi *car = cars[i];
        auto ad = car->get_remaining_queries();
        reverse(ALL(ad));
        ad.push_back(make_pair(car->get_crd(), make_pair(nullptr, 0)));
        reverse(ALL(ad));
        p1[i] = ad.back().first;
        mem[i] = ad;
    }

    vec< pii > matching = find_best_matching(p1, balance_points);

    for(pii ed : matching) {
        best_pt[ed.first] = ed.second;
    }

    for(int i = 0;i < (int)cars.size();i++) {
        mem[i].push_back(make_pair(balance_points[best_pt[i]], make_pair(nullptr, 0)));
        cars[i]->listen_cmd(mem[i]);
        out.push_back(make_pair(i + 1, mem[i]));
    }

#ifndef TEST
#ifndef debug
    printf("%d\n", (int)out.size());
    for(auto i : out) {
        Field::print_one_cmd_seq(i.first, i.second);
    }
    fflush(stdout);
#endif
#endif
}

int Field::get_total_score() const {
    double sum = 0;
    for(const Taxi *car : cars) {
        sum += car->get_score();
    }
    sum /= (double)(global_queries.size() - 1);
    return floor(sum + 0.5);
}

int total_cmd_len = 0;

// NO WAITING
// NO FINISH
// if KNOW_CAR then <= 2
// if IN_CAR then <= 1
void Field::print_one_cmd_seq(int car_id, const vec< pair< pii, pair< Query*, int > > >& seq) {
//    printf("%d 1\n", car_id);
//    printf("%d %d 0\n", cars[car_id - 1]->get_crd().first, cars[car_id - 1]->get_crd().second);
//    return;
    total_cmd_len += (int)seq.size();
    if(total_cmd_len <= 1e6) {} else throw -1;
//    assert(total_cmd_len <= 1e6);
    printf("%d %d\n", car_id, (int)seq.size());
    for(pair< pii, pair <Query*, int > > pr : seq) {
        printf("%d %d ", pr.first.first, pr.first.second);
        if(pr.second.second == 1) {
            printf("%d\n", pr.second.first->get_id());
        }else if(pr.second.second == -1) {
            printf("-%d\n", pr.second.first->get_id());
        }else {
            if(pr.second.second == 0) {} else throw -1;
//            assert(pr.second.second == 0);
            if(pr.second.first == nullptr) {} else throw -1;
//            assert(pr.second.first == nullptr);
            printf("%d\n", pr.second.second);
        }
    }
}

int main() {

#ifdef debug
#ifndef TEST
    freopen("input.txt", "r", stdin);
#endif
#endif

    Field field;

    field.read();
    field.give_cmds();

    while(1) {
        if(!field.read_query()) break;
        field.give_cmds();
    }

#ifdef TEST
    printf("total score: %d\n", field.get_total_score());
#endif

#ifdef debug
    printf("total score: %d\n", field.get_total_score());
    statistics::output_statistics();
#endif

    return 0;
}
//
