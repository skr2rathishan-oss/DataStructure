#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H

#include <iostream>
#include <filesystem>
#include <string>
using namespace std;
namespace fs = filesystem;

/*
 * StorageService - Handles real file storage operations
 * 
 * Directory structure:
 * backend/storage/
 *   ├── files/        (uploaded files)
 *   ├── downloads/    (files ready for download)
 *   └── trash/        (deleted files)
 * 
 * Naming convention: fileId_originalFileName
 * Example: 1001_report.pdf
 */

class StorageService {
private:
    string basePath = "storage";
    string filesDir = "storage/files";
    string downloadsDir = "storage/downloads";
    string trashDir = "storage/trash";

public:
    StorageService() {
        initializeDirectories();
    }

    // Create all required storage directories
    void initializeDirectories() {
        try {
            fs::create_directories(filesDir);
            fs::create_directories(downloadsDir);
            fs::create_directories(trashDir);
            cout << "\n✅ Storage directories initialized" << endl;
        } catch (const exception& e) {
            cerr << "\n❌ Failed to create storage directories: " << e.what() << endl;
        }
    }

    // Check if source file exists and is accessible
    bool sourceFileExists(const string& sourcePath) {
        try {
            return fs::exists(sourcePath) && fs::is_regular_file(sourcePath);
        } catch (const exception& e) {
            cerr << "❌ Error checking file: " << e.what() << endl;
            return false;
        }
    }

    // Get original file name from path
    string getFileName(const string& filePath) {
        try {
            return fs::path(filePath).filename().string();
        } catch (const exception& e) {
            cerr << "❌ Error getting file name: " << e.what() << endl;
            return "";
        }
    }

    // Get file extension (including the dot)
    string getFileExtension(const string& filePath) {
        try {
            string ext = fs::path(filePath).extension().string();
            return ext.empty() ? "" : ext;
        } catch (const exception& e) {
            cerr << "❌ Error getting file extension: " << e.what() << endl;
            return "";
        }
    }

    // Get actual file size in bytes
    long long getFileSize(const string& filePath) {
        try {
            if (fs::exists(filePath)) {
                return fs::file_size(filePath);
            }
            return 0;
        } catch (const exception& e) {
            cerr << "❌ Error getting file size: " << e.what() << endl;
            return 0;
        }
    }

    // Copy source file to storage/files with naming: fileId_originalFileName
    string uploadFile(int fileId, const string& sourcePath) {
        if (!sourceFileExists(sourcePath)) {
            cerr << "❌ Source file not found: " << sourcePath << endl;
            return "";
        }

        try {
            string fileName = getFileName(sourcePath);
            string storageName = to_string(fileId) + "_" + fileName;
            string storagePath = filesDir + "/" + storageName;

            fs::copy_file(sourcePath, storagePath, fs::copy_options::overwrite_existing);

            cout << "\n✅ File copied to storage: " << storagePath << endl;
            return storagePath;
        } catch (const exception& e) {
            cerr << "\n❌ Error uploading file: " << e.what() << endl;
            return "";
        }
    }

    // Copy stored file to storage/downloads for download
    bool downloadFile(const string& storagePath) {
        if (!fs::exists(storagePath)) {
            cerr << "❌ Stored file not found: " << storagePath << endl;
            return false;
        }

        try {
            string fileName = fs::path(storagePath).filename().string();
            string downloadPath = downloadsDir + "/" + fileName;

            fs::copy_file(storagePath, downloadPath, fs::copy_options::overwrite_existing);

            cout << "\n✅ File copied to downloads: " << downloadPath << endl;
            return true;
        } catch (const exception& e) {
            cerr << "\n❌ Error downloading file: " << e.what() << endl;
            return false;
        }
    }

    // Move stored file from storage/files to storage/trash
    bool moveToTrash(const string& storagePath) {
        if (!fs::exists(storagePath)) {
            cerr << "❌ File not found: " << storagePath << endl;
            return false;
        }

        try {
            string fileName = fs::path(storagePath).filename().string();
            string trashPath = trashDir + "/" + fileName;

            fs::rename(storagePath, trashPath);

            cout << "\n🗑️  File moved to trash: " << trashPath << endl;
            return true;
        } catch (const exception& e) {
            cerr << "\n❌ Error moving file to trash: " << e.what() << endl;
            return false;
        }
    }

    // Move file from trash back to storage/files for restore
    bool restoreFromTrash(const string& trashPath) {
        if (!fs::exists(trashPath)) {
            cerr << "❌ Trash file not found: " << trashPath << endl;
            return false;
        }

        try {
            string fileName = fs::path(trashPath).filename().string();
            string storagePath = filesDir + "/" + fileName;

            fs::rename(trashPath, storagePath);

            cout << "\n✅ File restored from trash: " << storagePath << endl;
            return true;
        } catch (const exception& e) {
            cerr << "\n❌ Error restoring from trash: " << e.what() << endl;
            return false;
        }
    }

    // Get trash path for a file (when it's in storage/files)
    string getTrashPath(const string& storagePath) {
        try {
            string fileName = fs::path(storagePath).filename().string();
            return trashDir + "/" + fileName;
        } catch (const exception& e) {
            cerr << "❌ Error getting trash path: " << e.what() << endl;
            return "";
        }
    }

    // Clean up a file from trash (permanent delete)
    bool deleteFromTrash(const string& trashPath) {
        try {
            if (fs::exists(trashPath)) {
                fs::remove(trashPath);
                cout << "\n🗑️  File permanently deleted: " << trashPath << endl;
                return true;
            }
            return false;
        } catch (const exception& e) {
            cerr << "\n❌ Error deleting file: " << e.what() << endl;
            return false;
        }
    }

    // Get storage directories for display
    string getFilesDir() { return filesDir; }
    string getDownloadsDir() { return downloadsDir; }
    string getTrashDir() { return trashDir; }
};

#endif
