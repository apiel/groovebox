#ifndef IO_PATTERN_STORAGE_H_
#define IO_PATTERN_STORAGE_H_

#include <SD.h>

#include "Pattern.h"
#include "io_state.h"
#include "io_storage.h"

#define PATTERN_COUNT 256
Pattern patterns[PATTERN_COUNT];

void setPatternPath(byte pos) {
    setFilePath("pattern/%03d.io", pos);
}

void loadPattern(byte patternPos) {
    patterns[patternPos].clear();
    Serial.println("loadPattern.");
    setPatternPath(patternPos);
    // Serial.printf("pattern file: %s\n", patternPath);
    if (sdAvailable && SD.exists(filePath)) {
        File file = SD.open(filePath);
        if (file) {
            String name = file.readStringUntil('\n');
            patterns[patternPos].pos = patternPos;
            patterns[patternPos].setName(name.c_str());
            // pattern.stepCount = 0;
            while (file.available() && assignStorageValues(&file)) {
                patterns[patternPos].add(
                    (byte)storageValues[0], (byte)storageValues[1],
                    (byte)storageValues[2], (byte)storageValues[3],
                    storageValues[4] == 1);
                // maybe it would be better to read the step count from a var in
                // the file
                // pattern.stepCount =
                //     constrain(pattern.stepCount + 1, 0,
                //     MAX_STEPS_IN_PATTERN);
            }
            file.close();
            return;
        }
    }
    Serial.println("No file found.");
    patterns[patternPos].setDefaultName();
}

bool savePattern(byte patternPos) {
    Serial.println("savePattern");
    if (sdAvailable) {
        setPatternPath(patternPos);
        // SD.remove(patternPath);
        File file = SD.open(filePath, FILE_WRITE);

        if (file) {
            file.seek(0);
            sprintf(storageBuffer, "%s\n", patterns[patternPos].name);
            file.print(storageBuffer);
            // Serial.printf("Save pattern %s with %d steps\n", patternPath,
            // pattern.stepCount);
            for (byte pos = 0; pos < patterns[patternPos].stepCount; pos++) {
                Step* step = &patterns[patternPos].steps[pos];
                sprintf(storageBuffer, "%d %d %d %d %d\n", (int)pos,
                        (int)step->note, (int)step->duration,
                        (int)step->velocity, step->slide ? 1 : 0);
                file.print(storageBuffer);
            }
            file.close();
            return true;
        }
    }
    Serial.println("- failed to open file for writing");
    return false;
}

#endif
