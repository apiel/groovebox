#ifndef IO_MIDI_PATTERN_H_
#define IO_MIDI_PATTERN_H_

#include <Arduino.h>

#include "io_pattern.h"

void patternNoteOnHandler(byte channel, byte note,
                        byte velocity) {
    if (channel == 11) {
    }
}

void patternNoteOffHandler(byte channel, byte note,
                         byte velocity) {
    if (channel == 11) {
    }
}

void patternControlChangeHandler(byte channel, byte knob,
                               int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            setCurrentPattern(direction);
        }
    }
}

#endif
