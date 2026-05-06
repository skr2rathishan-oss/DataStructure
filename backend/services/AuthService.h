#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include "../models/User.h"
#include "../data_structures/HashTable.h"
using namespace std;

class AuthService {
private:
    HashTable userTable;
    User* currentUser;

public:
    AuthService() {
        currentUser = nullptr;
    }

    void registerUser(int id, string username, string password, string role) {
        User newUser(id, username, password, role);
        userTable.insert(newUser);
    }

    bool login(string username, string password) {
        User* user = userTable.get(username);

        if (user != nullptr && user->password == password && user->isActive) {
            currentUser = user;
            return true;
        }

        return false;
    }

    void logout() {
        currentUser = nullptr;
    }

    bool isLoggedIn() {
        return currentUser != nullptr;
    }

    User* getCurrentUser() {
        return currentUser;
    }
};

#endif
