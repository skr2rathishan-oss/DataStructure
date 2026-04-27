#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    int userId;
    std::string username;
    std::string password;
    std::string role;

    User() {}

    User(int id, std::string uname, std::string pass, std::string r) {
        userId = id;
        username = uname;
        password = pass;
        role = r;
    }
};

#endif
