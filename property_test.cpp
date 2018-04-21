#include <bits/stdc++.h>
#include "ops.hpp"
#include "tree_rec.hpp"
#include "tree_iter.hpp"
#include "tree_vector.hpp"

using namespace std;


template<typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    out << "{";
    for (int i = 0; i < (int)v.size(); ++i) {
        out << v[i];
        if (i + 1 < (int)v.size())
            out << ", ";
    }
    out << "}";
    return out;
}

template<typename T>
void Error(vector<Op> ops, int i, T actual, T expected) {

    cout << "ERROR!" << endl;
    cout << ops << endl;
    cout << "At op: " << i << endl;
    cout << "Expected: " << expected << endl;
    cout << "Actual: " << actual << endl;

    exit(-1);
}

template<typename Set>
void Check(vector<Op> ops) {
    set<int> chk; Set s;

    for (int i = 0; i < (int)ops.size(); ++i) {
        int t = ops[i].t, val = ops[i].val;
        
        if (t == 0) { s.insert(val); chk.insert(val); }
        if (t == 1) { s.erase(val); chk.erase(val); }
        if (t == 2) { 
            int a = !!s.find(val), b = !!chk.count(val);
            if (a != b) Error(ops, i, a, b);
        }
        if (t == -1 or true) { 
            vector<int> a; s.for_each([&](int x) { a.push_back(x); });
            vector<int> b(chk.begin(), chk.end());
            if (a != b) Error(ops, i, a, b);
        }
    }    
}



int main(int argc, char** argv) {
    srand(time(0));
    string tree_type = argv[1];
    int tests = stoi(argv[2]);
    int values_per_test = stoi(argv[3]);
    int ops_per_test = stoi(argv[4]);

    for (int test = 0; test < tests; ++test) {
        vector<Op> ops = GenerateOps(values_per_test, ops_per_test);

        if (tree_type == "iter") Check<PreTree_Iter>(ops);
        else if (tree_type == "rec") Check<PreTree_Rec>(ops);
        else if (tree_type == "vec") Check<PreTree_Vec>(ops);
        else {
            cout << "UNKNOWN TREE TYPE: " << tree_type << endl;
            exit(-1);
        }
    }

    cout << "OK " << tests << " tests." << endl;
}
