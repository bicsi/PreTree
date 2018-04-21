#include "tree_iter.hpp"
#include "tree_rec.hpp"
#include "tree_vector.hpp"
#include "tree_rec2.hpp"
#include "ops.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

template<typename Set>
double BenchmarkSet(vector<Op> ops) {
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

void BenchmarkAll(vector<Op> ops) {
    cout << "--------------------------" << endl;
    cout << "PreTree recursive: " << BenchmarkSet<PreTree_Rec>(ops) << "s" << endl;
    cout << "PreTree iterative: " << BenchmarkSet<PreTree_Iter>(ops) << "s" << endl;
    cout << "PreTree vector: " << BenchmarkSet<PreTree_Vec>(ops) << "s" << endl;
    // cout << "PreTree recursive (no min): " << Benchmark<PreTreeX_Rec>(ops) << "s" << endl;
    cout << "STD Set: " << BenchmarkSet<set<int>>(ops) << "s" << endl;
    cout << "STD Unordered set: " << BenchmarkSet<unordered_set<int>>(ops) << "s" << endl;
    cout << "--------------------------" << endl << endl;
}

int main(int argc, char** argv) {
    srand(time(0));
    int value_count = stoi(argv[1]);
    int op_count = stoi(argv[2]);

    cout << "Generating ops..." << endl;

    cout << "Testing on " << op_count << " random operations..." << endl << endl;
    BenchmarkAll(GenerateOps(value_count, op_count));

    cout << "Testing on " << value_count << " sorted inserions..." << endl << endl;
    BenchmarkAll(GenerateSortedInsertions(value_count));

    cout << "Testing on " << value_count << " inverse sorted inserions..." << endl << endl;
    BenchmarkAll(GenerateInvSortedInsertions(value_count));

    cout << "Testing on 1:" << value_count << "..." << endl << endl;
    BenchmarkAll(GeneratePermutationInsertions(value_count));
    
    return 0;
}
