#pragma once
#include <algorithm>
#include <functional>
#include <cassert>

class PreTree_Iter {
    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node(int value) : value(value) {}
    };    
    using PNode = Node*;

    PNode root = new Node(-1);

    public:
    
    void insert(int value) { 
        PNode node = root;
        for (int level = 31; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr) {
                nxt = new Node(value);
                return;
            } else if (nxt->value == value)
                return;
            node = nxt;
        }
        assert(false);
    }

    bool erase(int value) {
        PNode node = root;
        for (int level = 31; level >= 0; --level) {
            int bit = get_bit(value, level);
            PNode& nxt = node->sons[bit];
            if (nxt == nullptr) return false;
            if (nxt->value == value) {
                //nxt = extract(nxt);
                return true;
            }
        }
        return false;
    }

    PNode Find(int value) {
        return nullptr;
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

        delete root;
        return nullptr;
    }

    PNode insert(PNode root, int value, int level) {
        if (root == nullptr) 
            return new Node(value);
        
        if (value == root->value) return root;
        if (value < root->value) 
            std::swap(value, root->value);
        
        int bit = get_bit(value, level);
        root->sons[bit] = insert(root->sons[bit], value, level - 1); 
        return root;
    }
    
    PNode erase(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;
        if (root->value == value) {
            return pop(root);
        }

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
