#pragma once
#include <algorithm>
#include <functional>

class PreTreeX_Rec {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value) : value(value) {}
    };    
    using PNode = Node*;

    PNode root = nullptr;

    public:
    
    void insert(int value) { root = _insert(root, value, 31); }
    
    PNode LowerBound(int value) { return lookup(root, value, 31, 0); }
    PNode UpperBound(int value) { return lookup(root, value, 31, 1); }
    
    void erase(int value) { root = _erase(root, value, 31); }
    
    PNode Find(int value) {
        auto ret = LowerBound(value);
        if (ret == nullptr || ret->value != value) 
            return nullptr;
        return ret;
    }

    void ForEach(std::function<void(int)> f) { for_each(root, f); }

    private:
    
    int get_bit(int x, int bit) { return !!(x & (1 << bit)); }

    PNode lookup(PNode root, int value, int level, int type) {
        if (root == nullptr) return nullptr;
        if (type == 0 && !(root->value < value)) return root;
        if (type == 1 && value < root->value) return root;
        
        int bit = get_bit(value, level);
        return lookup(root->sons[bit], value, level - 1, type);
    }

    PNode pop(PNode root) {
        if (root == nullptr) return nullptr;

        for (int i = 0; i < 2; ++i) {
            if (root->sons[i] != nullptr) {
                std::swap(root->sons[i]->value, root->value);
                root->sons[i] = pop(root->sons[i]);
                return root;
            }    
        }
        delete root;
        return nullptr;
    }

    PNode _insert(PNode root, int value, int level) {
        if (root == nullptr) 
            return new Node(value);
        
        if (value == root->value) return root;
        
        int bit = get_bit(value, level);
        root->sons[bit] = _insert(root->sons[bit], value, level - 1); 
        return root;
    }
    
    PNode _erase(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;
        if (root->value == value) {
            return pop(root);
        }

        int bit = get_bit(value, level);
        root->sons[bit] = _erase(root->sons[bit], value, level - 1);
        return root;
    }

    void for_each(PNode root, std::function<void(int)> f) {
        if (root == nullptr) return;
        f(root->value);
        for (int i = 0; i < 2; ++i)
            for_each(root->sons[i], f);    
    }
};
