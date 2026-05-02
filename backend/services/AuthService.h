#pragma once
#include <string>
#include <vector>
#include "../models/User.h"

class AuthService {
private:
    std::vector<User> users;
    User* currentUser;
    std::string hashPassword(const std::string& uname, const std::string& pass) const;
    bool isUsernameTaken(const std::string& uname) const;
    int getNextUserId() const;

public:
    AuthService();
    bool registerUser(int id, const std::string& uname, const std::string& pass, const std::string& role);
    bool registerUser(const std::string& uname, const std::string& pass, const std::string& role = "User");
    bool login(const std::string& uname, const std::string& pass);
    User* getCurrentUser();
    void logout();
};
