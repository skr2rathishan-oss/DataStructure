<<<<<<< HEAD
#pragma once
#include <iostream>
#include <string>
#include "../models/FileRecord.h"
using namespace std;

class LinkedList {
private:
    struct Node {
        FileRecord data;
        Node* next;
        Node(FileRecord rec) : data(rec), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    void addVersion(FileRecord record) {
        Node* newNode = new Node(record);
        newNode->next = head;
        head = newNode;
        size++;
    }

    FileRecord getLatest() {
        if (!head) throw runtime_error("No versions found!");
        return head->data;
    }

    FileRecord getPreviousVersion() {
        if (!head || !head->next) throw runtime_error("No previous version!");
        return head->next->data;
    }

    void removeLatest() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
    }

    void displayHistory(string fileId) {
        Node* curr = head;
        cout << "\n--- Version History for File: " << fileId << " ---\n";
        int v = 1;
        while (curr) {
            cout << "  Version " << v++ << ": " << curr->data.fileName
                 << " | Size: " << curr->data.fileSize << " bytes"
                 << " | Modified: " << curr->data.modifiedAt << "\n";
            curr = curr->next;
        }
    }

    int getSize() { return size; }
    bool isEmpty() { return head == nullptr; }
};
=======
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
using namespace std;

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(T value) {
            data = value;
            next = nullptr;
        }
    };

    Node* head;

public:
    LinkedList() {
        head = nullptr;
    }

    void insertAtEnd(T value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    Node* getHead() {
        return head;
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

#endif
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
