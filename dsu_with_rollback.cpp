struct DSU {
    vector<int> szs;
    vector<pair<int*, int>> buffer;

    vector<int> p, sz;

    DSU(int n) {
        p.resize(n + 1);
        sz.resize(n + 1, 1);
        for (int i = 1; i <= n; i++)
            p[i] = i;
    }

    void checkpoint() {
        szs.push_back((int) buffer.size());
    }

    void rollback() {
        int need_sz = szs.back();
        szs.pop_back();
        while ((int) buffer.size() > need_sz) {
            auto [ptr, val] = buffer.back();
            buffer.pop_back();
            *ptr = val;
        }
    }

    void assign(int* ptr, int val) {
        buffer.emplace_back(ptr, *ptr);
        *ptr = val;
    }

    int find(int x) {
        if (x == p[x])
            return x;
        assign(&p[x], find(p[x]));
        return p[x];
    }

    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y)
            return;
        assign(&p[y], x);
        assign(&sz[x], sz[x] + sz[y]);
    }
};