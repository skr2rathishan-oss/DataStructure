#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
public:
    int userId;
    string username;
    string password;
    string role;
    bool isActive;

    // Default constructor
    User() {
        userId = 0;
        username = "";
        password = "";
        role = "";
        isActive = true;
    }

    // Constructor with values
    User(int id, string uname, string pass, string r) {
        userId = id;
        username = uname;
        password = pass;
        role = r;
        isActive = true;
    }
};

#endif
