#ifndef REQUEST_H
#define REQUEST_H

#include <string>
using namespace std;

class Request {
public:
    int requestId;
    int requesterUserId;
    int userId;
    string requesterUsername;
    string ownerUsername;
    int fileId;
    string requestType;
    string status;

    Request() {
        requestId = 0;
        requesterUserId = 0;
        userId = 0;
        requesterUsername = "";
        ownerUsername = "";
        fileId = 0;
        requestType = "";
        status = "Pending";
    }

    Request(int rId, int requesterId, string requesterName, string ownerName, int fId, string type, string reqStatus = "Pending") {
        requestId = rId;
        requesterUserId = requesterId;
        userId = requesterId;
        requesterUsername = requesterName;
        ownerUsername = ownerName;
        fileId = fId;
        requestType = type;
        status = reqStatus;
    }
};

#endif
