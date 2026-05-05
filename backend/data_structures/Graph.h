#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
using namespace std;

class Graph {
private:
    static const int MAX = 20;
    string nodes[MAX];
    int adj[MAX][MAX];
    int count;

public:
    Graph() {
        count = 0;
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                adj[i][j] = 0;
            }
        }
    }

    void addNode(string name) {
        if (count < MAX) {
            nodes[count] = name;
            count++;
        }
    }

    int findNode(string name) {
        for (int i = 0; i < count; i++) {
            if (nodes[i] == name) {
                return i;
            }
        }
        return -1;
    }

    void addEdge(string from, string to) {
        int i = findNode(from);
        int j = findNode(to);

        if (i != -1 && j != -1) {
            adj[i][j] = 1;
        }
    }

    bool hasEdge(string from, string to) {
        int i = findNode(from);
        int j = findNode(to);

        if (i != -1 && j != -1) {
            return adj[i][j] == 1;
        }
        return false;
    }

    void display() {
        cout << "\nGraph Connections:\n";
        for (int i = 0; i < count; i++) {
            cout << nodes[i] << " -> ";
            for (int j = 0; j < count; j++) {
                if (adj[i][j] == 1) {
                    cout << nodes[j] << " ";
                }
            }
            cout << endl;
        }
    }
};

#endif
