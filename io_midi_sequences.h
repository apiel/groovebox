#ifndef IO_MIDI_SEQUENCES_H_
#define IO_MIDI_SEQUENCES_H_

#include <Arduino.h>

#include "io_midi_util.h"
#include "io_sequence.h"

void sequencesNoteOnHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
        byte pos = getItemKey(note);
        if (pos < 255) {
            pos += currentSeqRow * SEQUENCE_PER_ROW;
            sequences[pos].toggle();
        }
    }
}

void sequencesNoteOffHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
    }
}

void sequencesControlChangeHandler(byte channel, byte knob, int8_t direction) {
    if (channel == 11) {
        if (knob == 8) {
            setSequenceRow(direction);
        }
    }
}

#endif
