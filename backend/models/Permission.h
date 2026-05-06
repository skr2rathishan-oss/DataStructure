#ifndef PERMISSION_H
#define PERMISSION_H

#include <string>
using namespace std;

class Permission {
public:
    int userId;
    int fileId;
    string accessType;

    Permission() {
        userId = 0;
        fileId = 0;
        accessType = "";
    }

    Permission(int uId, int fId, string type) {
        userId = uId;
        fileId = fId;
        accessType = type;
    }
};

#endif
