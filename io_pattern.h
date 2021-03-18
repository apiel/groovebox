#ifndef IO_PATTERN_H_
#define IO_PATTERN_H_

#include <SD.h>

#include "Pattern.h"
#include "io_state.h"

#define PATTERN_PATH_LEN 16

Pattern pattern;
char patternPath[PATTERN_PATH_LEN];

byte currentPattern = 0;

void loadPattern() {
    pattern.clear();

    snprintf(patternPath, PATTERN_PATH_LEN, "parttern/%03d.io",
             currentPattern);
    if (sdAvailable && SD.exists(patternPath)) {
        File file = SD.open(patternPath);
        if (file) {
            String name = file.readStringUntil('\n');
            pattern.setName(name.c_str());
            while (file.available()) {
                Serial.write(file.read());
            }
            file.close();
            return;
        }
    }
    pattern.setDefaultName();
}

void setCurrentPattern(int8_t direction) {
    currentPattern += direction;
    loadPattern();
}

#endif
