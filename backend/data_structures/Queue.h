#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

template <class T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;

        Node(T value) {
            data = value;
            next = nullptr;
        }
    };

    Node* frontNode;
    Node* rearNode;

public:
    Queue() {
        frontNode = nullptr;
        rearNode = nullptr;
    }

    void enqueue(T value) {
        Node* newNode = new Node(value);

        if (rearNode == nullptr) {
            frontNode = rearNode = newNode;
            return;
        }

        rearNode->next = newNode;
        rearNode = newNode;
    }

    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }

        Node* temp = frontNode;
        T value = temp->data;
        frontNode = frontNode->next;

        if (frontNode == nullptr) {
            rearNode = nullptr;
        }

        delete temp;
        return value;
    }

    T front() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }

        return frontNode->data;
    }

    bool isEmpty() {
        return frontNode == nullptr;
    }
};

#endif
