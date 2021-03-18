#ifndef IO_DISPLAY_PATTERN_H_
#define IO_DISPLAY_PATTERN_H_

#include <Adafruit_SSD1306.h>

#include "io_display_util.h"
#include "io_pattern.h"

void displayPattern(Adafruit_SSD1306 * d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    dprintln(d, "%03d %s", currentPattern, pattern.name);
}

#endif
