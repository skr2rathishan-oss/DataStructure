#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

struct TreeNode {
    string name;
    bool isFile;
    vector<TreeNode*> children;

    TreeNode(string n, bool file = false) : name(n), isFile(file) {}

    ~TreeNode() {
        for (auto child : children) delete child;
    }
};

class Tree {
private:
    TreeNode* root;

    TreeNode* findChild(TreeNode* node, const string& name) {
        for (auto child : node->children) {
            if (child->name == name) return child;
        }
        return nullptr;
    }

    vector<string> splitPath(const string& path) {
        vector<string> parts;
        stringstream ss(path);
        string token;
        while (getline(ss, token, '/')) {
            if (!token.empty()) parts.push_back(token);
        }
        return parts;
    }

    void printTree(TreeNode* node, int depth) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << (node->isFile ? "[FILE] " : "[DIR]  ") << node->name << "\n";
        for (auto child : node->children)
            printTree(child, depth + 1);
    }

public:
    Tree() { root = new TreeNode("root"); }
    ~Tree() { delete root; }

    bool insertPath(const string& path, bool isFile = true) {
        vector<string> parts = splitPath(path);
        TreeNode* curr = root;
        for (int i = 0; i < (int)parts.size(); i++) {
            bool lastPart = (i == (int)parts.size() - 1);
            TreeNode* child = findChild(curr, parts[i]);
            if (!child) {
                child = new TreeNode(parts[i], lastPart && isFile);
                curr->children.push_back(child);
            }
            curr = child;
        }
        return true;
    }

    bool pathExists(const string& path) {
        vector<string> parts = splitPath(path);
        TreeNode* curr = root;
        for (auto& part : parts) {
            curr = findChild(curr, part);
            if (!curr) return false;
        }
        return true;
    }

    bool deletePath(const string& path) {
        vector<string> parts = splitPath(path);
        if (parts.empty()) return false;
        TreeNode* curr = root;
        TreeNode* parent = nullptr;
        for (auto& part : parts) {
            parent = curr;
            curr = findChild(curr, part);
            if (!curr) return false;
        }
        auto& ch = parent->children;
        for (auto it = ch.begin(); it != ch.end(); ++it) {
            if (*it == curr) {
                ch.erase(it);
                delete curr;
                return true;
            }
        }
        return false;
    }

    void display() {
        cout << "\n--- Directory Structure ---\n";
        printTree(root, 0);
    }
};