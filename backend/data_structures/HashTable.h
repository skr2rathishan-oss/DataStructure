#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "../models/User.h"
using namespace std;

class HashTable {
private:
    // SIZE set to prime number for better hash distribution
    // Increased from 20 to 257 for testing ~100 users
    static const int SIZE = 257;
    User table[SIZE];
    bool occupied[SIZE];

    int hashFunction(string username) {
        int sum = 0;
        // Fixed: use range-based loop to avoid signed/unsigned comparison warning
        for (char c : username) {
            sum += c;
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
        int iterations = 0;

        while (occupied[index] && iterations < SIZE) {
            if (table[index].username == user.username) {
                table[index] = user;
                return;
            }
            index = (index + 1) % SIZE;
            iterations++;
        }

        if (iterations >= SIZE) {
            cerr << "❌ HashTable is full! Cannot insert user: " << user.username << endl;
            return;
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
