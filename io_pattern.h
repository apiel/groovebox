#ifndef IO_PATTERN_H_
#define IO_PATTERN_H_

#include <SD.h>

#include "Pattern.h"
#include "io_state.h"
#include "io_storage.h"

#define PATTERN_PATH_LEN 16

Pattern pattern;
char patternPath[PATTERN_PATH_LEN];

byte currentPattern = 0;

void loadPattern() {
    pattern.clear();

    snprintf(patternPath, PATTERN_PATH_LEN, "parttern/%03d.io", currentPattern);
    if (sdAvailable && SD.exists(patternPath)) {
        File file = SD.open(patternPath);
        if (file) {
            String name = file.readStringUntil('\n');
            pattern.setName(name.c_str());
            while (file.available() && assignStorageValues(&file)) {
                pattern.add((byte)storageValues[0], (byte)storageValues[1],
                            (byte)storageValues[2], (byte)storageValues[3],
                            storageValues[4] == 1);
            }
            file.close();
            return;
        }
    }
    pattern.setDefaultName();
    // pattern.print();
}

void setCurrentPattern(int8_t direction) {
    currentPattern += direction;
    loadPattern();
}

#endif
