#pragma once
#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>
#include <vector>
#include <deque>
#include "../constants.hpp"

// Implementation of PreTree
// Iterative version with custom memory management
class PreTree_Vec {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value = -1) : value(value) {}
    };    
    using PNode = Node*;

    std::vector<PNode> freed;

    PNode alloc(int value) {
        if (freed.empty()) {
            PNode block = new Node[32];
            freed.reserve(32);
            for (int i = 0; i < 32; ++i)
                freed.push_back(block + i);
        } 
        PNode ret = freed.back();
        freed.pop_back();
        *ret = Node(value);
        return ret;
    }
    void delloc(PNode p) { freed.push_back(p); }

    PNode root = alloc(-1);
    
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
        assert(false);
    }

    void ForEach(std::function<void(int)> f) { for_each(root, f); }

    int Successor(int value) { return DEFAULT_VALUE; }
    int Predecessor(int value) { return DEFAULT_VALUE; }
    
    private:

    void DUMP(PNode root) {
        if (root == nullptr) {
            std::cerr << "NULL ";
            return;
        }
        
        std::cerr << root->value << " ";
        DUMP(root->sons[0]);
        DUMP(root->sons[1]);
    }

    PNode push(PNode node, int value, int level) {
        if (node == nullptr) return alloc(value);

        PNode ret = node;
        while (level--) {
            std::swap(node->value, value);
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr) {
                nxt = alloc(value);
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
            delloc(node); return nullptr;
        }

        PNode ret = node;
        while (true) {
            PNode& nxt = get_son(node);
            node->value = std::move(nxt->value);
            if (get_son(nxt) == nullptr) {
                delloc(nxt); nxt = nullptr;
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
};
