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
    byte pos = 0;
    for (byte y = 0; y < SCREEN_H; y += 16) {
        for (byte x = 0; x < SCREEN_W; x += 16) {
            dprintxyAbs(d, x + 2, y + 6, "%02d", pos + 1);
            if (sequences[pos].output) {
                dprintxyAbs(d, x + 7, y + 15, "%02d", sequences[pos].output);
                if (sequences[pos].active) {
                    d->fillTriangle(x + 2, y + 8, x + 5, y + 11, x + 2, y + 14,
                                    WHITE);
                } else {
                    d->fillRect(x + 2, y + 8, 4, 4, WHITE);
                }
            }
            pos++;
        }
    }
    resetFont(d);

    d->fillRect(SCREEN_W - 2, currentSeqRow * 16, 2, 16, WHITE);
}

#endif
