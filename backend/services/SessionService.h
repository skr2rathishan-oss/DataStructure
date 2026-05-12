#ifndef SESSIONSERVICE_H
#define SESSIONSERVICE_H

#include <iostream>
#include "../models/Session.h"
#include "../models/User.h"
using namespace std;

class SessionService {
private:
    Session currentSession;

public:
    void startSession(User user) {
        currentSession = Session(user.userId, user.username, user.role);
    }

    void endSession() {
        currentSession = Session();
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
