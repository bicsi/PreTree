#include <bits/stdc++.h>

using namespace std;

// Solves the SIP problem in complexity 
// O(n + m)
int SolveSIP(vector<string> L, string t) {
    int n = L.size(), m = t.size();
    int a = 0, b = n - 1;
    for (int i = 0; i < m; ++i) {
        while (a <= b && L[a][i] < t[i]) ++a;
        while (b >= a && L[b][i] > t[i]) --b;
        if (a > b || L[a][i] > t[i]) return a - 1;
        if (b < a || L[b][i] < t[i]) return b;
    }
    assert(a == b);
    return a - 1; 
}

// Solves the SIP problem in complexity
// O(n * m)
int SolveSIPNaive(vector<string> L, string t) {
    int n = L.size();
    for (int i = 0; i < n; ++i) {
        if (L[i] >= t)
            return i - 1;
    }
    return n - 1;
}

// Generates an instance of the SIP problem
pair<vector<string>, string> GenerateInstance(int sigma, int n, int m) {
    auto generate_string = [&]() {
        string ret = "";

        int len = rand() % (m - 1) + 1;
        for (int i = 0; i < len; ++i) {
            char now = (rand() % sigma) + 'a';
            ret += now;
        }
        for (int i = len; i < m; ++i) 
            ret += '\0';
        return ret;
    };
    
    vector<string> L;
    string t = generate_string();
    
    while (L.size() < n) {
        while (L.size() < n)
            L.push_back(generate_string());
        sort(L.begin(), L.end());
        L.erase(unique(L.begin(), L.end()), L.end());
    }
    return {L, t};
}

int main(int argc, char** argv) {
    srand(time(0));
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);
    int S = stoi(argv[3]);
    int T = stoi(argv[4]);
    
    for (int i = 0; i < T; ++i) {
        vector<string> L; string t;
        tie(L, t) = GenerateInstance(S, N, M);
        int expected = SolveSIPNaive(L, t);
        int got = SolveSIP(L, t);
        if (expected != got) {
            cout << "EXCEPTION!" << endl;
            cout << "L:" << endl;
            for (auto x : L) 
                cout << x << endl;
            cout << "t:" << endl << t << endl;
            cout << "expected: " << expected << " got: " << got << endl;
            exit(-1);
            cout << "OK " << expected << " " << got << endl;
        }
    }
    cout << "OK " << T << " tests." << endl;
}
