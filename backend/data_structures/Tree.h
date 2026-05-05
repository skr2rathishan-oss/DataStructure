#ifndef BST_H
#define BST_H

#include <iostream>
using namespace std;

// BST Node class
class BSTNode {
public:
    int fileId;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;

    BSTNode(int val) {
        fileId = val;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

// BST class
class BST {
private:
    BSTNode* root;
    int size;

    // Inorder traversal for display
    void inorder(BSTNode* node) {
        if (node == nullptr) return;

        inorder(node->left);
        cout << node->fileId << " ";
        inorder(node->right);
    }

public:
    // Constructor
    BST() {
        root = nullptr;
        size = 0;
    }

    // Insert file ID
    void insert(int fileId) {
        BSTNode* newNode = new BSTNode(fileId);

        if (root == nullptr) {
            root = newNode;
            size++;
            return;
        }

        BSTNode* current = root;
        BSTNode* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (fileId == current->fileId) {
                delete newNode; // avoid memory leak
                return; // duplicate not allowed
            }
            else if (fileId < current->fileId) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (fileId < parent->fileId) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        size++;
    }

    // Search file ID
    bool search(int fileId) {
        BSTNode* current = root;

        while (current != nullptr) {
            if (fileId == current->fileId) {
                return true;
            }
            else if (fileId < current->fileId) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return false;
    }

    // Display file IDs in sorted order
    void display() {
        cout << "BST File IDs: ";
        inorder(root);
        cout << endl;
    }

    // Get total number of nodes
    int getSize() {
        return size;
    }
};

#endif
