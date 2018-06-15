#include "../trees/tree_iter.hpp"
#include "../trees/tree_rec.hpp"
#include "../trees/tree_rec2.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

template<typename Set>
double BenchmarkInsertions(vector<int> values) {
    clock_t tick = clock();
    Set s;

    for (auto x : values) {
        s.insert(x);
    }
    clock_t tock = clock();
    return 1.0 * (tock - tick) / CLOCKS_PER_SEC;
}

int main(int argc, char** argv) {
    srand(time(0));
    int count = stoi(argv[1]);
    
    cout << "Generating values..." << endl;
    vector<int> values(count);
    for (int i = 0; i < count; ++i)
        values[i] = rand();

    double tm;
    cout << count << " values generated." << endl << endl;
    cout << "Testing..." << endl << endl;

    tm = BenchmarkInsertions<PreTree_Rec>(values);
    cout << "PreTree recursive: " << tm << "s" << endl;
    
    tm = BenchmarkInsertions<PreTree_Iter>(values);
    cout << "PreTree iterative: " << tm << "s" << endl;
    
    tm = BenchmarkInsertions<PreTreeX_Rec>(values);
    cout << "PreTree recursive (no min): " << tm << "s" << endl;

    tm = BenchmarkInsertions<set<int>>(values);
    cout << "STD Set: " << tm << "s" << endl;

    tm = BenchmarkInsertions<unordered_set<int>>(values);
    cout << "STD Unordered set: " << tm << "s" << endl;    
    
    return 0;
}
