#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "sample_data/sample_data.h"
using namespace std;

int main() {
    AuthService auth;
    loadSampleUsers(auth);

    bool running = true;

    while (running) {
        cout << "\n=== Smart Cloud Storage ===" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";

        int choice = 0;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again." << endl;
            continue;
        }

        if (choice == 1) {
            string username, password;
            cout << "New username: ";
            cin >> username;
            cout << "New password: ";
            cin >> password;

            if (auth.registerUser(username, password)) {
                cout << "Registration successful! You can now log in." << endl;
            } else {
                cout << "Registration failed. Username may already exist." << endl;
            }
        } else if (choice == 2) {
            string username, password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            if (auth.login(username, password)) {
                cout << "\nLogin successful!" << endl;
                cout << "Welcome: " << auth.getCurrentUser()->username << endl;
                cout << "Role: " << auth.getCurrentUser()->role << endl;
                running = false;
            } else {
                cout << "\nInvalid username or password." << endl;
            }
        } else if (choice == 3) {
            running = false;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
