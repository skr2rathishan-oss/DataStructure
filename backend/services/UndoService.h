#ifndef UNDOSERVICE_H
#define UNDOSERVICE_H

#include <iostream>
#include "../data_structures/Stack.h"
#include "../models/FileRecord.h"
#include "FileService.h"
using namespace std;

class UndoService {
private:
    Stack<FileRecord> trashStack;

public:
    UndoService() {}

    // Move file to trash
    void moveToTrash(FileRecord file) {
        trashStack.push(file);
        cout << "\n🗑️  File moved to trash: " << file.fileName << endl;
    }

    // Restore last deleted file
    void restoreLastFile(FileService& fileService) {
        if (trashStack.isEmpty()) {
            cout << "\n❌ Trash is empty! Nothing to restore." << endl;
            return;
        }

        FileRecord file = trashStack.pop();
        FileRecord* original = fileService.getFileById(file.fileId);

        if (original != nullptr) {
            original->isDeleted = false;
            cout << "\n✅ File restored successfully!" << endl;
            cout << "   Name : " << original->fileName << endl;
            cout << "   Path : " << original->filePath << endl;
        }
    }

    // View trash contents
    void viewTrash() {
        cout << "\n--- Trash ---" << endl;
        if (trashStack.isEmpty()) {
            cout << "  Trash is empty!" << endl;
            return;
        }
        cout << "  Last deleted file is on top." << endl;
        cout << "  Use Restore option to bring it back." << endl;
    }

    bool isTrashEmpty() {
        return trashStack.isEmpty();
    }
};

#endif