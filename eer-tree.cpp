const int N = 2e6 + 5;
 
struct EerTree {
    char s[N];
    int n;
    int sz;
    int link[N];
    int len[N];
    map<char, int> nxt[N];
    int diff[N];
    int dp[N][2];
    int slink[N];
    int max_suff;
    int ans[N]; // number of partitions into palindromes of even length
 
    void clr() {
        fill(s, s + N, 0);
        fill(link, link + N, 0);
        fill(len, len + N, 0);
        fill(nxt, nxt + N, map<char, int>());
        fill(diff, diff + N, 0);
        fill((int*) dp, (int*) dp + N * 2, 0);
        fill(slink, slink + N, 0);
        n = 0;
        sz = 0;
        max_suff = 0;
        fill(ans, ans + N, 0);
    }
 
    EerTree() {
        clr();
        s[0] = '#'; // not in alphabet
        link[0] = 1;
        link[1] = 0;
        len[0] = -1;
        sz = 2;
        ans[0] = 1;
    }
 
    int get_link(int from) {
        while (s[n] != s[n - len[from] - 1]) {
            from = link[from];
        }
        return from;
    }
 
    void add_symbol(char c) {
        s[++n] = c;
        max_suff = get_link(max_suff);
        if (!nxt[max_suff].count(c)) {
            {
                int x = get_link(link[max_suff]);
                link[sz] = nxt[x].count(c) ? nxt[x][c] : 1;
            }
            len[sz] = len[max_suff] + 2;
            diff[sz] = len[sz] - len[link[sz]];
            slink[sz] = diff[sz] == diff[link[sz]] ? slink[link[sz]] : link[sz];
            nxt[max_suff][c] = sz++;
        }
        max_suff = nxt[max_suff][c];
        for (int x = max_suff; len[x] > 0; x = slink[x]) {
            dp[x][0] = dp[x][1] = 0;
            int j = n - (len[slink[x]] + diff[x]);
            _inc(dp[x][j & 1], ans[j]);
            if (diff[x] == diff[link[x]]) {
                _inc(dp[x][0], dp[link[x]][0]);
                _inc(dp[x][1], dp[link[x]][1]);
            }
            _inc(ans[n], dp[x][n & 1]);
        }
    }
};
