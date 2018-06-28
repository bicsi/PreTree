#pragma once
#include "../constants.hpp"
#include <bitset>
#include <functional>
#include <algorithm>

class PreTrie_Iter {
    
    // Node class implementation
    struct Node {
        Node* children[SIGMA];   
        Node* parent;
        int child_id;
        int children_mask;
        std::string key;
        
        // Node constructor
        Node(std::string key) 
            : key(key), parent(nullptr), child_id(-1) {
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

        // Returns key bit (character) at given position
        char GetBit(int pos) const {
            if (pos >= key.size())
                return 0;
            return key[pos];
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

    Node* root = new Node("");

    public:

    // Inserts a new key (string) k into trie
    // Returns true iff successfully inserted
    bool Insert(std::string k) {
        assert(!k.empty());
        Node* at; int dep;
        std::tie(at, dep) = insertion_point(k);
        if (at->key == k) return false;

        // Push the value inside
        while (true) {
            Node* next = at->GetChild(k[dep]);
            if (next == nullptr) {
                // Insert leaf
                next = new Node(k);
                at->SetChild(k[dep], next);
                next->parent = at;
                break;
            }
            std::swap(k, next->key);
            at = next; dep += 1;
        }

        return true;
    }

    // Erases a key (string) k from trie
    // Returns true iff successfully erased
    bool Erase(std::string k) {
        assert(!k.empty());
        Node* at; int dep;
        std::tie(at, dep) = insertion_point(k);
        if (at->key != k) return false;

        // Pop the value
        while (true) {
            int next_id = at->FirstChildId();
            if (next_id == -1) {
                // Remove leaf
                assert(at->Detach());
                delete at;
                break;
            }

            Node* next = at->children[next_id];
            assert(next != nullptr);
            std::swap(at->key, next->key);
            at = next;
        }

        return true;
    }

    bool Find(const std::string& k) {
        assert(!k.empty());
        
        Node* at; int dep;
        std::tie(at, dep) = insertion_point(k);
        return at->key == k;
    }

    void ForEach(std::function<void(std::string)> f) {
        return for_each(root, f, false);
    }

    std::string Predecessor(std::string s) { return DEFAULT_VALUE_STRING; }
    std::string Successor(std::string s) { return DEFAULT_VALUE_STRING; }

    private:

    void for_each(Node* node, std::function<void(std::string)> f,
            bool apply_for_root) {
        if (apply_for_root) f(node->key);
        for (int i = node->FirstChildId(); i != -1; 
                i = node->NextChildId(i))
            for_each(node->children[i], f, true);
    }

    // Gets the node after which we should insert a key
    // Also returns the depth of the node
    std::pair<Node*, int> insertion_point(const std::string& k) {
        Node *first = root, *last = root;
        int a = 0, b = 0;
        // Advance with last node through keypath until the end
        while (b < (int)k.size()) {
             Node* next = last->GetChild(k[b]);
             if (next == nullptr) break;
             last = next; b += 1;
        }

        // Solve the string insertion problem
        for (int i = 0; i < (int)k.size(); ++i) {
            while (a <= b && first->GetBit(i) < k[i]) {
                Node* next = first->GetChild(k[a]);
                first = next; a += 1;
            }
            while (a <= b && last->GetBit(i) > k[i]) {
                Node* next = last->parent;
                last = next; b -= 1;
            }
            if (a > b || last->GetBit(i) < k[i])
                return std::make_pair(last, b);
            if (a > b || first->GetBit(i) > k[i])
                return std::make_pair(first->parent, a - 1);
        }

        assert(a == b);
        assert(first == last);
        return std::make_pair(last, b);
    }
};
