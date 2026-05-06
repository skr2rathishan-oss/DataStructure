#ifndef ACCESSCONTROLSERVICE_H
#define ACCESSCONTROLSERVICE_H

#include <iostream>
#include "../data_structures/Graph.h"
using namespace std;

class AccessControlService {
private:
    Graph permissionGraph;

public:
    void addUserNode(const string &username) {
        permissionGraph.addNode(username);
    }

    void addFileNode(const string &fileName) {
        permissionGraph.addNode(fileName);
    }

    void grantPermission(const string &userName, const string &fileName) {
        permissionGraph.addEdge(userName, fileName);
    }

    void revokePermission(const string &userName, const string &fileName) {
        permissionGraph.removeEdge(userName, fileName);
    }

    bool checkPermission(const string &userName, const string &fileName) {
        return permissionGraph.hasEdge(userName, fileName);
    }

    void showPermissions() {
        permissionGraph.display();
    }
};

#endif
