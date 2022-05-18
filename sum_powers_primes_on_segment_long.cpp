const int MAXX = 1e6 + 5;

int pr[MAXX];
int pr_sum[MAXX];
vector<int> ps;

void precalc() {
    fill(pr, pr + MAXX, 1);
    pr[0] = pr[1] = 0;
    for (int p = 2; p * p < MAXX; p++) {
        if (!pr[p])
            continue;
        for (int x = p * p; x < MAXX; x += p)
            pr[x] = 0;
    }
    ps.push_back(1);
    for (int x = 1; x < MAXX; x++) {
        pr_sum[x] = sum(pr_sum[x - 1], pr[x] ? x % mod : 0);
        if (pr[x])
            ps.push_back(x);
    }
}

vector<int> find_sum_of_primes(ll N) {
    // sum_{i=1}^{n} [i \in primes] * i
    // n in {N // k, 1 <= k <= N}
    // can be generalize to sum of i^T
    // O(n^{3/4}/log(N))
    vector<ll> vals;
    for (ll x = 1; x < N; x = N / (N / (x + 1)))
        vals.push_back(N / x);
    reverse(vals.begin(), vals.end());
    int sz = (int) vals.size();
    vector<int> S(sz), nx(sz);
    for (int i = 0; i < sz; i++) {
        ll n = vals[i];
        // (2 + n) * (n - 1) / 2 = 2 + 3 + ... + n
        S[i] = n % 2 ?
                mul(((n - 1) / 2) % mod, (n + 2) % mod) :
                mul(((n + 2) / 2) % mod, (n - 1) % mod);
    }
    unordered_map<ll, int> pos_hm;
    for (int i = 0; i < sz; i++)
        pos_hm[vals[i]] = i;
    for (int fr = 0, j = 1;;j++) {
        int any = 0;
        for (int i = fr; i < sz; i++) {
            ll n = vals[i];
            ll need = n / ps[j];
            auto fnd = pos_hm.find(need);
            int pos = fnd == pos_hm.end() ? -1 : fnd->second;
            nx[i] = S[i];
            if (pos < 0 || need < ps[j - 1]) {
                fr++;
                continue;
            }
            any = 1;
            int X = sub(S[pos], pr_sum[ps[j - 1]]);
            dec(nx[i], mul(ps[j], X));
        }
        swap(S, nx);
        if (!any)
            break;
    }
    return S;
}
