#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../models/User.h"
using namespace std;


class UserHashTable {
private:
    static const int TABLE_SIZE = 53; // prime number = fewer collisions

    enum SlotState { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        string    key;
        User      value;
        SlotState state;
        Slot() : state(EMPTY) {}
    };

    vector<Slot> table;

    // Polynomial rolling hash
    int hashFunction(const string& key) const {
        long long hash = 0;
        for (char c : key)
            hash = (hash * 31 + c) % TABLE_SIZE;
        return (int)hash;
    }

public:
    UserHashTable() : table(TABLE_SIZE) {}

    // Insert or update user
    bool insert(const string& key, const User& user) {
        int idx = hashFunction(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int pos = (idx + i) % TABLE_SIZE;
            if (table[pos].state != OCCUPIED || table[pos].key == key) {
                table[pos].key   = key;
                table[pos].value = user;
                table[pos].state = OCCUPIED;
                return true;
            }
        }
        cout << "UserHashTable is full!\n";
        return false;
    }

    // Get pointer to user (nullptr if not found)
    User* get(const string& key) {
        int idx = hashFunction(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int pos = (idx + i) % TABLE_SIZE;
            if (table[pos].state == EMPTY)   return nullptr;
            if (table[pos].state == OCCUPIED && table[pos].key == key)
                return &table[pos].value;
        }
        return nullptr;
    }

    // Soft delete (keeps probing chains intact)
    bool remove(const string& key) {
        int idx = hashFunction(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int pos = (idx + i) % TABLE_SIZE;
            if (table[pos].state == EMPTY)   return false;
            if (table[pos].state == OCCUPIED && table[pos].key == key) {
                table[pos].state = DELETED;
                return true;
            }
        }
        return false;
    }

    bool exists(const string& key) {
        return get(key) != nullptr;
    }

    // Admin: print all stored users
    void display() const {
        cout << "\n--- User Hash Table ---\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].state == OCCUPIED) {
                cout << "  [Slot " << i << "] " << table[i].key << " -> ";
                table[i].value.display();
            }
        }
    }
};