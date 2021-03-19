#ifndef IO_DISPLAY_SEQUENCES_H_
#define IO_DISPLAY_SEQUENCES_H_

#include <Adafruit_SSD1306.h>

#include "io_display_util.h"
#include "io_sequence.h"

void displaySequences(Adafruit_SSD1306* d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    for (byte x = 16; x < SCREEN_W; x += 16) {
        d->drawLine(x, 0, x, SCREEN_H, WHITE);
    }
    for (byte y = 16; y < SCREEN_H; y += 16) {
        d->drawLine(0, y, SCREEN_W, y, WHITE);
    }

    setSmallFont(d);
    byte pos = 1;
    for (byte y = 0; y < SCREEN_H; y += 16) {
        for (byte x = 0; x < SCREEN_W; x += 16) {
            dprintxyAbs(d, x + 2, y + 6, "%02d", pos);
            if (sequences[pos].isPlaying()) {
                d->fillTriangle(x + 4, y + 8, x + 10, y + 11, x + 4, y + 14,
                                WHITE);
            } else {
            }
            pos++;
        }
    }
    resetFont(d);
}

#endif
