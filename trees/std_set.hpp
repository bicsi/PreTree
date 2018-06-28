#pragma once
#include <algorithm>
#include <set>
#include <functional>
#include "../constants.hpp"

// Wrapper of std::set
template<typename T>
class StdSet {
    std::set<T> s;
    
    public:
    
    bool Find(T value) { return s.find(value) != s.end(); }
    void Insert(T value) { s.insert(value); }
    void Erase(T value) { s.erase(value); }
    void ForEach(std::function<void(T)> f) { 
        for (T value : s)
            f(value);
    }
    T Successor(T value) {
        auto it = s.upper_bound(value);
        if (it == s.end())
            return Default<T>();
        return *it; 
    }
    T Predecessor(T value) {
        auto it = s.lower_bound(value);
        if (it == s.begin())
            return Default<T>();
        return *prev(it);
    }
};
