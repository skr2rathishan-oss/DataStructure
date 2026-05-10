#ifndef FILERECORD_H
#define FILERECORD_H

#include <string>
using namespace std;

class FileRecord {
public:
    int fileId;
    string fileName;
    string filePath;
    string owner;
    string fileType;
    int fileSize;
    bool isDeleted;

    // Default constructor
    FileRecord() {
        fileId = 0;
        fileName = "";
        filePath = "";
        owner = "";
        fileType = "";
        fileSize = 0;
        isDeleted = false;
    }

    // Constructor with values
    FileRecord(int id, string name, string path, string own, string type, int size) {
        fileId = id;
        fileName = name;
        filePath = path;
        owner = own;
        fileType = type;
        fileSize = size;
        isDeleted = false;
    }
};

#endif