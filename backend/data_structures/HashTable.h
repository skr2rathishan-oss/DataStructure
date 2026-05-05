#ifndef USERHASHTABLE_H
#define USERHASHTABLE_H

#include <string>
#include "../models/User.h"
using namespace std;

class UserHashTable {
private:
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
    UserHashTable() {
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
