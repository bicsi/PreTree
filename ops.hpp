#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "constants.hpp"

template<typename T>
struct Op { 
    int t; T val; 
    Op(int t) : t(t) {}
    Op(int t, T val) : t(t), val(val) {}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Op<T>& op) {
    out << "{";
    switch (op.t) {
        case 0: out << "+ " << op.val; break;
        case 1: out << "- " << op.val; break;
        case 2: out << "? " << op.val; break;
        case 3: out << "> " << op.val; break;
        case 4: out << "< " << op.val; break;
        case -1: out << "DUMP"; break;
    }
    out << "}";
    return out;
}

template<typename T> T RandomValue();
template<> int RandomValue() { return rand() & ((1 << BIT_COUNT) - 1); }
template<> std::string RandomValue() {
    std::string ret = "";
    for (int i = 0; i < BIT_COUNT_STRING; ++i) {
        ret += 'a' + rand() % SIGMA;
    }
    return ret;
}

template<typename T>
std::vector<T> RandomValues(int size) {
    std::vector<T> ret(size);
    for (auto& x : ret)
        x = RandomValue<T>();
    return ret;
}

template<typename T>
std::vector<Op<T>> GenerateOps(int value_count, int op_count, bool insert_dumps = true) {
    auto values = RandomValues<T>(value_count);
    int chk_all_interval = (op_count + 19) / 20;

    std::vector<Op<T>> ops;
    for (int i = 0; i < op_count; ++i) {
        int t = OPS_TO_GENERATE[rand() % OPS_TO_GENERATE.size()];
        T val = values[rand() % values.size()];
        ops.push_back(Op<T>{t, val});
        if (insert_dumps && i % chk_all_interval == 0) {
            ops.push_back(Op<T>{-1});
        }
    }
    return ops;
}

template<typename T>
void InsertionsFromValues(std::vector<Op<T>>& ops, std::vector<T> values) {
    for (auto val : values) {
        ops.push_back(Op<T>{0, val});
    }
}

std::vector<Op<int>> GeneratePermutationInsertions(int value_count) {
    std::vector<int> values(value_count);
    iota(values.begin(), values.end(), 0);
    random_shuffle(values.begin(), values.end());
    std::vector<Op<int>> ops;
    InsertionsFromValues(ops, values);

    return ops;
}

template<typename T>
std::vector<Op<T>> GenerateSortedInsertions(int value_count) {
    auto values = RandomValues<T>(value_count);
    sort(values.begin(), values.end());
    std::vector<Op<T>> ops;
    InsertionsFromValues<T>(ops, values);
    
    return ops;
}

template<typename T>
std::vector<Op<T>> GenerateInvSortedInsertions(int value_count) {
    auto values = RandomValues<T>(value_count);
    sort(values.rbegin(), values.rend());
    std::vector<Op<T>> ops;
    InsertionsFromValues<T>(ops, values);
  
    return ops;
}
