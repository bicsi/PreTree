#pragma once
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

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

int RandomValue() { return rand() & ((1 << BIT_COUNT) - 1); }

std::vector<int> RandomValues(int size) {
    std::vector<int> ret(size);
    for (auto& x : ret)
        x = RandomValue();
    return ret;
}

std::vector<Op> GenerateOps(int value_count, int op_count) {
    auto values = RandomValues(value_count);
    
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

void InsertionsFromValues(std::vector<Op>& ops, std::vector<int> values) {
    for (auto val : values) {
        ops.push_back(Op{0, val});
    }
}

std::vector<Op> GeneratePermutationInsertions(int value_count) {
    std::vector<int> values(value_count);
    iota(values.begin(), values.end(), 0);
    random_shuffle(values.begin(), values.end());
    std::vector<Op> ops;
    InsertionsFromValues(ops, values);

    return ops;
}

std::vector<Op> GenerateSortedInsertions(int value_count) {
    auto values = RandomValues(value_count);
    sort(values.begin(), values.end());
    std::vector<Op> ops;
    InsertionsFromValues(ops, values);
    
    return ops;
}

std::vector<Op> GenerateInvSortedInsertions(int value_count) {
    auto values = RandomValues(value_count);
    sort(values.rbegin(), values.rend());
    std::vector<Op> ops;
    InsertionsFromValues(ops, values);
  
    return ops;
}