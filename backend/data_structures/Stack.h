<<<<<<< HEAD
#pragma once
#include <iostream>
using namespace std;

template <typename T>
=======
#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template <class T>
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
class Stack {
private:
    struct Node {
        T data;
        Node* next;
<<<<<<< HEAD
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
=======

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
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
    }

    T pop() {
        if (isEmpty()) {
<<<<<<< HEAD
            throw runtime_error("Stack is empty! Nothing to undo.");
        }
        Node* temp = top;
        T val = temp->data;
        top = top->next;
        delete temp;
        size--;
        return val;
=======
            throw runtime_error("Stack is empty");
        }

        Node* temp = topNode;
        T value = temp->data;
        topNode = topNode->next;
        delete temp;
        return value;
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
    }

    T peek() {
        if (isEmpty()) {
<<<<<<< HEAD
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
=======
            throw runtime_error("Stack is empty");
        }

        return topNode->data;
    }

    bool isEmpty() {
        return topNode == nullptr;
    }
};

#endif
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
