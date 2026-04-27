#include "AuthService.h"

AuthService::AuthService() {
    currentUser = nullptr;
}

void AuthService::registerUser(int id, std::string uname, std::string pass, std::string role) {
    users.push_back(User(id, uname, pass, role));
}

bool AuthService::login(std::string uname, std::string pass) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == uname && users[i].password == pass) {
            currentUser = &users[i];
            return true;
        }
    }
    return false;
}

User* AuthService::getCurrentUser() {
    return currentUser;
}

void AuthService::logout() {
    currentUser = nullptr;
}
