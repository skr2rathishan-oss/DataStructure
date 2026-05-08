#pragma once
#include <string>
#include <ctime>
#include <iostream>
using namespace std;

struct Session {
    int    userId;
    string username;
    string role;
    string loginTime;
    bool   valid;

    // Empty session (logged out state)
    Session() : userId(-1), valid(false) {}

    // Active session (created on login)
    Session(int id, const string& uname, const string& r)
        : userId(id), username(uname), role(r), valid(true) {
        time_t now = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        loginTime = string(buf);
    }

    bool isValid() const { return valid; }

    void display() const {
        if (!valid) return;
        cout << "  Session | User: " << username
             << " | Role: "      << role
             << " | Since: "     << loginTime << "\n";
    }
};