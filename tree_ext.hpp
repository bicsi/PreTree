#pragma once
#include <algorithm>
#include <functional>

class PreTree {

    struct Node {
        int value;
        Node* sons[2] = {nullptr, nullptr};
        Node* parent = nullptr;

        Node(int value) : value(value) {}
        void set_son(int i, Node* what) {
            sons[i] = what;
            sons[i]->parent = this;
        }
    };
    using PNode = Node*;
    
    struct iterator { 
        PNode ptr;  
        iterator operator++() {
            for (int i = 0; i < 2; ++i) {
                if (ptr->sons[i] != nullptr) {
                    ptr = ptr->sons[i];
                    return *this;
                }
            }
            do {
                ptr = ptr->parent;

            }
            do {
                ptr = ptr->parent;
                if (ptr == nullptr) 
                    return *this;
            } while (ptr->right == nullptr);
            
            ptr = ptr->right;
            return *this;
        }
        iterator operator--() {
            PNode temp = ptr;
            ptr = ptr->parent;

            if (ptr->left == nullptr || ptr->left == temp)
                return *this;
            ptr = ptr->left;
            while (ptr->right != nullptr)
                ptr = ptr->right;
            return ptr;
        } 
        bool operator==(const iterator& oth) const { 
            return ptr == oth.ptr;
        }
    };

    PNode root = nullptr;
    size_t sz = 0;

    public:
    
    size_t size() const { return sz; }
    iterator begin() const { return iterator(root); }
    iterator end() const { return iterator(nullptr); } 
    
    std::pair<iterator, bool> insert(int value) { 
        int old_size = size();
        root = _insert(root, value, 31); 
        return std::make_pair(find(value), old_size != size());
    }
    
    iterator lower_bound(int value) const { return _closest(root, value, 31, false); }
    iterator upper_bound(int value) const { return _closest(root, value, 31, true); }

    bool erase(int value) { 
        int old_size = size();
        root = _erase(root, value, 31);
        return old_size != size(); 
    }
    
    iterator find(int value) const {
        auto ret = lower_bound(value);
        if (ret == end() || ret->value != value)
            return end();
        return ret;
    }

    private:
    
    int get_bit(int x, int bit) { return ((x >> bit) & 1); }

    PNode _closest(PNode root, int value, int level, bool strict) const {
        if (root == nullptr) return nullptr;
        if (strict && value < root->value) return root;
        if (!strict && !(root->value < value)) return root;
        
        int bit = get_bit(value, level);
        return _closest(root->sons[bit], value, level - 1, strict);
    }

    PNode _pop(PNode root) {
        if (root == nullptr) return nullptr;

        for (int i = 0; i < 2; ++i) {
            if (root->sons[i] != nullptr) {
                std::swap(root->sons[i]->value, root->value);
                root->set_son(i, _pop(root->sons[i]));
                return root;
            }    
        }
        sz -= 1;
        delete root;
        return nullptr;
    }

    PNode _insert(PNode root, int value, int level) {
        if (root == nullptr) {
            sz += 1;
            return new Node(value);
        }
        
        if (value == root->value) return root;
        if (value < root->value) 
            std::swap(value, root->value);
        
        int bit = get_bit(value, level);
        root->set_son(bit, _insert(root->sons[bit], value, level - 1)); 
        return root;
    }
    
    PNode _erase(PNode root, int value, int level) {
        if (root == nullptr) return nullptr;
        if (root->value == value) {
            return _pop(root);
        }

        int bit = get_bit(value, level);
        root->set_son(bit, _erase(root->sons[bit], value, level - 1));
        return root;
    }
};
