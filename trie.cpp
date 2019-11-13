const int A = 27; // [a...z] + {'a' - 1} prefix-free
vec< int > empty_vec(A, -1);
struct Trie {
    int n;
    vec< vec< int > > nxt;
    vec< int > cnt;

    Trie():
        n(1)
    {
        nxt.push_back(empty_vec);
        cnt.push_back(0);
    }

    void add_string(const string &s) {
        int v = 0;
        cnt[v]++;
        for(char c : s) {
            c = c - 'a' + 1;
            if(nxt[v][c] == -1) {
                nxt[v][c] = (int)nxt.size();
                nxt.push_back(empty_vec);
                cnt.push_back(0);
            }
            v = nxt[v][c];
            cnt[v]++;
        }
    }

    void show_kth(int k) {
        for(int v = 0;k;) {
            int to = -1;
            for(int c = 0;c < A;c++) {
                if(nxt[v][c] == -1) continue;
                int u = nxt[v][c];
                if(cnt[u] < k) {
                    k -= cnt[u];
                }else {
                    to = c;
                    break;
                }
            }
            if(to == -1) break;
            if(to)
                writeChar(to + 'a' - 1);
            v = nxt[v][to];
        }
    }
};
