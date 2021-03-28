#ifndef IO_STORAGE_H_
#define IO_STORAGE_H_

#include <Arduino.h>
#include <SD.h>

#define STORAGE_BUFFER_SIZE 32
#define STORAGE_VALUES_SIZE 20

char storageBuffer[STORAGE_BUFFER_SIZE];
int storageValues[STORAGE_VALUES_SIZE];

bool assignStorageValues(File* file) {
    char c;
    byte pos = -1;
    byte i = 0;
    while (file->available() && i < STORAGE_VALUES_SIZE) {
        pos++;
        c = file->read();
        storageBuffer[pos] = c;
        if (c == ' ' || c == '\n') {
            storageBuffer[pos] = '\0';
            storageValues[i] = atoi(storageBuffer);
            i++;
            pos = -1;
            if (c == '\n') {
                return true;
            }
        }
    }
    return false;
}

#endif
