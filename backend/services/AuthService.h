#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include "../models/User.h"
#include "../models/Session.h"
#include "../data_structures/HashTable.h"
using namespace std;

class AuthService {
private:
    HashTable userTable;
    User* currentUser;
    Session currentSession;

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
            currentSession = Session(user->userId, user->username, user->role);
            return true;
        }

        return false;
    }

    void logout() {
        currentUser = nullptr;
        currentSession = Session();
    }

    bool isLoggedIn() {
        return currentUser != nullptr;
    }

    User* getCurrentUser() {
        return currentUser;
    }

    bool isSessionActive() {
        return currentSession.active;
    }

    Session getCurrentSession() {
        return currentSession;
    }

    void showSessionInfo() {
        if (currentSession.active) {
            cout << "Session Active\n";
            cout << "User ID: " << currentSession.userId << endl;
            cout << "Username: " << currentSession.username << endl;
            cout << "Role: " << currentSession.role << endl;
        } else {
            cout << "No active session\n";
        }
    }
};

#endif
