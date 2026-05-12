#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include "../models/FileRecord.h"
#include "../data_structures/FileHashTable.h"
#include "../data_structures/LinkedList.h"
#include "../data_structures/Tree.h"
#include "AccessControlService.h"
#include "StorageService.h"
using namespace std;

/*
 * FileService - Manages file metadata and operations
 *
 * CURRENT STATUS: Metadata-only implementation
 * ⚠️  NOTE: filePath and fileSize are stored as METADATA only.
 *     Real file upload/download from disk storage is NOT implemented yet.
 *     This is ready for future integration with cloud storage APIs or local file system.
 *
 * Data structures used:
 * - FileHashTable: O(1) lookup by file ID
 * - BST: O(log n) search by file ID
 * - LinkedList: Linear traversal for listing/searching
 */

class FileService {
private:
    FileHashTable fileTable;
    BST fileTree;
    LinkedList<FileRecord> fileList;
    StorageService storage;
    int idCounter;

public:
    FileService() {
        idCounter = 1001;
    }

    // Upload a file (metadata only - real file storage not implemented)
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
        cout << "   Path    : " << newFile.filePath << " (metadata, not real storage)" << endl;
        cout << "   Owner   : " << newFile.owner << endl;
        cout << "   Type    : " << newFile.fileType << endl;
        cout << "   Size    : " << newFile.fileSize << " bytes" << endl;
        return newFile;
    }

    // View all files
    void viewAllFiles(string owner) {
        viewAccessibleFiles(owner, nullptr);
    }

    void viewAccessibleFiles(const string& username, AccessControlService* accessControl, const string& role = "") {
        cout << "\n--- My Files ---" << endl;
        auto* node = fileList.getHead();
        bool found = false;
        unordered_set<int> shownIds;
        while (node != nullptr) {
            // Read latest state from hash table because linked-list nodes store copies.
            FileRecord* current = fileTable.get(node->data.fileId);
            bool isOwner = current != nullptr && current->owner == username;
            bool canAccess = current != nullptr && current->owner == "unknown";
            if (current != nullptr && accessControl != nullptr) {
                canAccess = accessControl->canAccess(username, role, *current) || current->owner == "unknown";
            }

            if (current != nullptr && !current->isDeleted &&
                shownIds.find(current->fileId) == shownIds.end() &&
                canAccess) {
                cout << "  ID: " << current->fileId
                     << " | Name: " << current->fileName
                     << " | Owner: " << (current->owner == "unknown" ? "[Unregistered]" : current->owner)
                     << " | Access: " << (isOwner ? "Owned" : (current->visibility == "PUBLIC" ? "Public" : "Shared"))
                     << " | Visibility: " << current->visibility
                     << " | Path: " << current->filePath
                     << " | Size: " << current->fileSize << " bytes" << endl;
                shownIds.insert(current->fileId);
                found = true;
            }
            node = node->next;
        }
        if (!found) cout << "  No files found!" << endl;
    }

    // Search accessible files by name
    void searchFile(string fileName) {
        searchAccessibleFiles(fileName, "", nullptr);
    }

    void searchAccessibleFiles(const string& fileName,
                               const string& username,
                               AccessControlService* accessControl,
                               const string& role = "") {
        cout << "\n--- Search Results for: " << fileName << " ---" << endl;
        auto* node = fileList.getHead();
        bool found = false;
        unordered_set<int> shownIds;
        while (node != nullptr) {
            FileRecord* current = fileTable.get(node->data.fileId);
            if (current != nullptr && !current->isDeleted &&
                shownIds.find(current->fileId) == shownIds.end()) {
                bool canAccess = accessControl == nullptr || current->owner == "unknown";
                if (accessControl != nullptr) {
                    canAccess = accessControl->canAccess(username, role, *current) || current->owner == "unknown";
                }

                // Search by exact name or case-insensitive partial match
                string nodeName = current->fileName;
                string searchName = fileName;
                
                // Convert to lowercase for case-insensitive search
                transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
                transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
                
                 if (nodeName.find(searchName) != string::npos && canAccess) {
                    cout << "  ID: " << current->fileId
                         << " | Name: " << current->fileName
                         << " | Owner: " << (current->owner == "unknown" ? "[Unregistered]" : current->owner)
                         << " | Visibility: " << current->visibility
                         << " | Path: " << current->filePath << endl;
                    shownIds.insert(current->fileId);
                    found = true;
                }
            }
            node = node->next;
        }
        if (!found) cout << "  File not found!" << endl;
    }

    // Download a file (metadata simulation - real download not implemented)
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
        cout << "   Path : " << file->filePath << " (metadata path, real storage not implemented)" << endl;
        cout << "   Size : " << file->fileSize << " bytes" << endl;
        cout << "   Download complete! (simulation - real file transfer not implemented)" << endl;
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

    bool setFileVisibility(int fileId, const string& visibility) {
        FileRecord* file = fileTable.get(fileId);
        if (file == nullptr || file->isDeleted) {
            return false;
        }

        file->visibility = visibility;
        writeFileMetadata(*file);
        return true;
    }

    void syncPermissionGraph(AccessControlService& accessControl) {
        auto* node = fileList.getHead();
        unordered_set<int> syncedIds;

        while (node != nullptr) {
            FileRecord* file = fileTable.get(node->data.fileId);
            if (file != nullptr && syncedIds.find(file->fileId) == syncedIds.end()) {
                accessControl.addFileNode(file->fileId);
                if (file->owner != "unknown" && !file->owner.empty()) {
                    accessControl.grantPermission(file->owner, file->fileId);
                }
                if (file->visibility == "PUBLIC" && !file->isDeleted) {
                    accessControl.makePublic(file->fileId);
                }
                syncedIds.insert(file->fileId);
            }
            node = node->next;
        }
    }

    // Display BST file IDs
    void displayFileTree() {
        fileTree.display();
    }

    // ========== REAL FILE OPERATIONS ==========

    string getMetadataPath(const string& storagePath) {
        try {
            return storagePath + ".meta";
        } catch (const exception& e) {
            cerr << "❌ Error getting metadata path: " << e.what() << endl;
            return "";
        }
    }

    void writeFileMetadata(const FileRecord& file) {
        string metadataPath = getMetadataPath(file.filePath);
        if (metadataPath.empty()) {
            return;
        }

        ofstream metadataFile(metadataPath, ios::trunc);
        if (!metadataFile.is_open()) {
            cerr << "❌ Could not write metadata file: " << metadataPath << endl;
            return;
        }

        metadataFile << "owner=" << file.owner << '\n';
        metadataFile << "name=" << file.fileName << '\n';
        metadataFile << "type=" << file.fileType << '\n';
        metadataFile << "size=" << file.fileSize << '\n';
        metadataFile << "visibility=" << file.visibility << '\n';
    }

    string readMetadataValue(const string& metadataPath, const string& key) {
        ifstream metadataFile(metadataPath);
        if (!metadataFile.is_open()) {
            return "";
        }

        string line;
        string prefix = key + "=";
        while (getline(metadataFile, line)) {
            if (line.rfind(prefix, 0) == 0) {
                string value = line.substr(prefix.size());
                while (!value.empty() &&
                       (value.back() == '\r' || value.back() == ' ' || value.back() == '\t')) {
                    value.pop_back();
                }
                return value;
            }
        }

        return "";
    }

    // Upload a real file from source path to storage
    FileRecord uploadRealFile(string sourcePath, string owner) {
        FileRecord emptyFile;
        emptyFile.fileId = -1;  // Mark as invalid

        // Check if source file exists
        if (!storage.sourceFileExists(sourcePath)) {
            cout << "\n❌ Source file not found: " << sourcePath << endl;
            return emptyFile;
        }

        try {
            // Get file information
            string fileName = storage.getFileName(sourcePath);
            string fileExtension = storage.getFileExtension(sourcePath);
            long long fileSize = storage.getFileSize(sourcePath);

            if (fileName.empty() || fileSize <= 0) {
                cout << "\n❌ Invalid file or file is empty!" << endl;
                return emptyFile;
            }

            // Create stored file path
            string storagePath = storage.uploadFile(idCounter, sourcePath);

            if (storagePath.empty()) {
                cout << "\n❌ Failed to copy file to storage!" << endl;
                return emptyFile;
            }

            // Create FileRecord with real file information
            FileRecord newFile(idCounter, fileName, storagePath, owner, fileExtension, (int)fileSize);
            
            // Insert into all data structures
            fileTable.insert(newFile);
            fileTree.insert(idCounter);
            fileList.insertAtEnd(newFile);
            writeFileMetadata(newFile);

            cout << "\n✅ Real file uploaded successfully!" << endl;
            cout << "   File ID : " << newFile.fileId << endl;
            cout << "   Name    : " << newFile.fileName << endl;
            cout << "   Owner   : " << newFile.owner << endl;
            cout << "   Type    : " << newFile.fileType << endl;
            cout << "   Size    : " << newFile.fileSize << " bytes (actual)" << endl;
            cout << "   Storage : " << newFile.filePath << endl;

            int currentId = idCounter;
            idCounter++;
            
            // Return a valid FileRecord
            return fileTable.get(currentId) != nullptr ? *fileTable.get(currentId) : emptyFile;

        } catch (const exception& e) {
            cerr << "\n❌ Error uploading real file: " << e.what() << endl;
            return emptyFile;
        }
    }

    // Download a real file from storage to downloads folder
    void downloadRealFile(int fileId) {
        if (!fileTree.search(fileId)) {
            cout << "\n❌ File ID " << fileId << " not found!" << endl;
            return;
        }

        FileRecord* file = fileTable.get(fileId);
        if (file == nullptr || file->isDeleted) {
            cout << "\n❌ File not available!" << endl;
            return;
        }

        // Check if file path contains storage directory (real file)
        if (file->filePath.find("storage/files") == string::npos &&
            file->filePath.find("storage\\files") == string::npos) {
            cout << "\n⚠️  This is a metadata-only file, not a real file." << endl;
            return;
        }

        // Copy file to downloads
        if (storage.downloadFile(file->filePath)) {
            cout << "   File name : " << file->fileName << endl;
            cout << "   File size : " << file->fileSize << " bytes" << endl;
            cout << "   Downloaded to: " << storage.getDownloadsDir() << endl;
        } else {
            cout << "\n❌ Failed to download file!" << endl;
        }
    }

    // Delete a real file (move to trash)
    bool deleteRealFile(int fileId) {
        if (!fileTree.search(fileId)) {
            cout << "\n❌ File ID " << fileId << " not found!" << endl;
            return false;
        }

        FileRecord* file = fileTable.get(fileId);
        if (file == nullptr) {
            cout << "\n❌ File not found!" << endl;
            return false;
        }

        // Check if file path contains storage directory (real file)
        if (file->filePath.find("storage/files") == string::npos &&
            file->filePath.find("storage\\files") == string::npos) {
            cout << "\n⚠️  This is a metadata-only file." << endl;
            return false;
        }

        // Move real file to trash
        if (storage.moveToTrash(file->filePath)) {
            // Update file record
            string oldStoragePath = file->filePath;
            file->filePath = storage.getTrashPath(oldStoragePath);
            file->isDeleted = true;

            string oldMetadataPath = getMetadataPath(oldStoragePath);
            string newMetadataPath = getMetadataPath(file->filePath);
            if (!oldMetadataPath.empty() && !newMetadataPath.empty() && filesystem::exists(oldMetadataPath)) {
                try {
                    filesystem::rename(oldMetadataPath, newMetadataPath);
                } catch (const exception& e) {
                    cerr << "❌ Error moving metadata to trash: " << e.what() << endl;
                }
            }
            cout << "   File moved to trash successfully." << endl;
            return true;
        } else {
            cout << "\n❌ Failed to delete file!" << endl;
            return false;
        }
    }

    // Restore a real file from trash
    bool restoreRealFile(int fileId) {
        if (!fileTree.search(fileId)) {
            cout << "\n❌ File ID " << fileId << " not found!" << endl;
            return false;
        }

        FileRecord* file = fileTable.get(fileId);
        if (file == nullptr) {
            cout << "\n❌ File not found!" << endl;
            return false;
        }

        // Check if file is in trash
        if (!file->isDeleted ||
            (file->filePath.find("storage/trash") == string::npos &&
             file->filePath.find("storage\\trash") == string::npos)) {
            cout << "\n⚠️  This file is not in trash." << endl;
            return false;
        }

        // Move file back from trash to storage
        if (storage.restoreFromTrash(file->filePath)) {
            // Update file record
            string oldTrashPath = file->filePath;
            string storedFileName = to_string(fileId) + "_" + file->fileName;
            file->filePath = storage.getFilesDir() + "/" + storedFileName;
            file->isDeleted = false;

            string oldMetadataPath = getMetadataPath(oldTrashPath);
            string newMetadataPath = getMetadataPath(file->filePath);
            if (!oldMetadataPath.empty() && !newMetadataPath.empty() && filesystem::exists(oldMetadataPath)) {
                try {
                    filesystem::rename(oldMetadataPath, newMetadataPath);
                } catch (const exception& e) {
                    cerr << "❌ Error restoring metadata from trash: " << e.what() << endl;
                }
            }
            cout << "   File restored successfully." << endl;
            return true;
        } else {
            cout << "\n❌ Failed to restore file!" << endl;
            return false;
        }
    }

    // ========== PERSISTENT STORAGE LOADING ==========

    // Load all previously stored files from storage/files/ on startup
    void loadStoredFiles() {
        try {
            namespace fs = filesystem;
            string filesDir = "storage/files";
            string trashDir = "storage/trash";

            // Check if storage directories exist
            if (!fs::exists(filesDir) && !fs::exists(trashDir)) {
                cout << "No previously stored files found." << endl;
                return;
            }

            int activeLoadedCount = 0;
            int trashLoadedCount = 0;
            int maxFileId = 1000;
            unordered_set<int> loadedIds;

            auto loadDirectory = [&](const string& dirPath, bool isDeletedFlag, int& counter) {
                if (!fs::exists(dirPath)) return;

                for (const auto& entry : fs::directory_iterator(dirPath)) {
                    if (!entry.is_regular_file()) continue;

                    string fullPath = entry.path().string();
                    string fileName = entry.path().filename().string();

                    // Parse format: fileId_originalName
                    size_t underscorePos = fileName.find('_');
                    if (underscorePos == string::npos || underscorePos == 0) continue;

                    try {
                        int fileId = stoi(fileName.substr(0, underscorePos));

                        // Skip duplicates by ID to avoid repeated rows.
                        if (loadedIds.find(fileId) != loadedIds.end()) {
                            continue;
                        }

                        string originalName = fileName.substr(underscorePos + 1);
                        long long fileSize = fs::file_size(entry.path());
                        size_t dotPos = originalName.rfind('.');
                        string fileExtension = (dotPos != string::npos) ? originalName.substr(dotPos) : "";

                        string owner = "unknown";
                        string metadataPath = fullPath + ".meta";
                        if (fs::exists(metadataPath)) {
                            string storedOwner = readMetadataValue(metadataPath, "owner");
                            if (!storedOwner.empty()) {
                                owner = storedOwner;
                            }
                        }

                        FileRecord loadedFile(fileId, originalName, fullPath, owner, fileExtension, (int)fileSize);
                        loadedFile.isDeleted = isDeletedFlag;
                        if (fs::exists(metadataPath)) {
                            string storedVisibility = readMetadataValue(metadataPath, "visibility");
                            if (storedVisibility == "PRIVATE" ||
                                storedVisibility == "RESTRICTED" ||
                                storedVisibility == "PUBLIC") {
                                loadedFile.visibility = storedVisibility;
                            }
                        }

                        fileTable.insert(loadedFile);
                        fileTree.insert(fileId);
                        fileList.insertAtEnd(loadedFile);
                        loadedIds.insert(fileId);

                        if (fileId > maxFileId) {
                            maxFileId = fileId;
                        }

                        counter++;
                    } catch (const exception&) {
                        cerr << "Skipping file with invalid format: " << fileName << endl;
                    }
                }
            };

            // Load active files first, then trashed files.
            loadDirectory(filesDir, false, activeLoadedCount);
            loadDirectory(trashDir, true, trashLoadedCount);

            // Set idCounter to next available ID
            idCounter = maxFileId + 1;

            if (activeLoadedCount > 0 || trashLoadedCount > 0) {
                cout << "\n✅ Loaded " << activeLoadedCount
                     << " active file(s) and " << trashLoadedCount
                     << " trash file(s) from storage/" << endl;
            }

        } catch (const exception& e) {
            cerr << "Error loading stored files: " << e.what() << endl;
        }
    }
};

#endif
