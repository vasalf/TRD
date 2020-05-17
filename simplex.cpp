template<class T>
vector<T> operator + (const vector<T> &a, const vector<T> &b) {
    vector<T> res(a.size());
    for (int i = 0; i < (int) a.size(); i++)
        res[i] = a[i] + b[i];
    return res;
}

template<class T>
vector<T> operator * (const T &coef, const vector<T> &a) {
    vector<T> res(a.size());
    for (int i = 0; i < (int) a.size(); i++)
        res[i] = coef * a[i];
    return res;
}

const ld eps = 1e-9;

struct Simplex {
    // Ax = b, x >= 0, <c, x> -> max

    int m; // the number of equations
    int n; // the number of variables
    vector<vector<ld>> A; // (m + 2) x (n + 1)
    // (m + 1)-th row: primary c
    // (m + 2)-th row: seconday c (c')
    // (n + 1)-th col: column of b
    vector<int> basis;
    bool bounded = true;

    Simplex(const vector<vector<ld>> &mat, const vector<int> &_basis): A(mat), basis(_basis) {
        m = (int) mat.size() - 2, n = (int) mat[0].size() - 1;
    }

    /// make primary c under basis components zero
    void reset_c() {
        for (int i = 0; i < m; i++) {
            int j = basis[i];
            A[m] = A[m] + (-A[m][j]) * A[i];
            A[m + 1] = A[m + 1] + (-A[m + 1][j]) * A[i];
        }
    }

    void pivot(int i, int k) {
        A[k] = (ld(1) / ld(A[k][i])) * A[k];
        for (int j = 0; j < (int) A.size(); j++) {
            if (j == k)
                continue;
            A[j] = A[j] + (-A[j][i]) * A[k];
        }
        basis[k] = i;
    }

    void show() {
        cout << "\n=============\n";
        for (auto row : A) {
            for (auto x : row)
                cout << x << " ";
            cout << "\n";
        }
        cout << "basis = { "; for (int j = 0; j < m; j++) cout << basis[j] << " "; cout << "}\n";
        cout << "\n=============\n";
    }

    void run() {
        while (true) {
            int j = 0;
            while (j < n && A[m][j] <= eps)
                j++;
            if (j == n)
                break;

            int k = -1;
            for (int i = 0; i < m; i++)
                if (A[i][j] > eps && (k == -1 || (A[i][n] / A[i][j] < A[k][n] / A[k][j])))
                    k = i;

            if (k == -1) {
                bounded = false;
                break;
            }

            pivot(j, k);
        }
    }

    vector<ld> get_solution() {
        vector<ld> res(n);
        for (int i = 0; i < m; i++)
            res[basis[i]] = A[i][n];
        return res;
    }

    void reset_column(int j) {
        for (int i = 0; i < (int) A.size(); i++)
            A[i][j] = 0;
    }

    ld get_max_value() {
        return -A[m][n];
    }

    void swap_primary_c() {
        swap(A[m], A[m + 1]);
    }

    void flip_task_type() {
        A[m] = ld(-1) * A[m];
        A[m + 1] = ld(-1) * A[m + 1];
    }
};

struct Response {
    bool bounded = true;
    bool exist = true;
    ld value = 0;
    vector<ld> solution = {};
};

// aa * x <= bb, <cc, x> ---> max
Response solve(const vector<vector<ld>> &aa, const vector<ld> &bb, const vector<ld> &cc) {
    int m = (int) aa.size();
    int n = (int) aa[0].size();
    vector<vector<ld>> a(m, vector<ld>(n + m + 1 + 1));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i][j] = aa[i][j];
        a[i][n + i] = +1;
        a[i][n + m] = -1;
        a[i][n + m + 1] = bb[i];
    }
    vector<ld> c(n + m + 1 + 1), c2(n + m + 1 + 1);
    for (int i = 0; i < n; i++)
        c[i] = cc[i];
    c2[n + m] = -1;
    vector<int> basis(m);
    for (int j = 0; j < m; j++)
        basis[j] = n + j;

    a.push_back(c2);
    a.push_back(c);

    Simplex simplex(a, basis);
    simplex.reset_c();

    {
        int k = 0;
        for (int i = 1; i < m; i++)
            if (a[i][n + m + 1] < a[k][n + m + 1])
                k = i;
        if (a[k][n + m + 1] < -eps)
            simplex.pivot(n + m, k);
    }

    simplex.run();

    if (!simplex.bounded || -simplex.get_max_value() > eps) {
        return Response{true, false, 0, {}};
    }

    {
        vector<int> in_basis(n + m + 1, -1);
        for (int i = 0; i < m; i++)
            in_basis[simplex.basis[i]] = i;
        int k = in_basis[n + m];
        if (k != -1) {
            for (int i = 0; i < n + m; i++) {
                if (in_basis[i] != -1)
                    continue;
                if (std::abs(simplex.A[k][i]) <= eps)
                    continue;
                simplex.pivot(i, k);
                break;
            }
        }
        simplex.reset_column(n + m);
    }

    simplex.swap_primary_c();
    simplex.run();

    if (!simplex.bounded) {
        return Response{false, true, 0, {}};
    }

    Response response;

    response.value = simplex.get_max_value();
    response.solution = simplex.get_solution();
    response.solution.resize(n);

    return response;
}
