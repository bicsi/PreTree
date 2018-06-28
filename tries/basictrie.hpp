#pragma once
#include "../constants.hpp"
#include <bitset>
#include <functional>
#include <algorithm>

class BasicTrie {
    
    // Node class implementation
    struct Node {
        Node* children[SIGMA];   
        Node* parent;
        int child_id;
        int children_mask;
        bool is_final;
        
        // Node constructor
        Node() : parent(nullptr), child_id(-1), is_final(false) {
            // Initialize sons array
            memset(children, 0, sizeof(children)); 
            children_mask = 0;
        }; 
        // Node destructor
        ~Node() {
            for (int cid = FirstChildId(); cid != -1; cid = FirstChildId()) {
                delete children[cid];
                children[cid] = nullptr;
                children_mask ^= (1 << cid);
            }
        }

        // Getter/setter wrappers for children
        Node* GetChild(char c) const { return children[c - 'a']; }
        void SetChild(char c, Node* value) { 
            children[c - 'a'] = value; 
            children_mask |= (1 << (c - 'a'));
            value->parent = this;
            value->child_id = c - 'a';
        }

        // Removes the node from its parent
        // Returns true if successful
        bool Detach() {
            if (parent == nullptr) 
                return false;
            parent->children[child_id] = nullptr;
            parent->children_mask &= ~(1 << child_id);
            parent = nullptr;
            child_id = -1;
            return true;
        }

        // Returns the (lexicographically) first child
        int FirstChildId() const { 
            if (children_mask == 0) return -1;
            return __builtin_ctz(children_mask);
        }
        int NextChildId(int id) const {
            int mask = children_mask >> (id + 1);
            if (mask == 0) return -1;
            return id + 1 + __builtin_ctz(mask);
        }
    };
    char id_to_char(int i) { return 'a' + i; }

    Node* root = new Node();

    public:

    // Inserts a new key (string) k into trie
    // Returns true iff successfully inserted
    bool Insert(std::string k) {
        assert(!k.empty());
        Node* at = root;
        for (char c : k) {
            Node* next = at->GetChild(c);
            if (next == nullptr) {
                // Insert leaf
                next = new Node();
                at->SetChild(c, next);
                next->parent = at;
            }
            at = next;
        }
        if (at->is_final)
            return false;
        at->is_final = true;
        return true;
    }

    // Erases a key (string) k from trie
    // Returns true iff successfully erased
    bool Erase(std::string k) {
        assert(!k.empty());
        Node* at = root;

        for (char c : k) {
            Node* next = at->GetChild(c);
            if (next == nullptr) {
                return false;
            }
            at = next;
        }

        if (!at->is_final) return false;
        at->is_final = false;
        while (at != root && !at->is_final && at->FirstChildId() == -1) {
            Node* next = at->parent;
            at->Detach();
            at = next;
        }

        return true;
    }

    bool Find(const std::string& k) {
        assert(!k.empty());
        
        Node* at = root;
        for (auto c : k) {
            Node* next = at->GetChild(c);
            if (next == nullptr) {
                return false;
            }
            at = next;
        }
        return at->is_final;
    }

    void ForEach(std::function<void(std::string)> f) {
        std::string key = "";
        return for_each(root, f, key);
    }

    std::string Predecessor(std::string s) { return DEFAULT_VALUE_STRING; }
    std::string Successor(std::string s) { return DEFAULT_VALUE_STRING; }

    private:

    void for_each(Node* node, std::function<void(std::string)> f, std::string& key) {
        if (node->is_final) f(key);
        for (int i = node->FirstChildId(); i != -1; 
                i = node->NextChildId(i)) {
            key += id_to_char(i);
            for_each(node->children[i], f, key);
            key.pop_back();
        }
    }
};
