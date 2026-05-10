#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <iostream>
#include <string>
#include "../models/FileRecord.h"
#include "../data_structures/FileHashTable.h"
#include "../data_structures/LinkedList.h"
#include "../data_structures/Tree.h"
using namespace std;

class FileService {
private:
    FileHashTable fileTable;
    BST fileTree;
    LinkedList<FileRecord> fileList;
    int idCounter;

public:
    FileService() {
        idCounter = 1001;
    }

    // Upload a file
    FileRecord uploadFile(string fileName, string filePath,
                          string owner, string fileType, int fileSize) {
        FileRecord newFile(idCounter, fileName, filePath, owner, fileType, fileSize);
        fileTable.insert(newFile);
        fileTree.insert(idCounter);
        fileList.insertAtEnd(newFile);
        idCounter++;
        cout << "\n✅ File uploaded successfully!" << endl;
        cout << "   File ID : " << newFile.fileId << endl;
        cout << "   Name    : " << newFile.fileName << endl;
        cout << "   Path    : " << newFile.filePath << endl;
        cout << "   Owner   : " << newFile.owner << endl;
        cout << "   Type    : " << newFile.fileType << endl;
        cout << "   Size    : " << newFile.fileSize << " bytes" << endl;
        return newFile;
    }

    // View all files
    void viewAllFiles(string owner) {
        cout << "\n--- My Files ---" << endl;
        auto* node = fileList.getHead();
        bool found = false;
        while (node != nullptr) {
            if (node->data.owner == owner && !node->data.isDeleted) {
                cout << "  ID: " << node->data.fileId
                     << " | Name: " << node->data.fileName
                     << " | Path: " << node->data.filePath
                     << " | Size: " << node->data.fileSize << " bytes" << endl;
                found = true;
            }
            node = node->next;
        }
        if (!found) cout << "  No files found!" << endl;
    }

    // Search a file by name
    void searchFile(string fileName) {
        cout << "\n--- Search Results for: " << fileName << " ---" << endl;
        auto* node = fileList.getHead();
        bool found = false;
        while (node != nullptr) {
            if (node->data.fileName == fileName && !node->data.isDeleted) {
                cout << "  ID: " << node->data.fileId
                     << " | Name: " << node->data.fileName
                     << " | Owner: " << node->data.owner
                     << " | Path: " << node->data.filePath << endl;
                found = true;
            }
            node = node->next;
        }
        if (!found) cout << "  File not found!" << endl;
    }

    // Download a file
    void downloadFile(int fileId) {
        if (!fileTree.search(fileId)) {
            cout << "\n❌ File ID " << fileId << " not found!" << endl;
            return;
        }
        FileRecord* file = fileTable.get(fileId);
        if (file == nullptr || file->isDeleted) {
            cout << "\n❌ File not available!" << endl;
            return;
        }
        cout << "\n⬇️  Downloading file..." << endl;
        cout << "   Name : " << file->fileName << endl;
        cout << "   Path : " << file->filePath << endl;
        cout << "   Size : " << file->fileSize << " bytes" << endl;
        cout << "   Download complete!" << endl;
    }

    // Delete a file (mark as deleted)
    bool deleteFile(int fileId) {
        if (!fileTree.search(fileId)) {
            cout << "\n❌ File ID " << fileId << " not found!" << endl;
            return false;
        }
        FileRecord* file = fileTable.get(fileId);
        if (file == nullptr) {
            cout << "\n❌ File not found!" << endl;
            return false;
        }
        file->isDeleted = true;
        return true;
    }

    // Get file by ID
    FileRecord* getFileById(int fileId) {
        return fileTable.get(fileId);
    }

    // Display BST file IDs
    void displayFileTree() {
        fileTree.display();
    }
};

#endif