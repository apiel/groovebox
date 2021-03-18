#ifndef IO_PATTERN_H_
#define IO_PATTERN_H_

#include <SD.h>

#include "Pattern.h"
#include "io_state.h"
#include "io_storage.h"
#include "note.h"

#define PATTERN_PATH_LEN 16

Pattern pattern;
char patternPath[PATTERN_PATH_LEN];

byte currentPattern = 0;
byte currentStepSelection = 0;

void loadPattern() {
    pattern.clear();

    snprintf(patternPath, PATTERN_PATH_LEN, "parttern/%03d.io", currentPattern);
    // Serial.printf("pattern file: %s\n", patternPath);
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
    // Serial.println("No file found.");
    pattern.setDefaultName();
    // pattern.print();
}

void patternInit() {
    loadPattern();
}

void setCurrentPattern(int8_t direction) {
    currentPattern += direction;
    loadPattern();
}

void setCurrentStepSelection(int8_t direction) {
    currentStepSelection = mod(currentStepSelection + direction, STEP_COUNT);
}

void setStepDuration(int8_t direction) {
    Step* pStep = &pattern.steps[currentStepSelection];
    // Instead of STEP_COUNT could try to find the next...
    pStep->duration = constrain(pStep->duration + direction, 0, STEP_COUNT);
}

// could use keyboard there
void setStepNote(int8_t direction) {
    Step* pStep = &pattern.steps[currentStepSelection];
    pStep->set(constrain(pStep->note + direction, _C0, _B8));
}

void setStepVelocity(int8_t direction) {
    Step* pStep = &pattern.steps[currentStepSelection];
    pStep->set(constrain(pStep->velocity + direction, 0, 127));
}

bool savePattern() {
    if (sdAvailable) {
        snprintf(patternPath, PATTERN_PATH_LEN, "parttern/%03d.io",
                 currentPattern);
        File file = SD.open(patternPath, FILE_WRITE);

        if (file) {
            sprintf(storageBuffer, "%s\n", pattern.name);
            file.print(storageBuffer);
            for (byte pos = 0; pos < MAX_STEPS_IN_PATTERN; pos++) {
                Step* step = &pattern.steps[pos];
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
