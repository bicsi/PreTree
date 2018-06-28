#pragma once
#include <algorithm>
#include <set>
#include <functional>
#include "../constants.hpp"

// Treap implementation (with split-join)
// should be fastest of them all
template<typename T>
class Treap {

    struct Node {
        int prior;
        T key;
        Node *left, *right;

        Node(T key, int prior) : key(key), prior(prior), left(nullptr), right(nullptr) {}
    };
    Node* root = nullptr;
    
    public:
    
    bool Find(T value) { return find(root, value); }
    void Insert(T value) { 
        if (Find(value)) return;
        root = insert(root, value, rand()); 
    }
    void Erase(T value) { 
        if (!Find(value)) return;
        root = erase(root, value);
    }
    void ForEach(std::function<void(T)> f) { for_each(root, f); }
    T Successor(T value) { return Default<T>(); } 
    T Predecessor(T value) { return Default<T>(); }

    int for_each(Node* root, std::function<void(T)> f) {
        if (root == nullptr) return 0;
        int l = for_each(root->left, f);
        f(root->key);
        int r = for_each(root->right, f);
        assert(l <= root->prior);
        assert(r <= root->prior);
        return root->prior;
    }

    bool find(Node* root, T key) {
        if (root == nullptr) return false;
        if (root->key == key) return true;
        if (root->key < key) return find(root->right, key);
        else return find(root->left, key); 
    }

    std::pair<Node*, Node*> split(Node* root, T key) {
        if (root == nullptr) return std::make_pair(nullptr, nullptr);
        Node* ret;
        if (root->key < key) {
            std::tie(root->right, ret) = split(root->right, key);
            return std::make_pair(root, ret);
        } else {
            std::tie(ret, root->left) = split(root->left, key);
            return std::make_pair(ret, root);
        }
    }

    Node* join(Node* left, Node* right) {
        if (left == nullptr) return right;
        if (right == nullptr) return left;
        if (left->prior < right->prior) {
            right->left = join(left, right->left);
            return right;
        } else {
            left->right = join(left->right, right);
            return left;
        }
    }

    Node* insert(Node* root, T key, int prior) {
        if (root == nullptr || root->prior < prior) {
            Node* ret = new Node(key, prior);
            std::tie(ret->left, ret->right) = split(root, key);
            return ret;            
        }
        if (root->key < key) root->right = insert(root->right, key, prior);
        else root->left = insert(root->left, key, prior);
        return root;
    }

    Node* erase(Node* root, T key) {
        if (root == nullptr) return nullptr;
        if (root->key == key) {
            Node* ret = join(root->left, root->right);
            delete root;
            return ret;
        }
        if (root->key < key) root->right = erase(root->right, key);
        else root->left = erase(root->left, key);
        return root;
    }

};
