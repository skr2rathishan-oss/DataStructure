#ifndef REQUEST_H
#define REQUEST_H

#include <string>
using namespace std;

class Request {
public:
    int requestId;
    int userId;
    int fileId;
    string requestType;
    string status;

    Request() {
        requestId = 0;
        userId = 0;
        fileId = 0;
        requestType = "";
        status = "Pending";
    }

    Request(int rId, int uId, int fId, string type, string reqStatus = "Pending") {
        requestId = rId;
        userId = uId;
        fileId = fId;
        requestType = type;
        status = reqStatus;
    }
};

#endif
