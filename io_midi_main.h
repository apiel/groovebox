#ifndef IO_MIDI_MAIN_H_
#define IO_MIDI_MAIN_H_

#include <Arduino.h>

#include "io_midi_util.h"
#include "io_sequencer.h"

void mainNoteOnHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
    }
}

void mainNoteOffHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
    }
}

void mainControlChangeHandler(byte channel, byte knob, int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            setTempo(direction);
        }
    }
}

#endif
