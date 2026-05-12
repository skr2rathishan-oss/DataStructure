#ifndef ACCESSCONTROLSERVICE_H
#define ACCESSCONTROLSERVICE_H

#include <iostream>
#include <string>
#include "../data_structures/Graph.h"
#include "../models/FileRecord.h"
using namespace std;

class AccessControlService {
private:
    Graph permissionGraph;

    string fileNodeName(int fileId) {
        return "file_" + to_string(fileId);
    }

public:
    void addUserNode(const string &username) {
        permissionGraph.addNode(username);
    }

    void addFileNode(const string &fileName) {
        permissionGraph.addNode(fileName);
    }

    void addFileNode(int fileId) {
        permissionGraph.addNode(fileNodeName(fileId));
    }

    void grantPermission(const string &userName, int fileId) {
        permissionGraph.addNode(userName);
        permissionGraph.addNode(fileNodeName(fileId));
        permissionGraph.addEdge(userName, fileNodeName(fileId));
    }

    void revokePermission(const string &userName, int fileId) {
        permissionGraph.removeEdge(userName, fileNodeName(fileId));
    }

    bool checkPermission(const string &userName, int fileId) {
        return permissionGraph.hasEdge(userName, fileNodeName(fileId));
    }

    void makePublic(int fileId) {
        permissionGraph.addNode("PUBLIC");
        permissionGraph.addNode(fileNodeName(fileId));
        permissionGraph.addEdge("PUBLIC", fileNodeName(fileId));
    }

    void makePrivate(int fileId) {
        permissionGraph.removeEdge("PUBLIC", fileNodeName(fileId));
    }

    void makePrivate(int fileId, const string& ownerName) {
        permissionGraph.removeIncomingEdges(fileNodeName(fileId), ownerName);
        if (!ownerName.empty()) {
            grantPermission(ownerName, fileId);
        }
    }

    bool isPublic(int fileId) {
        return permissionGraph.hasEdge("PUBLIC", fileNodeName(fileId));
    }

    bool canAccess(const string& username, const string& role, const FileRecord& file) {
        if (file.isDeleted) {
            return false;
        }

        if (role == "Admin" || file.owner == username) {
            return true;
        }

        if (file.visibility == "PUBLIC" || isPublic(file.fileId)) {
            return true;
        }

        return file.visibility == "RESTRICTED" && checkPermission(username, file.fileId);
    }

    void showPermissions() {
        permissionGraph.display();
    }
};

#endif
