#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "sample_data/sample_data.h"
using namespace std;

int main() {
    AuthService auth;
    loadSampleUsers(auth);

    string username, password;

    cout << "=== Smart Cloud Storage Login ===" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
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
