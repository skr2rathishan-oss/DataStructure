#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "sample_data/sample_data.h"
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
        cout << "\nLogin successful!" << endl;
        cout << "Welcome: " << auth.getCurrentUser()->username << endl;
        cout << "Role: " << auth.getCurrentUser()->role << endl;
    } else {
        cout << "\nInvalid username or password." << endl;
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}