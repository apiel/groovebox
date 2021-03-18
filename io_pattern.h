#ifndef IO_PATTERN_H_
#define IO_PATTERN_H_

#include <SD.h>

#include "Pattern.h"
#include "io_state.h"
#include "io_state_pattern.h"

#define PATTERN_PATH_LEN 16

Pattern pattern;
char patternPath[PATTERN_PATH_LEN];

void loadPattern() {
    pattern.clear();

    snprintf(patternPath, PATTERN_PATH_LEN, "/parttern/%03d.io",
             currentPattern);
    if (sdAvailable && SD.exists(patternPath)) {

    } else {
        pattern.setDefaultName();
    }
}

#endif
