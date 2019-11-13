/*
n <= 120,
m <= min(n * n, 1000)

100 points

TL 0.2 sec
ML 256 mb

0.031 sec
*/

#include <bits/stdc++.h>
#include "optimization.h"

using namespace std;

#define vec vector
#define ALL(x) (x).begin(), (x).end()

typedef pair< int, int > pii;
typedef long long ll;

const int inf = 1e9;
ll const inf64 = 1ll * inf * inf;

const int N = 125;
const int M = 1005;

int n, m;
int x[M][3];
int e[M][3];
vec< int > v, vn;

int main() {

#ifdef debug
    freopen("input.txt", "r", stdin);
#endif

    srand(42);

    scanf("%d %d", &n, &m);

    vec< vec< int > > clause(n + 1);

    for(int i = 0;i < m;i++) {
        for(int j = 0;j < 3;j++) {
            scanf("%d %d", &x[i][j], &e[i][j]);
            clause[x[i][j]].push_back(i);
        }
    }

    for(int i = 1;i <= n;i++) {
        sort(ALL(clause[i]));
        clause[i].resize(unique(ALL(clause[i])) - clause[i].begin());
    }

    int iter = 3 * n;

    v = vn = vec< int >(n + 1);
    int res = -1;

    vec< int > perm(n);
    for(int i = 0;i < n;i++) {
        perm[i] = i + 1;
    }

    mt19937 rnd(42);

    while(1) {
        for(int i = 1;i <= n;i++) {
            vn[i] = rand() & 1;
        }
        int cur = 0;
        for(int i = 0;i < m;i++) {
            int any = 0;
            for(int j = 0;j < 3;j++) {
                if(vn[x[i][j]] == e[i][j]) {
                    any = 1;
                    break;
                }
            }
            cur += any;
        }
        for(int it = 0;it < iter && cur < m;it++) {
            shuffle(ALL(perm), rnd);
            for(int i : perm) {
                int delt = 0;
                for(int j : clause[i]) {
                    int was = 0;
                    for(int q = 0;q < 3;q++) {
                        was |= (vn[x[j][q]] == e[j][q]);
                    }
                    int now = 0;
                    vn[i] ^= 1;
                    for(int q = 0;q < 3;q++) {
                        now |= (vn[x[j][q]] == e[j][q]);
                    }
                    delt += now - was;
                    vn[i] ^= 1;
                }
                if(delt >= 0) {
                    vn[i] ^= 1;
                    cur += delt;
                }
            }
        }
        if(cur == m) {
            break;
        }
    }

    for(int i = 1;i <= n;i++) {
        writeInt(vn[i]);
    }

    writeChar('\n');

    return 0;
}
////
