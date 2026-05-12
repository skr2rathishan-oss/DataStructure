#ifndef FILEHASHTABLE_H
#define FILEHASHTABLE_H

#include "../models/FileRecord.h"
using namespace std;

class FileHashTable {
private:
    // Slot state for handling deletions without breaking probing chain
    enum SlotState { EMPTY, OCCUPIED, DELETED };

    // SIZE set to prime number for better hash distribution
    // Increased from 50 to 251 for testing ~100 files
    static const int SIZE = 251;

    struct Slot {
        FileRecord file;
        SlotState state;
        Slot() : state(EMPTY) {}
    };

    Slot table[SIZE];

    int hashFunction(int fileId) {
        return fileId % SIZE;
    }

public:
    FileHashTable() {
        for (int i = 0; i < SIZE; i++) {
            table[i].state = EMPTY;
        }
    }

    void insert(FileRecord file) {
        int index = hashFunction(file.fileId);
        int iterations = 0;

        // Probe until we find EMPTY slot or matching fileId
        while (table[index].state == OCCUPIED && iterations < SIZE) {
            if (table[index].file.fileId == file.fileId) {
                // Update existing
                table[index].file = file;
                table[index].state = OCCUPIED;
                return;
            }
            index = (index + 1) % SIZE;
            iterations++;
        }

        if (iterations >= SIZE) {
            cerr << "❌ FileHashTable is full! Cannot insert file ID: " << file.fileId << endl;
            return;
        }

        // Found EMPTY or DELETED slot
        table[index].file = file;
        table[index].state = OCCUPIED;
    }

    FileRecord* get(int fileId) {
        int index = hashFunction(fileId);
        int start = index;

        // Linear probing with tombstone handling
        while (table[index].state != EMPTY) {
            if (table[index].state == OCCUPIED && table[index].file.fileId == fileId) {
                return &table[index].file;
            }
            index = (index + 1) % SIZE;
            if (index == start) break;
        }
        return nullptr;
    }

    bool remove(int fileId) {
        int index = hashFunction(fileId);
        int start = index;

        // Linear probing with tombstone handling
        while (table[index].state != EMPTY) {
            if (table[index].state == OCCUPIED && table[index].file.fileId == fileId) {
                // Mark as DELETED instead of clearing - preserves probing chain
                table[index].state = DELETED;
                return true;
            }
            index = (index + 1) % SIZE;
            if (index == start) break;
        }
        return false;
    }

    bool exists(int fileId) {
        return get(fileId) != nullptr;
    }
};

#endif