#include "tree_iter.hpp"
#include "tree_rec.hpp"
#include "tree_rec2.hpp"
#include "ops.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

template<typename Set>
double Benchmark(vector<Op> ops) {
    clock_t tick = clock();
    Set s;

    for (auto op : ops) {
        if (op.t == 0) s.insert(op.val);
        if (op.t == 1) s.erase(op.val);
        if (op.t == 2) s.find(op.val);
    }

    clock_t tock = clock();
    return 1.0 * (tock - tick) / CLOCKS_PER_SEC;
}

int main(int argc, char** argv) {
    srand(time(0));
    int value_count = stoi(argv[1]);
    int op_count = stoi(argv[2]);

    cout << "Generating ops..." << endl;
    vector<Op> ops = GenerateOps(value_count, op_count);

    cout << "Testing..." << endl << endl;

    cout << "PreTree recursive: " << Benchmark<PreTree_Rec>(ops) << "s" << endl;
    cout << "PreTree iterative: " << Benchmark<PreTree_Iter>(ops) << "s" << endl;
    // cout << "PreTree recursive (no min): " << Benchmark<PreTreeX_Rec>(ops) << "s" << endl;
    cout << "STD Set: " << Benchmark<set<int>>(ops) << "s" << endl;
    cout << "STD Unordered set: " << Benchmark<unordered_set<int>>(ops) << "s" << endl;    
    
    return 0;
}
