#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "services/FileService.h"
#include "services/UndoService.h"
#include "sample_data/sample_data.h"
using namespace std;

int main() {
    AuthService auth;
    FileService fileService;
    UndoService undoService;

    loadSampleUsers(auth);

    int choice;
    string username, password;

    do {
        cout << "\n=== Smart Cloud Storage ===" << endl;

        if (!auth.isLoggedIn()) {
            cout << "1. Login" << endl;
            cout << "4. Exit" << endl;
        } else {
            cout << "Logged in as: " << auth.getCurrentUser()->username
                 << " (" << auth.getCurrentUser()->role << ")" << endl;
            cout << "\n-- File Management --" << endl;
            cout << "19. Upload File" << endl;
            cout << "20. Download File" << endl;
            cout << "21. Delete File" << endl;
            cout << "22. Search File" << endl;
            cout << "23. View My Files" << endl;
            cout << "27. Restore Last Deleted File" << endl;
            cout << "\n-- Account --" << endl;
            cout << "2. Show Current User" << endl;
            cout << "3. Logout" << endl;
            cout << "4. Exit" << endl;
        }

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1: {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (auth.login(username, password)) {
                cout << "Login successful!" << endl;
                cout << "Welcome, " << auth.getCurrentUser()->username << endl;
                cout << "Role: " << auth.getCurrentUser()->role << endl;
            } else {
                cout << "Invalid username or password!" << endl;
            }
            break;
        }

        case 2: {
            if (auth.isLoggedIn()) {
                cout << "Current user: " << auth.getCurrentUser()->username
                     << " (" << auth.getCurrentUser()->role << ")" << endl;
            } else {
                cout << "No user is logged in!" << endl;
            }
            break;
        }

        case 3: {
            auth.logout();
            cout << "Logged out successfully!" << endl;
            break;
        }

        case 19: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }
            string fileName, filePath, fileType;
            int fileSize;
            cout << "Enter file name: ";
            cin >> fileName;
            cout << "Enter file path: ";
            cin >> filePath;
            cout << "Enter file type: ";
            cin >> fileType;
            cout << "Enter file size (bytes): ";
            cin >> fileSize;
            fileService.uploadFile(fileName, filePath,
                auth.getCurrentUser()->username, fileType, fileSize);
            break;
        }

        case 20: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }
            int fileId;
            cout << "Enter file ID: ";
            cin >> fileId;
            fileService.downloadFile(fileId);
            break;
        }

        case 21: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }
            int fileId;
            cout << "Enter file ID to delete: ";
            cin >> fileId;
            FileRecord* file = fileService.getFileById(fileId);
            if (file != nullptr) {
                undoService.moveToTrash(*file);
                fileService.deleteFile(fileId);
            } else {
                cout << "File not found!" << endl;
            }
            break;
        }

        case 22: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }
            string fileName;
            cout << "Enter file name to search: ";
            cin >> fileName;
            fileService.searchFile(fileName);
            break;
        }

        case 23: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }
            fileService.viewAllFiles(auth.getCurrentUser()->username);
            break;
        }

        case 27: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }
            undoService.restoreLastFile(fileService);
            break;
        }

        case 4:
            cout << "Exiting..." << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 4);

    return 0;
}