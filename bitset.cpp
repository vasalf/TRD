const int BITS = 126;

__int128 pw[BITS + 1];

void precalc() {
    pw[0] = 1;
    for (int i = 1; i <= BITS; i++) {
        pw[i] = pw[i - 1] * 2;
    }
}

template<int N>
struct BitSet {
    const static int BLOCKS = N / BITS + 1;

    __int128 a[BLOCKS];

    BitSet() {
        memset(a, 0, sizeof(__int128) * BLOCKS);
    }

    BitSet& operator ^= (const BitSet& o) {
        for (int i = 0; i < BLOCKS; i++) {
            a[i] ^= o.a[i];
        }
        return *this;
    }

    BitSet& operator &= (const BitSet& o) {
        for (int i = 0; i < BLOCKS; i++) {
            a[i] &= o.a[i];
        }
        return *this;
    }

    BitSet& operator |= (const BitSet& o) {
        for (int i = 0; i < BLOCKS; i++) {
            a[i] |= o.a[i];
        }
        return *this;
    }

    BitSet& operator <<= (int k) { // k is less than BITS !!!!!
        if (k <= 0) return *this;
        for (int i = BLOCKS - 1; i >= 0; i--) {
            a[i] <<= k;
            a[i] &= (pw[BITS] - 1);
            if (i > 0) {
                a[i] |= (a[i - 1] >> (BITS - k));
            }
        }
        return *this;
    }

    BitSet& operator >>= (int k) { // k is less than BITS !!!!!
        if (k <= 0) return *this;
        for (int i = 0; i < BLOCKS; i++) {
            a[i] >>= 1;
            if (i + 1 < BLOCKS) {
                a[i] |= ((a[i + 1] & (pw[k] - 1)) << (BITS - k));
            }
        }
        return *this;
    }

    void set(int p) {
        a[p / BITS] |= pw[p % BITS];
    }

    int get(int p) {
        return int((a[p / BITS] >> (p % BITS)) & 1);
    }

    void reset(int p) {
        a[p / BITS] &= ~pw[p & BITS];
    }

    BitSet& operator += (const BitSet& o) {
        for (int carry = 0, i = 0; i < BLOCKS; i++) {
            a[i] += o.a[i] + carry;
            if (a[i] >= pw[BITS]) {
                a[i] -= pw[BITS];
                carry = 1;
            } else {
                carry = 0;
            }
        }
        return *this;
    }

    int pop_count(__int128 value) const {
        int res = 0;
        for (int i = 0; i < BITS; i++) {
            if (pw[i] & value) {
                res++;
            }
        }
        return res;
    }

    int pop_count() const {
        int res = 0;
        for (int i = 0; i < BLOCKS; i++) {
            res += pop_count(a[i]);
        }
        return res;
    }

    BitSet& operator = (const BitSet& o) {
        if (this == &o) return *this;
        memcpy(a, o.a, sizeof(__int128) * BLOCKS);
        return *this;
    }

    void show() const {
        for (int j = N - 1; j >= 0; j--) {
            cout << int((a[j / BITS] >> (j % BITS)) & 1);
        }
        cout << "\n";
    }

    void clear() {
        memset(a, 0, sizeof(__int128) * BLOCKS);
    }
};
