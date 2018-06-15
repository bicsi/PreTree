#include "trees/tree_iter.hpp"
#include <iostream>

using namespace std;

int main() {
    PreTree_Iter pt;
    
    while (true) {
        string c; int x; cin >> c >> x;
        if (c == "+") pt.Insert(x);
        if (c == "-") pt.Erase(x);
        // if (c == "L") output(pt.lower_bound(x));
        if (c == "?") cout << pt.Find(x) << endl;
        if (c == "D") pt.DUMP();

        pt.ForEach([](int x) { cout << x << " "; });
        cout << endl;
    }
}
