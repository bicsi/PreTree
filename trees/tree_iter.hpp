#pragma once
#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>
#include "../constants.hpp"

// Implementation of PreTree
// Iterative version
class PreTree_Iter {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value) : value(value) {}
    };    
    using PNode = Node*;

    PNode root = new Node(-1);

    public:

    void DUMP() { DUMP(root); }
    
    void Insert(int value) { 
        PNode node = root;
        for (int level = BIT_COUNT - 1; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr || value < nxt->value)
                nxt = push(nxt, value, level);
            if (nxt->value == value) 
                return;
            node = nxt;
        }
        assert(false);
    }

    bool Erase(int value) {
        PNode node = root;
        for (int level = BIT_COUNT - 1; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr || value < nxt->value) return false;
            if (nxt->value == value) {
                nxt = pop(nxt);
                return true;
            }
            node = nxt;
        }
        return false;
    }

    bool Find(int value) {
        PNode node = root;
        for (int level = BIT_COUNT - 1; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr || value < nxt->value) return false;
            if (nxt->value == value) return true;
        }
        return false;
    }

    void ForEach(std::function<void(int)> f) { for_each(root, f); }

    int Successor(int value) { 
        PNode node = root;

        PNode ret = nullptr;
        for (int level = BIT_COUNT - 1; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            
            if (bit == 0 && node->sons[1] != nullptr)
                ret = node->sons[1];
            
            if (nxt == nullptr) break;
            if (value < nxt->value) 
                return nxt->value;
        }
        return ret == nullptr ? DEFAULT_VALUE : ret->value;
    }
    int Predecessor(int value) { return DEFAULT_VALUE; }
    
    private:


    PNode push(PNode node, int value, int level) {
        if (node == nullptr) return new Node(value);

        PNode ret = node;
        while (level--) {
            std::swap(node->value, value);
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr) {
                nxt = new Node(value);
                break;
            }
            node = nxt;
        }
        return ret;
    }

    PNode pop(PNode node) {
        auto get_son = [](PNode node) -> PNode& {
            return node->sons[0] ?: node->sons[1];
        };

        if (get_son(node) == nullptr) {
            delete node; return nullptr;
        }

        PNode ret = node;
        while (true) {
            PNode& nxt = get_son(node);
            node->value = std::move(nxt->value);
            if (get_son(nxt) == nullptr) {
                delete nxt; nxt = nullptr;
                break;
            }
            node = nxt;
        }
        return ret;
    }
    
    int get_bit(int x, int bit) { return !!(x & (1 << bit)); }

    void for_each(PNode node, std::function<void(int)> f) {
        if (node == nullptr) return;
        if (node != root) f(node->value);
        for (int i = 0; i < 2; ++i)
            for_each(node->sons[i], f);    
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
