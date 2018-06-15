#include "../trees/tree_iter.hpp"
#include "../trees/tree_rec.hpp"
#include "../trees/tree_vector.hpp"
#include "../trees/treer_rec.hpp"
#include "../trees/std_set.hpp"
#include "../trees/std_unordered_set.hpp"
#include "../tries/trie_iter.hpp"
#include "../ops.hpp"
#include <iostream>
#include <vector>

using namespace std;

int UNUSED = 0;

template<typename Set, typename T>
double BenchmarkSet(vector<Op<T>> ops) {
    clock_t tick = clock();
    
    Set s;
    int ans = 0;
    for (auto op : ops) {
        if (op.t == 0) s.Insert(op.val);
        if (op.t == 1) s.Erase(op.val);
        if (op.t == 2) ans += s.Find(op.val);
    }
    UNUSED += ans;

    clock_t tock = clock();
    return 1.0 * (tock - tick) / CLOCKS_PER_SEC;
}

void BenchmarkTrees(vector<Op<int>> ops) {
    cout << "--------------------------" << endl;
    cout << "PreTree recursive: " << BenchmarkSet<PreTree_Rec, int>(ops) << "s" << endl;
    cout << "PreTree iterative: " << BenchmarkSet<PreTree_Iter, int>(ops) << "s" << endl;
    // cout << "PreTree vector: " << BenchmarkSet<PreTree_Vec, int>(ops) << "s" << endl;
    cout << "PreTree recursive (random): " << BenchmarkSet<PreTreeR_Rec, int>(ops) << "s" << endl;
    cout << "STD Set: " << BenchmarkSet<StdSet<int>, int>(ops) << "s" << endl;
    cout << "STD Unordered set: " << BenchmarkSet<StdUnorderedSet<int>, int>(ops) << "s" << endl;
    cout << "--------------------------" << endl << endl;
}

void BenchmarkTries(vector<Op<string>> ops) {
    cout << "--------------------------" << endl;
    cout << "PreTrie iterative: " << BenchmarkSet<PreTrie_Iter, string>(ops) << "s" << endl;
    cout << "STD Set: " << BenchmarkSet<StdSet<string>, string>(ops) << "s" << endl;
    cout << "STD Unordered set: " << BenchmarkSet<StdUnorderedSet<string>, string>(ops) << "s" << endl;
    cout << "--------------------------" << endl << endl;
}

int main(int argc, char** argv) {
    int REQUIRED_ARGS = 2;
    if (argc != REQUIRED_ARGS + 1) {
        cout << "Wrong number of arguments (got " << argc - 1 << ", expected " << REQUIRED_ARGS << ")" << endl;
        exit(-1);
    }

    srand(time(0));
    int value_count = stoi(argv[1]);
    int op_count = stoi(argv[2]);

    cout << "_________________________________________" << endl;
    cout << "TESTING PRETREE" << endl;
    cout << "_________________________________________" << endl;
    cout << endl;

    cout << "Testing on " << op_count << " random operations..." << endl << endl;
    BenchmarkTrees(GenerateOps<int>(value_count, op_count, false));

    cout << "Testing on " << value_count << " sorted inserions..." << endl << endl;
    BenchmarkTrees(GenerateSortedInsertions<int>(value_count));

    cout << "Testing on " << value_count << " inverse sorted inserions..." << endl << endl;
    BenchmarkTrees(GenerateInvSortedInsertions<int>(value_count));

    cout << "Testing on 1:" << value_count << "..." << endl << endl;
    BenchmarkTrees(GeneratePermutationInsertions(value_count));

    cout << "_________________________________________" << endl;
    cout << "TESTING PRETRIE" << endl;
    cout << "_________________________________________" << endl;
    cout << endl;


    cout << "Testing on " << op_count << " random operations..." << endl << endl;
    BenchmarkTries(GenerateOps<string>(value_count, op_count, false));

    cout << "Testing on " << value_count << " sorted inserions..." << endl << endl;
    BenchmarkTries(GenerateSortedInsertions<string>(value_count));

    cout << "Testing on " << value_count << " inverse sorted inserions..." << endl << endl;
    BenchmarkTries(GenerateInvSortedInsertions<string>(value_count));

    
    srand(UNUSED);
    return 0;
}
