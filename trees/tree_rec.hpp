#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include "../constants.hpp"

// Implementation of PreTree
// Recursive version
class PreTree_Rec {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value) : value(value) {}
    };    
    using PNode = Node*;

    PNode root = nullptr;

    public:
    
    void Insert(int value) { root = insert(root, value, BIT_COUNT - 1); }
    
    void Erase(int value) { root = erase(root, value, BIT_COUNT - 1); }
        
    bool Find(int value) { return find(root, value, BIT_COUNT - 1) != nullptr; }

    void ForEach(std::function<void(int)> f) { for_each(root, f); }

    int Successor(int value) { 
        auto ret = successor(root, value, BIT_COUNT - 1); 
        if (ret == nullptr) return DEFAULT_VALUE;
        return ret->value;    
    }
    int Predecessor(int value) {
        auto ret = predecessor(root, value, BIT_COUNT - 1);
        if (ret == nullptr) return DEFAULT_VALUE;
        return ret->value;
    }

    int CALC() { return CALC(root, BIT_COUNT - 1); }
    void DUMP() { DUMP(root); std::cerr << std::endl; }


    private:

    int get_bit(int x, int bit) { return !!(x & (1 << bit)); }
    
    PNode find(PNode root, int value, int level) {
        // Value is not in the tree
        if (root == nullptr || root->value > value) return nullptr;
        // Value is in the root
        if (root->value == value) return root;
        // Otherwise, recurse down
        int bit = get_bit(value, level);
        return find(root->sons[bit], value, level - 1);
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

    PNode successor(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;
        // If root is bigger than value, no need to recurse down
        if (root->value > value) return root;

        int bit = get_bit(value, level);
        auto ret = successor(root->sons[bit], value, level - 1);
        // Go to the 1 branch only if no answer has been obtained yet
        if (ret == nullptr && bit == 0)
            ret = successor(root->sons[1], value, level - 1);
        return ret;
    }


    PNode greatest(PNode root) {
        if (root == nullptr) return nullptr;
        for (int i = 1; i >= 0; --i)
            if (root->sons[i] != nullptr)
                return greatest(root->sons[i]);
        return root;
    }
    PNode predecessor(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;
        // If root is bigger than value, no need to recurse down
        if (root->value >= value) return nullptr;

        int bit = get_bit(value, level);
        auto ret = predecessor(root->sons[bit], value, level - 1);
        // Go to the 1 branch only if no answer has been obtained yet
        if (ret == nullptr && bit == 1)
            ret = greatest(root->sons[0]);
        // If nothing was obtained on the left branch, return root
        if (ret == nullptr)
            ret = root;
        return ret;
    }

    void for_each(PNode root, std::function<void(int)> f) {
        if (root == nullptr) return;
        // Simple preorder traversal
        f(root->value);
        for (int i = 0; i < 2; ++i)
            for_each(root->sons[i], f);    
    }
    
    int CALC(PNode root, int level) {
        if (root == nullptr) return 0;
        int ret = level;
        for (int i = 0; i < 2; ++i) {
            ret += CALC(root->sons[i], level - 1);
        }
        return ret;
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
};
