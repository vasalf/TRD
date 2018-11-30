vector<int> get_pi(const string& s){
    int n = (int)s.length();
    vector<int> pr(n);
    for(int i = 1; i < n; i++){
        int k = pr[i - 1];
        while(k && s[k] != s[i]) k = pr[k - 1];
        if(s[k] == s[i]) k++;
        pr[i] = k;
    }
    return pr;
}
