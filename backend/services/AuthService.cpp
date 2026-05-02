#include "AuthService.h"
#include <functional>

AuthService::AuthService() {
    currentUser = nullptr;
}

std::string AuthService::hashPassword(const std::string& uname, const std::string& pass) const {
    std::hash<std::string> hasher;
    return std::to_string(hasher(uname + "::" + pass));
}

bool AuthService::isUsernameTaken(const std::string& uname) const {
    for (const auto& user : users) {
        if (user.username == uname) {
            return true;
        }
    }
    return false;
}

int AuthService::getNextUserId() const {
    int maxId = 0;
    for (const auto& user : users) {
        if (user.userId > maxId) {
            maxId = user.userId;
        }
    }
    return maxId + 1;
}

bool AuthService::registerUser(int id, const std::string& uname, const std::string& pass, const std::string& role) {
    if (uname.empty() || pass.empty() || role.empty()) {
        return false;
    }
    if (isUsernameTaken(uname)) {
        return false;
    }
    users.push_back(User(id, uname, hashPassword(uname, pass), role));
    return true;
}

bool AuthService::registerUser(const std::string& uname, const std::string& pass, const std::string& role) {
    if (uname.empty() || pass.empty() || role.empty()) {
        return false;
    }
    if (isUsernameTaken(uname)) {
        return false;
    }
    users.push_back(User(getNextUserId(), uname, hashPassword(uname, pass), role));
    return true;
}

bool AuthService::login(const std::string& uname, const std::string& pass) {
    const std::string hashedPassword = hashPassword(uname, pass);
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == uname && users[i].password == hashedPassword) {
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
