#ifndef FILEHASHTABLE_H
#define FILEHASHTABLE_H

#include "../models/FileRecord.h"
using namespace std;

class FileHashTable {
private:
    static const int SIZE = 50;
    FileRecord table[SIZE];
    bool occupied[SIZE];

    int hashFunction(int fileId) {
        return fileId % SIZE;
    }

public:
    FileHashTable() {
        for (int i = 0; i < SIZE; i++) {
            occupied[i] = false;
        }
    }

    void insert(FileRecord file) {
        int index = hashFunction(file.fileId);

        while (occupied[index]) {
            if (table[index].fileId == file.fileId) {
                table[index] = file;
                return;
            }
            index = (index + 1) % SIZE;
        }

        table[index] = file;
        occupied[index] = true;
    }

    FileRecord* get(int fileId) {
        int index = hashFunction(fileId);
        int start = index;

        while (occupied[index]) {
            if (table[index].fileId == fileId) {
                return &table[index];
            }
            index = (index + 1) % SIZE;
            if (index == start) break;
        }
        return nullptr;
    }

    bool remove(int fileId) {
        int index = hashFunction(fileId);
        int start = index;

        while (occupied[index]) {
            if (table[index].fileId == fileId) {
                occupied[index] = false;
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