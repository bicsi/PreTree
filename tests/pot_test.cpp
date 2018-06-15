#include <bits/stdc++.h>
#include "../trees/tree_rec.hpp"

using namespace std;

int main() {
    srand(time(0));
    for (auto n : {10, 100, 1000, 10000, 100000, 1000000}) {
        int best = 0;
        for (int it = 0; it < 50; ++it) {
            vector<int> vals(n);
            for (auto& x : vals) 
                x = rand();
            int split = rand();
            PreTree_Rec l, r, a;
            for (auto x : vals) {
                if (x < split) l.insert(x);
                else r.insert(x);
                a.insert(x);
            }
            int now = l.CALC() + r.CALC() - a.CALC();
            if (best < now) best = now;
        }

        cout << n << ": " << best << endl;
    }
    
    return 0;
}
