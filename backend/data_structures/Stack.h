#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        while (!isEmpty()) pop();
    }

    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = top;
        top = newNode;
        size++;
    }

    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty! Nothing to undo.");
        }
        Node* temp = top;
        T val = temp->data;
        top = top->next;
        delete temp;
        size--;
        return val;
    }

    T peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty!");
        }
        return top->data;
    }

    bool isEmpty() { return top == nullptr; }
    int getSize() { return size; }

    void display() {
        Node* curr = top;
        cout << "Stack (top -> bottom): ";
        while (curr) {
            cout << "[" << curr->data << "] ";
            curr = curr->next;
        }
        cout << endl;
    }
};