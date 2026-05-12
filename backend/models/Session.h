#ifndef SESSION_H
#define SESSION_H

#include <string>
using namespace std;

class Session {
public:
    int userId;
    string username;
    string role;
    bool active;

    Session() {
        userId = 0;
        username = "";
        role = "";
        active = false;
    }

    Session(int id, string uname, string userRole) {
        userId = id;
        username = uname;
        role = userRole;
        active = true;
    }
};

#endif
