#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    int userId;
    std::string username;
    std::string password;
    std::string role;
    std::string salt;

    User() {}

    User(int id, std::string uname, std::string pass, std::string r, std::string s = "") {
        userId = id;
        username = uname;
        password = pass;
        role = r;
        salt = s;
    }
};

#endif
