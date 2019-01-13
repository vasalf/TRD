int f(int x) { return (1 << __lg(x)); }

void add(vector< int > &a, int x) {
    for(int v : a) {
        if(f(v) & x) x ^= v;
    }
    if(!x) return;
    for(int &v : a) {
        if(f(x) & v) v ^= x;
    }
    a.push_back(x);
}

int merge(const vector< int > &a, const vector< int > &b) {
    vector< int > c;
    for(int x : a) add(c, x);
    for(int x : b) add(c, x);
    sort(c.rbegin(), c.rend());
    int x = 0;
    for(int y : c) x = max(x, x ^ y);
    return x;
}
