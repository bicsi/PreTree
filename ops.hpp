#pragma once
#include <iostream>
#include <vector>

struct Op { int t, val; };

std::ostream& operator<<(std::ostream& out, const Op& op) {
    out << "{";
    switch (op.t) {
        case 0: out << "+ " << op.val; break;
        case 1: out << "- " << op.val; break;
        case 2: out << "? " << op.val; break;
        case -1: out << "DUMP"; break;
    }
    out << "}";
    return out;
}

std::vector<Op> GenerateOps(int value_count, int op_count) {
    std::vector<int> values;
    for (int i = 0; i < value_count; ++i) {
        int value = rand() & ((1 << BIT_COUNT) - 1);
        values.push_back(value);
    }
    
    int chk_all_interval = (op_count + 19) / 20;

    std::vector<Op> ops;
    for (int i = 0; i < op_count; ++i) {
        int t = rand() % 2;
        int val = values[rand() % values.size()];
        ops.push_back({t, val});
        if (i % chk_all_interval == 0) {
            ops.push_back({-1, -1});
        }
    }

    return ops;
}