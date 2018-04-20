#pragma once
#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>

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
    
    void insert(int value) { 
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

    bool erase(int value) {
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

    PNode find(int value) {
        PNode node = root;
        for (int level = BIT_COUNT - 1; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr || value < nxt->value) return nullptr;
            if (nxt->value == value) return nxt;
        }
        assert(false);
    }

    void for_each(std::function<void(int)> f) { for_each(root, f); }

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
};
