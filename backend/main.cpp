#include <iostream>
#include <limits>
#include "services/AuthService.h"
#include "services/FileService.h"
#include "services/UndoService.h"
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

        // ── File Storage & Undo System ──────────────
        FileService fileService;
        UndoService undoService;

        cout << "\n========================================\n";
        cout << "       File Storage System Demo\n";
        cout << "========================================\n";

        // Upload files
        cout << "\n--- Uploading Files ---\n";
        string owner = auth.getCurrentUser()->username;
        FileRecord f1 = fileService.uploadFile("report.pdf", "/documents", owner, "pdf", 2048);
        FileRecord f2 = fileService.uploadFile("photo.png", "/images", owner, "png", 5120);
        FileRecord f3 = fileService.uploadFile("notes.txt", "/documents", owner, "txt", 512);

        undoService.recordAction("UPLOAD", FileRecord(), f1);
        undoService.recordAction("UPLOAD", FileRecord(), f2);
        undoService.recordAction("UPLOAD", FileRecord(), f3);

        // Display directory
        fileService.displayDirectory();

        // Rename
        cout << "\n--- Renaming File ---\n";
        FileRecord beforeRename = *fileService.getFileById(f1.fileId);
        fileService.renameFile(f1.fileId, "final_report.pdf");
        undoService.recordAction("RENAME", beforeRename, *fileService.getFileById(f1.fileId));

        // Move
        cout << "\n--- Moving File ---\n";
        FileRecord beforeMove = *fileService.getFileById(f2.fileId);
        fileService.moveFile(f2.fileId, "/archive");
        undoService.recordAction("MOVE", beforeMove, *fileService.getFileById(f2.fileId));

        // Undo stack
        undoService.displayUndoStack();

        // Undo
        cout << "\n--- Undoing Last 2 Actions ---\n";
        UndoAction ua1 = undoService.undo();
        UndoAction ua2 = undoService.undo();
        fileService.moveFile(ua1.after.fileId, ua1.before.filePath);
        fileService.renameFile(ua2.after.fileId, ua2.before.fileName);

        // Redo
        cout << "\n--- Redo Last Action ---\n";
        UndoAction ra1 = undoService.redo();
        fileService.renameFile(ra1.before.fileId, ra1.after.fileName);

        // Version history
        cout << "\n--- Version History ---\n";
        fileService.displayVersionHistory(f1.fileId);

        // Delete
        cout << "\n--- Deleting File ---\n";
        FileRecord deleteCopy = *fileService.getFileById(f3.fileId);
        fileService.deleteFile(f3.fileId);
        undoService.recordAction("DELETE", deleteCopy, FileRecord());

        // Final directory
        cout << "\n--- Final Directory State ---\n";
        fileService.displayDirectory();

        cout << "\n========================================\n";
        cout << "           Demo Complete! ✅             \n";
        cout << "========================================\n";

    } else {
        cout << "\nInvalid username or password." << endl;
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}