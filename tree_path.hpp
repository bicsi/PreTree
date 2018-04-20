#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

namespace { using namespace std;

class PreTree_Path {
    struct Path {
        vector<int> values;
        vector<Path*> links;
    };
    
    Path* root = nullptr;

    public:
    
    void insert(int value) { 
        apply(root, value, 31, [&](Path* path, int pos, int level) {
            if (pos < (int)path->values.size() && path->values[pos] == value)
                return path;
            return push(path, pos, level, value);
        });
    }
    void erase(int value) {
        apply(root, value, 31, [&](Path* path, int pos, int level) {
            if (pos < (int)path->values.size() && path->values[pos] == value)
                return pop(path, pos);
            return path;
        });
    } 
    int lower_bound(int value) { 
        int ret = INT_MAX;
        apply(root, value, 31, [&](Path* path, int pos, int level) {
            if (pos < (int)path->values.size())
                ret = path->values[pos];
            return path; 
        });
        return ret;
    };
    int upper_bound(int value) { return lower_bound(value + 1); }
    int find(int value) { return lower_bound(value) == value; }

    void for_each(function<void(int)> f) { for_each(root, f); }

    private:
    
    int get_bit(int x, int bit) { return !!(x & (1 << bit)); }
    
    template<typename T>
    void join(vector<T>& a, vector<T> b) {
        a.insert(a.end(), b.begin(), b.end());
    };
    template<typename T>
    vector<T> split(vector<T>& a, int pos) {
        vector<T> ret(a.begin() + pos, a.end());
        a.resize(pos);
        return ret;
    };

    Path* pop(Path* path, int pos) {
        path->values.erase(path->values.begin() + pos);
        path->links.pop_back();
        
        if (path->values.empty()) {
            delete path;
            return nullptr;
        }

        // Need to join paths
        Path* oth = path->links.back();
        if (oth != nullptr) {
            join(path->values, oth->values);
            join(path->links, oth->links);
            delete oth;
        }
        
        return path;
    }

    Path* push(Path* path, int pos, int level, int value) {
        for (int i = pos + 1; i < (int)path->values.size(); ++i) {
            if (get_bit(path->values[i], level - i)) {
                // Need to split path
                Path* oth = new Path();
                oth->values = split(path->values, i);
                oth->links = split(path->links, i);
                path->links.back() = oth;
                break;
            }
        }   
        path->values.insert(path->values.begin() + pos, value);
        path->links.push_back(nullptr);
        return path;
    }
    
    Path* apply(Path* path, int value, 
            int level, function<Path*(Path*, int, int)> f) {
        if (path == nullptr) 
            return f(new Path, 0, value);

        for (int i = 0; i < (int)path->values.size(); ++i) {
            if (path->values[i] >= value) 
                return f(path, i, level);
            
            int mbit = get_bit(value, level - i);
            int obit = get_bit(path->values[i], level - i);

            if (mbit == 1 && obit == 0) {
                path->links[i] = apply(path->links[i], value, level - i - 1, f);
                return path;
            } 
        }
        assert(false);      
    }
    
    void for_each(Path* root, function<void(int)> f) {
        if (root == nullptr) return;
        for (auto x : root->values) f(x);
        for (auto it = root->links.begin(); it != root->links.end(); ++it)
            for_each(*it, f);
    }
};

}
