#ifndef IO_DISPLAY_PATTERN_H_
#define IO_DISPLAY_PATTERN_H_

#include <Adafruit_SSD1306.h>

void displayPattern(Adafruit_SSD1306 * d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    d->println("Pattern");
}

#endif
