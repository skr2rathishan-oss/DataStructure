#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "services/FileService.h"
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
    AccessControlService accessControl;
    RequestService requestService;

    loadSampleData(auth, accessControl);
    fileService.loadStoredFiles();  // Load previously stored files from storage/
    fileService.syncPermissionGraph(accessControl);

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
            cout << "4. Share File with User" << endl;
            cout << "5. Change File Visibility" << endl;
            cout << "6. Check File Access" << endl;
            cout << "7. Revoke User Access" << endl;
            cout << "11. Show Permission Graph" << endl;

            cout << "\n-- Access Request Management --" << endl;
            cout << "8. Request Access to File" << endl;
            cout << "9. Show Next Access Request" << endl;
            cout << "10. Process Next Access Request" << endl;

            cout << "\n-- File Management (Real Files) --" << endl;
            cout << "19. Upload File" << endl;
            cout << "20. Download File" << endl;
            cout << "21. Delete File" << endl;
            cout << "22. Search File" << endl;
            cout << "23. View My Files" << endl;
            cout << "27. Restore Deleted File from Trash" << endl;

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

            int fileId;
            cout << "Enter file ID to share: ";
            cin >> fileId;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                cout << "File not found or deleted!" << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin" &&
                file->owner != auth.getCurrentUser()->username) {
                cout << "Only Admin or the file owner can share this file." << endl;
                break;
            }

            cout << "Enter username to grant permission: ";
            cin >> username;

            if (!auth.userExists(username)) {
                cout << "User not found!" << endl;
                break;
            }

            if (username == file->owner) {
                cout << "Owner already has access." << endl;
                break;
            }

            accessControl.grantPermission(username, fileId);
            if (file->visibility == "PRIVATE") {
                fileService.setFileVisibility(fileId, "RESTRICTED");
            }
            cout << "\n✅ Permission granted successfully!" << endl;
            cout << "   File: " << file->fileName << endl;
            cout << "   File Node: file_" << file->fileId << endl;
            cout << "   Path: " << file->filePath << endl;
            cout << "   User: " << username << endl;
            break;
        }

        case 5: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            int fileId;
            cout << "Enter file ID to change visibility: ";
            cin >> fileId;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                cout << "File not found or deleted!" << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin" &&
                file->owner != auth.getCurrentUser()->username) {
                cout << "Only Admin or the file owner can change visibility." << endl;
                break;
            }

            int visibilityChoice;
            cout << "Choose visibility:" << endl;
            cout << "1. PRIVATE" << endl;
            cout << "2. RESTRICTED" << endl;
            cout << "3. PUBLIC" << endl;
            cout << "Enter choice: ";
            cin >> visibilityChoice;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid visibility choice!" << endl;
                break;
            }

            if (visibilityChoice == 1) {
                fileService.setFileVisibility(fileId, "PRIVATE");
                accessControl.makePrivate(fileId, file->owner);
            } else if (visibilityChoice == 2) {
                fileService.setFileVisibility(fileId, "RESTRICTED");
                accessControl.makePrivate(fileId);
            } else if (visibilityChoice == 3) {
                fileService.setFileVisibility(fileId, "PUBLIC");
                accessControl.makePublic(fileId);
            } else {
                cout << "Invalid visibility choice!" << endl;
                break;
            }

            cout << "\n✅ File visibility updated." << endl;
            cout << "   File: " << file->fileName << endl;
            cout << "   File Node: file_" << file->fileId << endl;
            cout << "   Visibility: " << file->visibility << endl;
            break;
        }

        case 6: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            int fileId;
            cout << "Enter file ID to check access: ";
            cin >> fileId;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                cout << "File not found or deleted!" << endl;
                break;
            }

            cout << "Enter username to check: ";
            cin >> username;

            User* checkedUser = auth.getUser(username);
            string checkedRole = checkedUser != nullptr ? checkedUser->role : "";

            if (accessControl.canAccess(username, checkedRole, *file)) {
                cout << "\n✅ Access allowed for " << username << endl;
                cout << "   File: " << file->fileName << endl;
                cout << "   Visibility: " << file->visibility << endl;
                cout << "   File Node: file_" << file->fileId << endl;
            } else {
                cout << "\n❌ Access denied for " << username << endl;
                cout << "   File: " << file->fileName << endl;
                cout << "   Visibility: " << file->visibility << endl;
            }
            break;
        }

        case 7: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            int fileId;
            cout << "Enter file ID to revoke access: ";
            cin >> fileId;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                cout << "File not found or deleted!" << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin" &&
                file->owner != auth.getCurrentUser()->username) {
                cout << "Only Admin or the file owner can revoke access." << endl;
                break;
            }

            cout << "Enter username to revoke access: ";
            cin >> username;

            if (username == file->owner) {
                cout << "Owner access cannot be revoked." << endl;
                break;
            }

            accessControl.revokePermission(username, fileId);
            cout << "\n✅ Permission revoked successfully!" << endl;
            cout << "   File: " << file->fileName << endl;
            cout << "   File Node: file_" << file->fileId << endl;
            cout << "   User: " << username << endl;
            break;
        }

        case 8: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            string ownerUsername;
            int fileId;
            cout << "Enter owner username: ";
            cin >> ownerUsername;

            cout << "Enter file ID: ";
            cin >> fileId;

            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number." << endl;
                break;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                cout << "❌ File not found or deleted!" << endl;
                break;
            }

            if (file->owner != ownerUsername) {
                cout << "❌ The entered owner does not own this file." << endl;
                break;
            }

            if (file->owner == auth.getCurrentUser()->username) {
                cout << "❌ You already own this file." << endl;
                break;
            }

            if (accessControl.canAccess(auth.getCurrentUser()->username,
                                        auth.getCurrentUser()->role,
                                        *file)) {
                cout << "❌ You already have access to this file." << endl;
                break;
            }

            requestService.addAccessRequest(
                requestId,
                auth.getCurrentUser()->userId,
                auth.getCurrentUser()->username,
                ownerUsername,
                fileId
            );

            requestId++;
            break;
        }

        case 9: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            cout << "\n=== Next Access Request ===" << endl;
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

            requestService.processNextRequest(
                auth.getCurrentUser()->username,
                auth.getCurrentUser()->role,
                fileService,
                accessControl
            );
            break;
        }

        case 11: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first." << endl;
                break;
            }

            accessControl.showPermissions();
            break;
        }

        case 19: {
            // Upload Real File
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }

            string sourcePath;
            cout << "Enter real file path (e.g., C:\\Users\\...\\file.pdf): ";
            getline(cin >> ws, sourcePath);

            // Support paths pasted with quotes, e.g. "C:\\Users\\...\\My File.pdf"
            if (sourcePath.size() >= 2 &&
                ((sourcePath.front() == '"' && sourcePath.back() == '"') ||
                 (sourcePath.front() == '\'' && sourcePath.back() == '\''))) {
                sourcePath = sourcePath.substr(1, sourcePath.size() - 2);
            }

            FileRecord uploadedFile = fileService.uploadRealFile(sourcePath, auth.getCurrentUser()->username);

            // Check if upload was successful (valid fileId)
            if (uploadedFile.fileId > 0) {
                // Add file to permission graph and grant owner automatic access
                accessControl.addFileNode(uploadedFile.fileId);
                accessControl.grantPermission(auth.getCurrentUser()->username, uploadedFile.fileId);
                cout << "\n✅ Owner permission added automatically for "
                     << auth.getCurrentUser()->username << endl;
                cout << "   Default visibility: " << uploadedFile.visibility << endl;
                cout << "   File node: file_" << uploadedFile.fileId << endl;
            } else {
                cout << "\n❌ File upload failed - permission not granted." << endl;
            }
            break;
        }

        case 20: {
            // Download Real File
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

            if (file->isDeleted) {
                cout << "Cannot download deleted file!" << endl;
                break;
            }

            if (!accessControl.canAccess(auth.getCurrentUser()->username,
                                         auth.getCurrentUser()->role,
                                         *file)) {
                cout << "Access denied! You do not have permission for this file." << endl;
                break;
            }

            fileService.downloadRealFile(fileId);
            break;
        }

        case 21: {
            // Delete Real File
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

            if (file->isDeleted) {
                cout << "File is already deleted!" << endl;
                break;
            }

            if (auth.getCurrentUser()->role != "Admin" &&
                file->owner != auth.getCurrentUser()->username &&
                !accessControl.checkPermission(auth.getCurrentUser()->username, fileId)) {
                cout << "Access denied! You do not have permission to delete this file." << endl;
                break;
            }

            // Delete the real file (moves to trash)
            if (fileService.deleteRealFile(fileId)) {
                cout << "✅ File moved to trash successfully." << endl;
                cout << "   File ID: " << fileId << endl;
                cout << "   Trash location: " << file->filePath << endl;
                accessControl.makePrivate(fileId);
            } else {
                cout << "❌ Failed to delete file." << endl;
            }
            break;
        }

        case 22: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }

            cout << "Enter file name to search: ";
            cin >> fileName;

            fileService.searchAccessibleFiles(fileName,
                                              auth.getCurrentUser()->username,
                                              &accessControl,
                                              auth.getCurrentUser()->role);
            break;
        }

        case 23: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }

            fileService.viewAccessibleFiles(auth.getCurrentUser()->username,
                                            &accessControl,
                                            auth.getCurrentUser()->role);
            break;
        }

        case 27: {
            // Restore Deleted File from Trash
            if (!auth.isLoggedIn()) {
                cout << "Please login first!" << endl;
                break;
            }

            int fileId;
            cout << "Enter file ID to restore from trash: ";
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

            if (!file->isDeleted) {
                cout << "This file is not in trash." << endl;
                break;
            }

            if (fileService.restoreRealFile(fileId)) {
                accessControl.addFileNode(file->fileId);
                if (file->owner != "unknown") {
                    accessControl.grantPermission(file->owner, fileId);
                    cout << "Owner permission restored for: " << file->owner << endl;
                }
                if (file->visibility == "PUBLIC") {
                    accessControl.makePublic(fileId);
                }
            }
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
