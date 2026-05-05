#include <iostream>
#include "../backend/services/AuthService.h"
using namespace std;

int main() {
    AuthService auth;

    // Sample users
    auth.registerUser(1, "admin", "admin123", "Admin");
    auth.registerUser(2, "user1", "user123", "User");
    auth.registerUser(3, "viewer1", "viewer123", "Viewer");

    int choice;
    string username, password;

    do {
        cout << "\n=== Smart Cloud Storage ===\n";
        cout << "1. Login\n";
        cout << "2. Show Current User\n";
        cout << "3. Logout\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
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
            auth.logout();
            cout << "Logged out successfully\n";
            break;

        case 4:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice\n";
        }

    } while (choice != 4);

    return 0;
}
