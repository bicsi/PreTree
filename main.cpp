#include "tree_path.hpp"
#include <iostream>

using namespace std;

int main() {
    PreTree_Path pt;
    
    auto output = [](int it) {
        cout << it << endl;
    };
    while (true) {
        char c; int x; cin >> c >> x;
        if (c == 'I') pt.insert(x);
        if (c == 'E') pt.erase(x);
        if (c == 'L') output(pt.lower_bound(x));
        if (c == 'F') output(pt.find(x));
        
        pt.for_each([](int x) { cout << x << " "; });
        cout << endl;
    }
}
