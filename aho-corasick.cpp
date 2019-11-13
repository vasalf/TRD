const int A = 300; // alphabet size
struct Aho {

    struct Node {
        int nxt[A], go[A];
        int par, pch, link;
        int good;

        Node():
            par(-1), pch(-1), link(-1), good(-1)
        {
            fill(nxt, nxt + A, -1);
            fill(go, go + A, -1);
        }
    };

    vec< Node > a;

    Aho() {
        a.push_back(Node());
    }

    void add_string(const string &s) {
        int v = 0;
        for(char c : s) {
            if(a[v].nxt[c] == -1) {
                a[v].nxt[c] = (int)a.size();
                a.push_back(Node());
                a.back().par = v;
                a.back().pch = c;
            }
            v = a[v].nxt[c];
        }
        a[v].good = 1;
    }

    int go(int v, int c) {
        if(a[v].go[c] == -1) {
            if(a[v].nxt[c] != -1) {
                a[v].go[c] = a[v].nxt[c];
            }else {
                a[v].go[c] = v ? go(get_link(v), c) : 0;
            }
        }
        return a[v].go[c];
    }

    int get_link(int v) {
        if(a[v].link == -1) {
            if(!v || !a[v].par) a[v].link = 0;
            else a[v].link = go(get_link(a[v].par), a[v].pch);
        }
        return a[v].link;
    }

    bool is_good(int v) {
        if(!v) return false;
        if(a[v].good == -1) {
            a[v].good = is_good(get_link(v));
        }
        return a[v].good;
    }

    bool is_there_substring(const string &s) {
        int v = 0;
        for(char c : s) {
            v = go(v, c);
            if(is_good(v)) {
                return true;
            }
        }
        return false;
    }
};
