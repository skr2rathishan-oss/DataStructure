#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "services/AccessControlService.h"
#include "services/RequestService.h"
#include "sample_data/sample_data.h"
using namespace std;

// Helper function to validate numeric input
void clearInputStream() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    AuthService auth;
    AccessControlService accessControl;
    RequestService requestService;

    loadSampleData(auth, accessControl);
    loadSampleRequests(requestService);

    int choice;
    string username, password;
    string fileName;
    int requestId = 100;

    do {
        cout << "\n=== Smart Cloud Storage ===\n";
        cout << "1. Login\n";
        cout << "2. Show Current User\n";
        cout << "3. Grant File Permission\n";
        cout << "4. Check File Permission\n";
        cout << "5. Revoke File Permission\n";
        cout << "6. Show Permission Graph\n";
        cout << "7. Create Upload Request\n";
        cout << "8. Show Next Request\n";
        cout << "9. Process Next Request\n";
        cout << "10. Logout\n";
        cout << "11. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        // Validate input
        if (cin.fail()) {
            clearInputStream();
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
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
                cout << "Login successful\n";
                cout << "Welcome, " << auth.getCurrentUser()->username << endl;
                cout << "Role: " << auth.getCurrentUser()->role << endl;
            } else {
                cout << "Invalid username or password\n";
            }
            break;

        case 2:
            if (auth.isLoggedIn()) {
                cout << "Current user: " << auth.getCurrentUser()->username
                     << " (" << auth.getCurrentUser()->role << ")\n";
            } else {
                cout << "No user is logged in\n";
            }
            break;

        case 3:
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            if (auth.getCurrentUser()->role != "Admin") {
                cout << "Only Admin can grant permission\n";
                break;
            }

            cout << "Enter username to grant permission: ";
            cin >> username;
            cout << "Enter file name (example: file101): ";
            cin >> fileName;

            accessControl.grantPermission(username, fileName);
            cout << "Permission granted successfully\n";
            break;

        case 4:
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            cout << "Enter file name to check access: ";
            cin >> fileName;

            if (accessControl.checkPermission(auth.getCurrentUser()->username, fileName)) {
                cout << "Access allowed for " << auth.getCurrentUser()->username << endl;
            } else {
                cout << "Access denied for " << auth.getCurrentUser()->username << endl;
            }
            break;

        case 5:
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            if (auth.getCurrentUser()->role != "Admin") {
                cout << "Only Admin can revoke permission\n";
                break;
            }

            cout << "Enter username to revoke permission: ";
            cin >> username;
            cout << "Enter file name (example: file101): ";
            cin >> fileName;

            accessControl.revokePermission(username, fileName);
            cout << "Permission revoked successfully\n";
            break;

        case 6:
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            accessControl.showPermissions();
            break;

        case 7: {
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            cout << "Enter file ID for upload request: ";
            int fileId;
            cin >> fileId;

            // Validate file ID input
            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid file ID! Please enter a number.\n";
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

        case 8:
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            requestService.showNextRequest();
            break;

        case 9:
            if (!auth.isLoggedIn()) {
                cout << "Please login first\n";
                break;
            }

            if (auth.getCurrentUser()->role != "Admin") {
                cout << "Only Admin can process requests\n";
                break;
            }

            requestService.processNextRequest();
            break;

        case 10:
            if (!auth.isLoggedIn()) {
                cout << "No user is currently logged in\n";
            } else {
                auth.logout();
                cout << "Logged out successfully\n";
            }
            break;

        case 11:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice\n";
        }

    } while (choice != 11);

    return 0;
}
