#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include "../models/User.h"
#include "../models/Session.h"
#include "../data_structures/UserHashTable.h"
using namespace std;

class AuthService {
private:
    // Using UserHashTable: better design with tombstone pattern and EMPTY/OCCUPIED/DELETED states
    // Provides O(1) average lookup for login + handles deletions correctly
    UserHashTable userTable;
    User* currentUser;
    Session currentSession;

public:
    AuthService() {
        currentUser = nullptr;
    }

    void registerUser(int id, string username, string password, string role) {
        User newUser(id, username, password, role);
        userTable.insert(username, newUser);
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

    User* getUser(string username) {
        return userTable.get(username);
    }

    bool userExists(string username) {
        return userTable.exists(username);
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
