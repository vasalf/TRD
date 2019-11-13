const int mod = inf + 7;

inline int _sum(int a, int b) {
    return a + b < mod ? a + b : a + b - mod;
}

inline int _mul(int a, int b) {
    return (1ll * a * b) % mod;
}

inline int _sub(int a, int b) {
    return a >= b ? a - b : a - b + mod;
}

inline void _inc(int &x, int y) {
    if ((x += y) >= mod) {
        x -= mod;
    }
}

inline void _dec(int &x, int y) {
    if ((x -= y) < 0) {
        x += mod;
    }
}

inline int _binpow(int x, int pw) {
    int res = 1;
    int tmp = x;
    while (pw > 0) {
        if (pw & 1) {
            res = _mul(res, tmp);
        }
        tmp = _mul(tmp, tmp);
        pw >>= 1;
    }
    return res;
}

inline int _div(int a, int b) {
    return _mul(a, _binpow(b, mod - 2));
}

// Ax = b, ret x, A ~ n * n
// guaranteed that there exists unique solution of system
vector<int> solve_linear_equation_system(vector<vector<int>> A, vector<int> b) {
    int n = (int)A.size();
    for (int col = 0; col < n; col++) {
        int hlp_row = -1;
        for (int q = col; q < n; q++) {
            if (A[q][col]) {
                hlp_row = q;
                break;
            }
        }
        for (int q = 0; q < n; q++) {
            swap(A[hlp_row][q], A[col][q]);
        }
        swap(b[hlp_row], b[col]);
        for (int row = col + 1; row < n; row++) {
            int cf = (mod - _div(A[row][col], A[col][col])) % mod;
            for (int q = 0; q < n; q++) {
                _inc(A[row][q], _mul(cf, A[col][q]));
            }
            _inc(b[row], _mul(cf, b[col]));
        }
    }
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            cout << A[i][j] << " ";
//        }
//        cout << "| " << b[i] << "\n";
//    }
//    cout << "\n------------\n";
    for (int j = n - 1; j >= 0; j--) {
        for (int i = j - 1; i >= 0; i--) {
            int cf = (mod - _div(A[i][j], A[j][j]));
            _inc(A[i][j], _mul(cf, A[j][j]));
            _inc(b[i], _mul(cf, b[j]));
        }
    }
    for (int i = 0; i < n; i++) {
        b[i] = _div(b[i], A[i][i]);
    }
    return b;
}
