const int LOG = 21;
struct SuffixArray {
    string s;
    int n;
    vec< int > p;
    vec< int > c[LOG];

    SuffixArray():
        n(0)
    { }

    SuffixArray(string ss):
        s(ss)
    {
        s.push_back(0);
        n = (int)s.size();

        vec< int > pn, cn;
        vec< int > cnt;

        p.resize(n);
        for(int i = 0;i < LOG;i++) c[i].resize(n);
        pn.resize(n);
        cn.resize(n);

        cnt.assign(300, 0);

        for(int i = 0;i < n;i++) cnt[s[i]]++;
        for(int i = 1;i < (int)cnt.size();i++) cnt[i] += cnt[i - 1];
        for(int i = n - 1;i >= 0;i--) p[--cnt[s[i]]] = i;

        for(int i = 1;i < n;i++) {
            c[0][p[i]] = c[0][p[i - 1]];
            if(s[p[i]] != s[p[i - 1]]) c[0][p[i]]++;
        }

        for(int lg = 0, k = 1;k < n;k <<= 1, lg++) {
            for(int i = 0;i < n;i++) {
                if((pn[i] = p[i] - k) < 0) pn[i] += n;
            }
            cnt.assign(n, 0);
            for(int i = 0;i < n;i++) cnt[c[lg][pn[i]]]++;
            for(int i = 1;i < (int)cnt.size();i++) cnt[i] += cnt[i - 1];
            for(int i = n - 1;i >= 0;i--) p[--cnt[c[lg][pn[i]]]] = pn[i];
            for(int l1, r1, l2, r2, i = 1;i < n;i++) {
                cn[p[i]] = cn[p[i - 1]];
                l1 = p[i - 1];
                l2 = p[i];
                if((r1 = l1 + k) >= n) r1 -= n;
                if((r2 = l2 + k) >= n) r2 -= n;
                if(c[lg][l1] != c[lg][l2] || c[lg][r1] != c[lg][r2]) cn[p[i]]++;
            }
            c[lg + 1] = cn;
        }

        p.erase(p.begin(), p.begin() + 1);
        n--;

//        cout << "sz = " << (int)p.size() << "\n";

//        for(int i : p) {
//            cout << i << "\n";
//            cout << s.substr(i) << "\n";
//        }

//        cout << "sz = " << (int)c.size() << "\n";

//        for(int i : c) {
//            cout << i << "\n";
//        }
    }

    int get_lcp(int i, int j) {
        int res = 0;
        for(int lg = LOG - 1;lg >= 0;lg--) {

            if(i + (1 << lg) > n || j + (1 << lg) > n) continue;

            if(c[lg][i] == c[lg][j]) {
                i += (1 << lg);
                j += (1 << lg);
                res += (1 << lg);
            }
        }
        return res;
    }
};
