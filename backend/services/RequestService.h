#ifndef REQUESTSERVICE_H
#define REQUESTSERVICE_H

#include <iostream>
#include "../models/Request.h"
#include "../data_structures/Queue.h"
using namespace std;

class RequestService {
private:
    Queue<Request> requestQueue;

public:
    void addRequest(int requestId, int userId, int fileId, string requestType) {
        Request newRequest(requestId, userId, fileId, requestType, "Pending");
        requestQueue.enqueue(newRequest);
        cout << "Request added successfully\n";
    }

    void processNextRequest() {
        if (requestQueue.isEmpty()) {
            cout << "No requests to process\n";
            return;
        }

        Request currentRequest = requestQueue.dequeue();

        cout << "\nProcessing Request...\n";
        cout << "Request ID: " << currentRequest.requestId << endl;
        cout << "User ID: " << currentRequest.userId << endl;
        cout << "File ID: " << currentRequest.fileId << endl;
        cout << "Request Type: " << currentRequest.requestType << endl;
        cout << "Status: Processed\n";
    }

    bool hasPendingRequests() {
        return !requestQueue.isEmpty();
    }

    void showNextRequest() {
        if (requestQueue.isEmpty()) {
            cout << "No pending requests\n";
            return;
        }

        Request currentRequest = requestQueue.front();

        cout << "\nNext Pending Request:\n";
        cout << "Request ID: " << currentRequest.requestId << endl;
        cout << "User ID: " << currentRequest.userId << endl;
        cout << "File ID: " << currentRequest.fileId << endl;
        cout << "Request Type: " << currentRequest.requestType << endl;
        cout << "Status: " << currentRequest.status << endl;
    }
};

#endif
