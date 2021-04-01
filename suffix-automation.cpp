const int ALPHSIZE = 26; // alphabet size
struct SuffixAutomaton {
  struct Node {
    int link, len;
    int next[ALPHSIZE];
    Node() {
      link = -1;
      len = 0;
      for (int i(0); i < ALPHSIZE; i++)
        next[i] = -1;
    }
  };
  string s;
  vector<Node> sa;
  int last;
  SuffixAutomaton() {
    sa.emplace_back();
    last = 0;
    sa[0].len = 0;
    sa[0].link = -1;
    for (int i(0); i < ALPHSIZE; i++)
      sa[0].next[i] = -1;
  }
  void add(const int& c) {
    s.push_back(c + 'a');
    int cur = (int)sa.size();
    sa.emplace_back();
    sa[cur].len = sa[last].len + 1;
    int p;
    for (p = last; p != -1 && sa[p].next[c] == -1;
         p = sa[p].link) {
      sa[p].next[c] = cur;
    }
    if (p == -1) {
      sa[cur].link = 0;
    } else {
      int q = sa[p].next[c];
      if (sa[p].len + 1 == sa[q].len) {
        sa[cur].link = q;
      } else {
        int clone = (int)sa.size();
        sa.emplace_back();
        sa[clone].len = sa[p].len + 1;
        sa[clone].link = sa[q].link;
        for (int i(0); i < ALPHSIZE; i++)
          sa[clone].next[i] = sa[q].next[i];
        sa[cur].link = sa[q].link = clone;
        for (; p != -1 && sa[p].next[c] == q;
             p = sa[p].link) {
          sa[p].next[c] = clone;
        }
      }
    }
    last = cur;
  }
};
