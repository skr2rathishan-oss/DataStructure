#include "UndoService.h"
#include <iostream>
using namespace std;

UndoService::UndoService() {}

void UndoService::recordAction(string actionType, FileRecord before, FileRecord after) {
    UndoAction action(actionType, before, after);
    undoStack.push(action);
    while (!redoStack.isEmpty()) redoStack.pop();
    cout << "📝 Action recorded: " << actionType << " on " << after.fileName << "\n";
}

UndoAction UndoService::undo() {
    if (!canUndo()) throw runtime_error("Nothing to undo!");
    UndoAction action = undoStack.pop();
    redoStack.push(action);
    cout << "↩️  Undo: " << action.actionType << " on " << action.after.fileName;
    cout << " → Restored to: " << action.before.fileName << "\n";
    return action;
}

UndoAction UndoService::redo() {
    if (!canRedo()) throw runtime_error("Nothing to redo!");
    UndoAction action = redoStack.pop();
    undoStack.push(action);
    cout << "↪️  Redo: " << action.actionType << " on " << action.before.fileName;
    cout << " → Re-applied: " << action.after.fileName << "\n";
    return action;
}

bool UndoService::canUndo() { return !undoStack.isEmpty(); }
bool UndoService::canRedo() { return !redoStack.isEmpty(); }

void UndoService::displayUndoStack() {
    cout << "\n--- Undo History (" << undoStack.getSize() << " actions) ---\n";
    undoStack.display();
}

void UndoService::displayRedoStack() {
    cout << "\n--- Redo History (" << redoStack.getSize() << " actions) ---\n";
    redoStack.display();
}