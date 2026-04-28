#pragma once
#include <string>
#include <unordered_map>
#include "../models/FileRecord.h"
#include "../data_structures/HashTable.h"
#include "../data_structures/Tree.h"
#include "../data_structures/LinkedList.h"
using namespace std;

class FileService {
private:
    HashTable fileIndex;
    Tree directoryTree;
    unordered_map<string, LinkedList*> versionHistory;
    int idCounter;
    string generateId();

public:
    FileService();
    ~FileService();

    FileRecord uploadFile(string fileName, string filePath, string owner,
                          string fileType, int fileSize);

    bool deleteFile(string fileId);
    bool renameFile(string fileId, string newName);
    bool moveFile(string fileId, string newPath);

    FileRecord* getFileById(string fileId);
    bool fileExists(string fileId);

    void addVersion(string fileId, FileRecord record);
    FileRecord getPreviousVersion(string fileId);

    void displayDirectory();
    void displayFileIndex();
    void displayVersionHistory(string fileId);
};