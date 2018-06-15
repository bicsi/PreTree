#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include "../constants.hpp"


// Implementation of PreTree(*)
// Recursive version
// Does not keep minimum in root, keeps any value instead
class PreTreeR_Rec {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value) : value(value) {}
    };    
    using PNode = Node*;

    PNode root = nullptr;

    public:
    
    void Insert(int value) { root = insert(root, value, BIT_COUNT - 1); }
    
    // FIXME: Is there even a solution?
    int Successor(int value) { return DEFAULT_VALUE; }
    int Predecessor(int value) { return DEFAULT_VALUE; }
    
    void Erase(int value) { root = erase(root, value, BIT_COUNT - 1); }
        
    bool Find(int value) {
        return find(root, value, BIT_COUNT - 1) != nullptr;
    }

    void ForEach(std::function<void(int)> f) { 
        int at = Successor(-1);
        while (at != DEFAULT_VALUE) {
            f(at);
            at = Successor(at);
        }    
    }

    int CALC() { return calc_potential(root, BIT_COUNT - 1); }
    void DUMP() { DUMP(root); std::cerr << std::endl; }

    private:

    int calc_potential(PNode root, int level) {
        if (root == nullptr) return 0;
        int ret = level;
        for (int i = 0; i < 2; ++i) {
            ret += calc_potential(root->sons[i], level - 1);
        }
        return ret;
    }
    
    int get_bit(int x, int bit) { return !!(x & (1 << bit)); }
    
    PNode find(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;
        if (root->value == value) return root;
        int bit = get_bit(value, level);
        return find(root->sons[bit], value, level - 1);
    }
   
    /*
    std::pair<PNode, bool> successor(PNode root, int value, int level, int type) {
        if (root == nullptr) return {nullptr, 0};
        int bit = type ? 0 : get_bit(value, level);
        PNode ret; bool recursed;
        std::tie(ret, recursed) = successor(root->sons[bit], value, level - 1, 0);
        if (root->value > value && (ret == nullptr || ret->value > root->value)) 
            ret = root;

        if (ret == nullptr && type == 0 && bit == 0)
            ret = successor(root->sons[1], value, level - 1, 1);
        return {ret, recursed};
    }
    */

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
        if (root == nullptr) return new Node(value);
        if (root->value == value) return root;
        
        int bit = get_bit(value, level);
        root->sons[bit] = insert(root->sons[bit], value, level - 1); 
        
        return root;
    }
    
    int detached_value;
    PNode detach_leaf(PNode root) {
        for (int bit : {0, 1}) {
            if (root->sons[bit] != nullptr) {
                root->sons[bit] = detach_leaf(root->sons[bit]);
                return root;
            }
        }

        detached_value = root->value;
        delete root;
        return nullptr;
    }
    PNode erase(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;

        if (root->value == value) {
            root = detach_leaf(root);
            if (root != nullptr) {
                std::swap(detached_value, root->value);
            }
            return root;
        }
        
        int bit = get_bit(value, level);
        root->sons[bit] = erase(root->sons[bit], value, level - 1);
        
        return root;
    }
};
