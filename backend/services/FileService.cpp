#include "FileService.h"
#include <iostream>
#include <sstream>
using namespace std;

FileService::FileService() : idCounter(1000) {}

FileService::~FileService() {
    for (auto& pair : versionHistory) delete pair.second;
}

string FileService::generateId() {
    return "FILE_" + to_string(idCounter++);
}

FileRecord FileService::uploadFile(string fileName, string filePath,
                                    string owner, string fileType, int fileSize) {
    string id = generateId();
    FileRecord record(id, fileName, filePath, owner, fileType, fileSize);

    fileIndex.insert(id, record);
    directoryTree.insertPath(filePath + "/" + fileName, true);

    if (!versionHistory.count(id)) {
        versionHistory[id] = new LinkedList();
    }
    versionHistory[id]->addVersion(record);

    cout << "✅ File uploaded: " << fileName << " [ID: " << id << "]\n";
    return record;
}

bool FileService::deleteFile(string fileId) {
    FileRecord* rec = fileIndex.get(fileId);
    if (!rec) {
        cout << "❌ File not found: " << fileId << "\n";
        return false;
    }
    string path = rec->filePath + "/" + rec->fileName;
    directoryTree.deletePath(path);
    fileIndex.remove(fileId);
    cout << "🗑️  File deleted: " << rec->fileName << " [ID: " << fileId << "]\n";
    return true;
}

bool FileService::renameFile(string fileId, string newName) {
    FileRecord* rec = fileIndex.get(fileId);
    if (!rec) {
        cout << "❌ File not found: " << fileId << "\n";
        return false;
    }
    string oldName = rec->fileName;
    versionHistory[fileId]->addVersion(*rec);
    directoryTree.deletePath(rec->filePath + "/" + rec->fileName);
    rec->fileName = newName;
    directoryTree.insertPath(rec->filePath + "/" + newName, true);
    fileIndex.insert(fileId, *rec);
    cout << "✏️  File renamed: " << oldName << " → " << newName << "\n";
    return true;
}

bool FileService::moveFile(string fileId, string newPath) {
    FileRecord* rec = fileIndex.get(fileId);
    if (!rec) {
        cout << "❌ File not found: " << fileId << "\n";
        return false;
    }
    string oldPath = rec->filePath;
    versionHistory[fileId]->addVersion(*rec);
    directoryTree.deletePath(rec->filePath + "/" + rec->fileName);
    rec->filePath = newPath;
    directoryTree.insertPath(newPath + "/" + rec->fileName, true);
    fileIndex.insert(fileId, *rec);
    cout << "📦 File moved: " << rec->fileName << " from " << oldPath << " to " << newPath << "\n";
    return true;
}

FileRecord* FileService::getFileById(string fileId) {
    return fileIndex.get(fileId);
}

bool FileService::fileExists(string fileId) {
    return fileIndex.exists(fileId);
}

void FileService::addVersion(string fileId, FileRecord record) {
    if (!versionHistory.count(fileId))
        versionHistory[fileId] = new LinkedList();
    versionHistory[fileId]->addVersion(record);
}

FileRecord FileService::getPreviousVersion(string fileId) {
    if (!versionHistory.count(fileId))
        throw runtime_error("No history for file: " + fileId);
    return versionHistory[fileId]->getPreviousVersion();
}

void FileService::displayDirectory() {
    directoryTree.display();
}

void FileService::displayFileIndex() {
    fileIndex.display();
}

void FileService::displayVersionHistory(string fileId) {
    if (!versionHistory.count(fileId)) {
        cout << "No history found for: " << fileId << "\n";
        return;
    }
    versionHistory[fileId]->displayHistory(fileId);
}