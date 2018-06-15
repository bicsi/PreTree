#pragma once
#include <algorithm>
#include <unordered_set>
#include <functional>
#include "../constants.hpp"

// Wrapper of std::unordered_set
template<typename T>
class StdUnorderedSet {
    std::unordered_set<T> s;

    public:
    
    bool Find(T value) { return s.find(value) != s.end(); }
    void Insert(T value) { s.insert(value); }
    void Erase(T value) { s.erase(value); }
    void ForEach(std::function<void(T)> f) { 
        for (T value : s)
            f(value);
    }
};
