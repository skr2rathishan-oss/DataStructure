#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template <class T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(T value) {
            data = value;
            next = nullptr;
        }
    };

    Node* topNode;

public:
    Stack() {
        topNode = nullptr;
    }

    void push(T value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
    }

    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }

        Node* temp = topNode;
        T value = temp->data;
        topNode = topNode->next;
        delete temp;
        return value;
    }

    T peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }

        return topNode->data;
    }

    bool isEmpty() {
        return topNode == nullptr;
    }
};

#endif
