#pragma once
#include <string>
#include <vector>
#include "../models/User.h"

class AuthService {
private:
    std::vector<User> users;
    User* currentUser;

public:
    AuthService();
    void registerUser(int id, std::string uname, std::string pass, std::string role);
    bool login(std::string uname, std::string pass);
    User* getCurrentUser();
    void logout();
};
