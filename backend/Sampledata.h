#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include "./services/AuthService.h"

void loadSampleUsers(AuthService& auth) {
    auth.registerUser(1, "admin", "admin123", "Admin");
    auth.registerUser(2, "user1", "user123", "User");
    auth.registerUser(3, "viewer1", "viewer123", "Viewer");
}

#endif
