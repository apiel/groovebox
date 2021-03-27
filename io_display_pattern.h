#ifndef IO_DISPLAY_PATTERN_H_
#define IO_DISPLAY_PATTERN_H_

#include <Adafruit_SSD1306.h>

#include "io_config.h"
#include "io_display_util.h"
#include "io_pattern.h"
#include "io_sequence.h"

void displayStep(Adafruit_SSD1306* d, Pattern* pPattern, byte pos) {
    byte topMargin = 18;
    byte x = pos % 8;
    byte y = pos / 8 + 1;

    Step* step = &pPattern->steps[pos];

    d->drawLine(x * 16 + 1, y * 7 + 7 + topMargin, x * 16 + 12,
                y * 7 + 7 + topMargin, WHITE);
    if (step->duration) {
        d->setCursor(x * 16 + 2, y * 7 + 5 + topMargin);
        d->printf("%s%d", getNoteDot(step->note), getNoteOctave(step->note));

        // this show the duration but will be buggy when note go on the next
        // line need to think about a solution
        for (byte duration = 1; duration < step->duration; duration++) {
            d->drawLine(duration * (x + 1) * 16 - 4, y * 7 + 6 + topMargin,
                        duration * (x + 1) * 16 - 4 + 5, y * 7 + 6 + topMargin,
                        WHITE);
        }
    }

    if (currentStepSelection == pos) {
        d->drawLine(x * 16 + 2, y * 7 + 6 + topMargin, x * 16 + 10,
                    y * 7 + 6 + topMargin, WHITE);
    }
}

void displayPattern(Adafruit_SSD1306* d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    d->printf("%03d %s\n", currentPattern, pattern.name);

    Step* step = &pattern.steps[currentStepSelection];
    d->printf("\nD: %d V: %03d S: %d\n", step->duration, step->velocity,
              step->slide ? 1 : 0);

    setSmallFont(d);
    for (byte pos = 0; pos < STEP_COUNT; pos++) {
        displayStep(d, &pattern, pos);
    }
    resetFont(d);

    if (sequenceOutput == 0) {
        dprintxy(d, 0, 7, "Pos: %d Out: none", currentSequence + 1);
    } else {
        bool isMidi = isMidiOutput(sequenceOutput);
        dprintxy(d, 0, 7, "Pos: %d Out: %s %d", currentSequence + 1,
                 isMidi ? "midi" : "synth",
                 isMidi ? getMidiChannel(sequenceOutput)
                        : getSynthChannel(sequenceOutput));
    }
}

#endif
