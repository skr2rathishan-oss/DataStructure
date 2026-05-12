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
    string visibility;

    // Default constructor
    FileRecord() {
        fileId = 0;
        fileName = "";
        filePath = "";
        owner = "";
        fileType = "";
        fileSize = 0;
        isDeleted = false;
        visibility = "PRIVATE";
    }

    // Constructor with values
    FileRecord(int id, string name, string path, string own, string type, int fSize) {
        fileId = id;
        fileName = name;
        filePath = path;
        owner = own;
        fileType = type;
        fileSize = fSize;
        isDeleted = false;
        visibility = "PRIVATE";
    }
};

#endif
