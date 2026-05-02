#pragma once
#include <string>
using namespace std;

struct FileRecord {
    string fileId;
    string fileName;
    string filePath;
    string owner;
    string fileType;
    int fileSize;
    string createdAt;
    string modifiedAt;
    bool isDeleted;

    FileRecord() : fileSize(0), isDeleted(false) {}

    FileRecord(string id, string name, string path, string own, string type, int size)
        : fileId(id), fileName(name), filePath(path), owner(own),
          fileType(type), fileSize(size), isDeleted(false),
          createdAt("2025-01-01"), modifiedAt("2025-01-01") {}
};