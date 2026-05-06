<<<<<<< HEAD
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../models/FileRecord.h"
=======
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "../models/User.h"
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
using namespace std;

class HashTable {
private:
<<<<<<< HEAD
    static const int TABLE_SIZE = 101;

    struct Entry {
        string key;
        FileRecord value;
        bool occupied;
        bool deleted;
        Entry() : occupied(false), deleted(false) {}
    };

    vector<Entry> table;

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % TABLE_SIZE;
        return hash;
    }

    int probe(const string& key) {
        int idx = hashFunction(key);
        int i = 0;
        while (table[(idx + i) % TABLE_SIZE].occupied &&
               table[(idx + i) % TABLE_SIZE].key != key) {
            i++;
        }
        return (idx + i) % TABLE_SIZE;
    }

public:
    HashTable() : table(TABLE_SIZE) {}

    void insert(const string& key, const FileRecord& record) {
        int idx = probe(key);
        table[idx].key = key;
        table[idx].value = record;
        table[idx].occupied = true;
        table[idx].deleted = false;
    }

    FileRecord* get(const string& key) {
        int idx = hashFunction(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int pos = (idx + i) % TABLE_SIZE;
            if (!table[pos].occupied && !table[pos].deleted) return nullptr;
            if (table[pos].occupied && table[pos].key == key)
                return &table[pos].value;
        }
        return nullptr;
    }

    bool remove(const string& key) {
        int idx = hashFunction(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int pos = (idx + i) % TABLE_SIZE;
            if (!table[pos].occupied && !table[pos].deleted) return false;
            if (table[pos].occupied && table[pos].key == key) {
                table[pos].occupied = false;
                table[pos].deleted = true;
                return true;
            }
        }
        return false;
    }

    bool exists(const string& key) {
        return get(key) != nullptr;
    }

    void display() {
        cout << "\n--- Hash Table (File Index) ---\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].occupied) {
                cout << "  [" << i << "] Key: " << table[i].key
                     << " | File: " << table[i].value.fileName
                     << " | Path: " << table[i].value.filePath << "\n";
            }
        }
    }
};
=======
    static const int SIZE = 20;
    User table[SIZE];
    bool occupied[SIZE];

    int hashFunction(string username) {
        int sum = 0;
        for (int i = 0; i < username.length(); i++) {
            sum += username[i];
        }
        return sum % SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < SIZE; i++) {
            occupied[i] = false;
        }
    }

    void insert(User user) {
        int index = hashFunction(user.username);

        while (occupied[index]) {
            if (table[index].username == user.username) {
                table[index] = user;
                return;
            }
            index = (index + 1) % SIZE;
        }

        table[index] = user;
        occupied[index] = true;
    }

    User* get(string username) {
        int index = hashFunction(username);
        int start = index;

        while (occupied[index]) {
            if (table[index].username == username) {
                return &table[index];
            }

            index = (index + 1) % SIZE;

            if (index == start) {
                break;
            }
        }

        return nullptr;
    }

    bool exists(string username) {
        return get(username) != nullptr;
    }
};

#endif
>>>>>>> ba0e2d1c9776df62045e4e163d7d006446776019
