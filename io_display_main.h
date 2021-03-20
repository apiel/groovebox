#ifndef IO_DISPLAY_MAIN_H_
#define IO_DISPLAY_MAIN_H_

#include <Adafruit_SSD1306.h>

#include "io_sequencer.h"

void displayMain(Adafruit_SSD1306 * d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    d->printf("bmp %d", bpm);
}

#endif
