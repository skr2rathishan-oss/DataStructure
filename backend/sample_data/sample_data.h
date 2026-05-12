#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include "../services/AuthService.h"
#include "../services/AccessControlService.h"
#include "../services/RequestService.h"

void loadSampleData(AuthService& auth, AccessControlService& accessControl) {
    // Sample users
    auth.registerUser(1, "admin", "admin123", "Admin");
    auth.registerUser(2, "user1", "user123", "User");
    auth.registerUser(3, "viewer1", "viewer123", "Viewer");

    // Sample graph nodes
    accessControl.addUserNode("admin");
    accessControl.addUserNode("user1");
    accessControl.addUserNode("viewer1");

    accessControl.addFileNode("file101");
    accessControl.addFileNode("file102");
    accessControl.addFileNode("file103");
}

void loadSampleRequests(RequestService&) {
    // Intentionally left empty.
    // Request management now uses live access requests created from the menu.
}

#endif
