#ifndef IO_MIDI_PATTERN_H_
#define IO_MIDI_PATTERN_H_

#include <Arduino.h>

#include "io_pattern.h"

void patternNoteOnHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
        if (note == 22 || note == 46) {
            setSequence();
        } else if (note == 23 || note == 47) {
            savePattern();
        }
    }
}

void patternNoteOffHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
    }
}

void patternControlChangeHandler(byte channel, byte knob, int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            setCurrentPattern(direction);
        } else if (knob == 2) {
            setCurrentStepSelection(direction);
        } else if (knob == 3) {
            setStepDuration(direction);
        } else if (knob == 4) {
            setStepNote(direction);
        } else if (knob == 5) {
            setStepVelocity(direction);
        }
    }
}

#endif
