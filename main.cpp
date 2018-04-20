#include "tree_iter.hpp"
#include <iostream>

using namespace std;

int main() {
    PreTree_Iter pt;
    
    auto output = [](auto it) {
        if (!it) cout << "NONE" << endl;
        else cout << it->value << endl;
    };
    while (true) {
        string c; int x; cin >> c >> x;
        if (c == "+") pt.insert(x);
        if (c == "-") pt.erase(x);
        // if (c == "L") output(pt.lower_bound(x));
        if (c == "?") output(pt.find(x));
        if (c == "D") pt.DUMP();

        pt.for_each([](int x) { cout << x << " "; });
        cout << endl;
    }
}
