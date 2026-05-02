#pragma once
#include <string>
#include "../data_structures/Stack.h"
#include "../models/FileRecord.h"
using namespace std;

struct UndoAction {
    string actionType;
    FileRecord before;
    FileRecord after;

    UndoAction() {}
    UndoAction(string type, FileRecord b, FileRecord a)
        : actionType(type), before(b), after(a) {}

    friend ostream& operator<<(ostream& os, const UndoAction& a) {
        os << a.actionType << ":" << a.after.fileName;
        return os;
    }
};

class UndoService {
private:
    Stack<UndoAction> undoStack;
    Stack<UndoAction> redoStack;

public:
    UndoService();

    void recordAction(string actionType, FileRecord before, FileRecord after);
    UndoAction undo();
    UndoAction redo();

    bool canUndo();
    bool canRedo();

    void displayUndoStack();
    void displayRedoStack();
};