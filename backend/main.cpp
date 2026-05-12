#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "services/FileService.h"
#include "services/UndoService.h"
#include "services/AccessControlService.h"
#include "services/RequestService.h"
#include "sample_data/sample_data.h"

using namespace std;

void clearInputStream() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    AuthService auth;
    FileService fileService;
    UndoService undoService;
    AccessControlService accessControl;
    RequestService requestService;

    loadSampleData(auth, accessControl);
    loadSampleRequests(requestService);

    int choice;
    string username, password;
    string fileName;
    int requestId = 100;

    do {
        cout << "\n=== Smart Cloud Storage ===" << endl;

        if (!auth.isLoggedIn()) {
            cout << "1. Login" << endl;
            cout << "0. Exit" << endl;
        } else {
            cout << "Logged in as: " << auth.getCurrentUser()->username
                 << " (" << auth.getCurrentUser()->role << ")" << endl;

            cout << "\n-- Account --" << endl;
            cout << "2. Show Current User" << endl;
            cout << "3. Logout" << endl;

            cout << "\n-- Permission Management --" << endl;
            cout << "4. Grant File Permission" << endl;
            cout << "5. Check File Permission" << endl;
            cout << "6. Revoke File Permission" << endl;
            cout << "7. Show Permission Graph" << endl;

            cout << "\n-- Request Management --" << endl;
            cout << "8. Create Upload Request" << endl;
            cout << "9. Show Next Request" << endl;
            cout << "10. Process Next Request" << endl;

            cout << "\n-- File Management --" << endl;
            cout << "19. Upload File" << endl;
            cout << "20. Download File" << endl;
            cout << "21. Delete File" << endl;
            cout << "22. Search File" << endl;
            cout << "23. View My Files" << endl;
            cout << "27. Restore Last Deleted File" << endl;

            cout << "\n0. Exit" << endl;
        }

        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInputStream();
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (choice) {

        case 1: {
            if (auth.isLoggedIn()) {
                cout << "A user is already logged in: "
                     << auth.getCurrentUser()->username << endl;
                break;
            }

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
            if (!auth.isLoggedIn()) {
                cout << "No user is currently logged in." << endl;
            } else {
                auth.logout();
                cout << "Logged out successfully!" << endl;
            }
            break;
        }

        case 4: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin") {
                cout << "Only Admin can grant permission." << endl;
                break;
            }

            cout << "Enter username to grant permission: ";
            cin >> username;

            cout << "Enter file name: ";
            cin >> fileName;

            accessControl.grantPermission(username, fileName);
            cout << "Permission granted successfully." << endl;
            break;
        }

        case 5: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            cout << "Enter file name to check access: ";
            cin >> fileName;

            if (accessControl.checkPermission(auth.getCurrentUser()->username, fileName)) {
                cout << "Access allowed for "
                     << auth.getCurrentUser()->username << endl;
            } else {
                cout << "Access denied for "
                     << auth.getCurrentUser()->username << endl;
            }
            break;
        }

        case 6: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin") {
                cout << "Only Admin can revoke permission." << endl;
                break;
            }

            cout << "Enter username to revoke permission: ";
            cin >> username;

            cout << "Enter file name: ";
            cin >> fileName;

            accessControl.revokePermission(username, fileName);
            cout << "Permission revoked successfully." << endl;
            break;
        }

        case 7: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            accessControl.showPermissions();
            break;
        }

        case 8: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            int fileId;
            cout << "Enter file ID for upload request: ";
            cin >> fileId;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            requestService.addRequest(
                requestId,
                auth.getCurrentUser()->userId,
                fileId,
                "Upload"
            );

            cout << "Upload request created by "
                 << auth.getCurrentUser()->username << endl;

            requestId++;
            break;
        }

        case 9: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            requestService.showNextRequest();
            break;
        }

        case 10: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin") {
                cout << "Only Admin can process requests." << endl;
                break;
            }

            requestService.processNextRequest();
            break;
        }

        case 19: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }

            string filePath, fileType;
            int fileSize;

            cout << "Enter file name: ";
            cin >> fileName;

            cout << "Enter file path: ";
            cin >> filePath;

            cout << "Enter file type: ";
            cin >> fileType;

            cout << "Enter file size (bytes): ";
            cin >> fileSize;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file size! Please enter a number." << endl;
                break;
            }

            fileService.uploadFile(
                fileName,
                filePath,
                auth.getCurrentUser()->username,
                fileType,
                fileSize
            );

            // add file into permission graph and give owner automatic access
            accessControl.addFileNode(fileName);
            accessControl.grantPermission(auth.getCurrentUser()->username, fileName);

            cout << "Owner permission added automatically for "
                 << auth.getCurrentUser()->username << endl;
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

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);

            if (file == nullptr) {
                cout << "File not found!" << endl;
                break;
            }

            if (!accessControl.checkPermission(auth.getCurrentUser()->username, file->fileName)) {
                cout << "Access denied! You do not have permission for this file." << endl;
                break;
            }

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

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);

            if (file == nullptr) {
                cout << "File not found!" << endl;
                break;
            }

            if (!accessControl.checkPermission(auth.getCurrentUser()->username, file->fileName)) {
                cout << "Access denied! You do not have permission to delete this file." << endl;
                break;
            }

            undoService.moveToTrash(*file);
            fileService.deleteFile(fileId);
            accessControl.revokePermission(auth.getCurrentUser()->username, file->fileName);

            cout << "File deleted and moved to trash." << endl;
            break;
        }

        case 22: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }

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

        case 0:
            cout << "Exiting..." << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);

    return 0;
}
