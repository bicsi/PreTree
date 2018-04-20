#pragma once
#include <algorithm>
#include <functional>
#include <iostream>

class PreTree_Rec {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value) : value(value) {}
    };    
    using PNode = Node*;

    PNode root = nullptr;

    public:
    
    void insert(int value) { root = insert(root, value, BIT_COUNT - 1); }
    
    PNode lower_bound(int value) { return lookup(root, value, BIT_COUNT - 1, 0); }
    PNode upper_bound(int value) { return lookup(root, value, BIT_COUNT - 1, 1); }
    
    void erase(int value) { root = erase(root, value, BIT_COUNT - 1); }
        
    PNode find(int value) {
        auto ret = lower_bound(value);
        if (ret == nullptr || ret->value != value) 
            return nullptr;
        return ret;
    }

    void for_each(std::function<void(int)> f) { for_each(root, f); }

    int CALC() { return calc_potential(root, 31); }
    void DUMP() { DUMP(root); std::cerr << std::endl; }

    int calc_potential(PNode root, int level) {
        if (root == nullptr) return 0;
        int ret = level;
        for (int i = 0; i < 2; ++i) {
            ret += calc_potential(root->sons[i], level - 1);
        }
        return ret;
    }

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
        for (int i = 0; i < 2; ++i) {
            if (root->sons[i] != nullptr) {
                // Found next minimum
                // Swap values and recurse into son
                root->value = std::move(root->sons[i]->value);
                root->sons[i] = pop(root->sons[i]);
                return root;
            }
        }
        
        // All sons are nullptr -- root must be leaf
        // Delete the root and return nullptr
        delete root; return nullptr;
    }

    PNode push(PNode root, int value, int level) {
        // If root is null, then return a new node
        if (root == nullptr) 
            return new Node(value);
        
        // Find the son to "push" the value to
        // and recurse into that son
        int bit = get_bit(root->value, level);
        root->sons[bit] = push(root->sons[bit], std::move(root->value), level - 1);
        root->value = std::move(value);
        return root;
    }
    
    void DUMP(PNode root) {
        if (root == nullptr) {
            std::cerr << "NULL ";
            return;
        }
        
        std::cerr << root->value << " ";
        DUMP(root->sons[0]);
        DUMP(root->sons[1]);
    }
    
    PNode insert(PNode root, int value, int level) {
        // If value is less than minimum, push it into tree
        if (root == nullptr || value < root->value) 
            return push(root, value, level);

        // If value is equal to minimum, we have duplicate
        if (value == root->value) 
            return root;
        
        // Value is greater than minimum -- we should recurse
        // further down
        int bit = get_bit(value, level);
        root->sons[bit] = insert(root->sons[bit], value, level - 1); 
        return root;
    }
    
    PNode erase(PNode root, int value, int level) {
        // If value is less than minimum, then it is not in tree
        if (root == nullptr || value < root->value) 
            return root;

        // If value is equal to minimum, we can erase 
        if (root->value == value)
            return pop(root);
        
        // Value is greater than minimum -- we should recurse
        // further down
        int bit = get_bit(value, level);
        root->sons[bit] = erase(root->sons[bit], value, level - 1);
        return root;
    }

    void for_each(PNode root, std::function<void(int)> f) {
        if (root == nullptr) return;
        f(root->value);
        for (int i = 0; i < 2; ++i)
            for_each(root->sons[i], f);    
    }
};
