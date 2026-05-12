#ifndef REQUESTSERVICE_H
#define REQUESTSERVICE_H

#include <iostream>
#include "../models/Request.h"
#include "../data_structures/Queue.h"
#include "FileService.h"
#include "AccessControlService.h"
using namespace std;

class RequestService {
private:
    Queue<Request> requestQueue;

public:
    void addAccessRequest(int requestId,
                          int requesterUserId,
                          const string& requesterUsername,
                          const string& ownerUsername,
                          int fileId) {
        Request newRequest(requestId, requesterUserId, requesterUsername, ownerUsername, fileId, "Access", "Pending");
        requestQueue.enqueue(newRequest);
        cout << "\n✅ Access request added successfully" << endl;
        cout << "   Request ID: " << requestId << endl;
        cout << "   Requester : " << requesterUsername << endl;
        cout << "   Owner     : " << ownerUsername << endl;
        cout << "   File ID   : " << fileId << endl;
        cout << "   Status    : Pending" << endl;
    }

    void showNextRequest() {
        if (requestQueue.isEmpty()) {
            cout << "No pending access requests\n";
            return;
        }

        Request currentRequest = requestQueue.front();

        cout << "\nNext Pending Access Request:\n";
        cout << "Request ID       : " << currentRequest.requestId << endl;
        cout << "Requester        : " << currentRequest.requesterUsername << endl;
        cout << "Owner            : " << currentRequest.ownerUsername << endl;
        cout << "Requester User ID : " << currentRequest.requesterUserId << endl;
        cout << "File ID          : " << currentRequest.fileId << endl;
        cout << "Request Type     : " << currentRequest.requestType << endl;
        cout << "Status           : " << currentRequest.status << endl;
    }

    void processNextRequest(const string& processorUsername,
                            const string& processorRole,
                            FileService& fileService,
                            AccessControlService& accessControl) {
        if (requestQueue.isEmpty()) {
            cout << "No requests to process\n";
            return;
        }

        Request currentRequest = requestQueue.front();
        FileRecord* file = fileService.getFileById(currentRequest.fileId);

        if (file == nullptr) {
            requestQueue.dequeue();
            cout << "\n❌ The requested file no longer exists." << endl;
            return;
        }

        bool canProcess = (processorRole == "Admin" || processorUsername == currentRequest.ownerUsername);
        if (!canProcess) {
            cout << "Only Admin or file owner can process this request." << endl;
            return;
        }

        cout << "\n--- Access Request ---" << endl;
        cout << "Request ID   : " << currentRequest.requestId << endl;
        cout << "Requester    : " << currentRequest.requesterUsername << endl;
        cout << "Owner        : " << currentRequest.ownerUsername << endl;
        cout << "File ID      : " << currentRequest.fileId << endl;
        cout << "Request Type : " << currentRequest.requestType << endl;
        cout << "Status       : " << currentRequest.status << endl;

        char approveChoice;
        cout << "Approve request? (y/n): ";
        cin >> approveChoice;

        requestQueue.dequeue();

        if (approveChoice == 'y' || approveChoice == 'Y') {
            // Ensure both nodes exist in the permission graph
            accessControl.addUserNode(currentRequest.requesterUsername);
            accessControl.addFileNode(currentRequest.fileId);
            accessControl.grantPermission(currentRequest.requesterUsername, currentRequest.fileId);
            cout << "\n✅ Request approved." << endl;
            cout << "   Access granted to: " << currentRequest.requesterUsername << endl;
            cout << "   File ID: " << currentRequest.fileId << endl;
        } else {
            cout << "\n❌ Request rejected." << endl;
        }
    }

    bool hasPendingRequests() {
        return !requestQueue.isEmpty();
    }
};

#endif
