#include "tree_iter.hpp"
#include "tree_rec.hpp"
#include "tree_rec2.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

template<typename Set>
double BenchmarkInsertions(vector<pair<int, int>> ops) {
    clock_t tick = clock();
    Set s;

    for (auto op : ops) {
        if (op.first == 0)
            s.insert(op.second);
        else 
            s.erase(op.second);
    }
    clock_t tock = clock();
    return 1.0 * (tock - tick) / CLOCKS_PER_SEC;
}

int main(int argc, char** argv) {
    srand(time(0));
    int value_count = stoi(argv[1]);
    int op_count = stoi(argv[2]);

    cout << "Generating values..." << endl;
    vector<int> values;
    for (int i = 0; i < value_count; ++i)
        values.push_back(rand());
    cout << values.size() << " values generated." << endl << endl;
    
    cout << "Generating ops..." << endl;
    vector<pair<int, int>> ops;
    for (int i = 0; i < op_count; ++i) {
        int t = rand() % 2;
        int val = values[rand() % values.size()];
        ops.emplace_back(t, val);
    }
    cout << ops.size() << " ops generated." << endl << endl;

    double tm;
    
    cout << "Testing..." << endl << endl;

    tm = BenchmarkInsertions<PreTree_Rec>(ops);
    cout << "PreTree recursive: " << tm << "s" << endl;
    
    tm = BenchmarkInsertions<PreTree_Iter>(ops);
    cout << "PreTree iterative: " << tm << "s" << endl;
    
    tm = BenchmarkInsertions<PreTreeX_Rec>(ops);
    cout << "PreTree recursive (no min): " << tm << "s" << endl;

    tm = BenchmarkInsertions<set<int>>(ops);
    cout << "STD Set: " << tm << "s" << endl;

    tm = BenchmarkInsertions<unordered_set<int>>(ops);
    cout << "STD Unordered set: " << tm << "s" << endl;    
    
    return 0;
}
