#include <bits/stdc++.h>
#include "../ops.hpp"
#include "../trees/tree_rec.hpp"
#include "../trees/tree_iter.hpp"
#include "../trees/tree_vector.hpp"
#include "../trees/treer_rec.hpp"
#include "../tries/trie_iter.hpp"
#include "../trees/treap.hpp"
#include "../trees/std_set.hpp"
#include "../tries/basictrie.hpp"

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

template<typename TRet, typename T>
void Error(vector<Op<T>> ops, int i, TRet actual, TRet expected) {

    cout << "ERROR!" << endl;
    cout << ops << endl;
    cout << "At op: " << i << endl;
    cout << "Expected: " << expected << endl;
    cout << "Actual: " << actual << endl;

    exit(-1);
}


template<typename CheckSet, typename T>
void Check(vector<Op<T>> ops) {
    StdSet<T> chk; CheckSet s;

    for (int i = 0; i < (int)ops.size(); ++i) {
        int t = ops[i].t;
        T val = ops[i].val;
        
        if (t == 0) { s.Insert(val); chk.Insert(val); }
        if (t == 1) { s.Erase(val); chk.Erase(val); }
        if (t == 2) { 
            bool a = s.Find(val), b = chk.Find(val);
            if (a != b) Error(ops, i, a, b);
        }
        if (t == 3) {
            T a = s.Successor(val), b = chk.Successor(val);
            if (a != b) Error(ops, i, a, b);
        }
        if (t == 4) {
            T a = s.Predecessor(val), b = chk.Predecessor(val);
            if (a != b) Error(ops, i, a, b);
        }
        if (t == -1) { 
            vector<T> a; s.ForEach([&](T x) { a.push_back(x); });
            vector<T> b; chk.ForEach([&](T x) { b.push_back(x); });
            if (a != b) Error(ops, i, a, b);
        }
    }  
}

template<typename Set, typename T>
void RunTest(int value_count, int op_count) {
    Check<Set>(GenerateOps<T>(value_count, op_count));  
}

int main(int argc, char** argv) {
    int REQUIRED_ARGS = 4;
    if (argc != REQUIRED_ARGS + 1) {
        cout << "Wrong number of arguments (got " << argc - 1 << ", expected " << REQUIRED_ARGS << ")" << endl;
        exit(-1);
    }

    srand(time(0));
    string tree_type = argv[1];
    int tests = stoi(argv[2]);
    int values_per_test = stoi(argv[3]);
    int ops_per_test = stoi(argv[4]);

    for (int test = 0; test < tests; ++test) {
        if (tree_type == "iter") RunTest<PreTree_Iter, int>(values_per_test, ops_per_test);
        else if (tree_type == "rec") RunTest<PreTree_Rec, int>(values_per_test, ops_per_test);
        else if (tree_type == "vec") RunTest<PreTree_Vec, int>(values_per_test, ops_per_test);
        else if (tree_type == "rrec") RunTest<PreTreeR_Rec, int>(values_per_test, ops_per_test);
        else if (tree_type == "treap") RunTest<Treap<int>, int>(values_per_test, ops_per_test);
        else if (tree_type == "trie") RunTest<PreTrie_Iter, std::string>(values_per_test, ops_per_test);
        else if (tree_type == "basictrie") RunTest<BasicTrie, std::string>(values_per_test, ops_per_test);
        else {
            cout << "UNKNOWN TREE TYPE: " << tree_type << endl;
            exit(-1);
        }
    }

    cout << "OK " << tests << " tests." << endl;
}
