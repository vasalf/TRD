struct RMQ {
    vector<int> a; // [0, n)
    int n, LOG;
    vector<int> _log;
    vector<vector<pii>> rmq;
 
    RMQ(const vector<int>& aa) {
        a = aa;
        n = (int) a.size();
        LOG = 0;
        while ((1 << LOG) <= n) LOG++;
        _log.resize(n + 1);
        for (int i = 2; i <= n; i++) {
            _log[i] = _log[i - 1] + 1;
            if ((1 << _log[i]) > i) _log[i]--;
        }
        rmq.resize(LOG, vector<pii>(n));
        for (int i = 0; i < n; i++) {
            rmq[0][i] = {a[i], i};
        }
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                rmq[j][i] = min(rmq[j - 1][i], rmq[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
 
    pii get_min(int l, int r) const {
        int lg = _log[r - l + 1];
        return min(rmq[lg][l], rmq[lg][r - (1 << lg) + 1]);
    }
};
