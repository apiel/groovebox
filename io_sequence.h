#ifndef IO_SEQUENCE_H_
#define IO_SEQUENCE_H_

#include <Arduino.h>
#include <Metro.h>

#include "Pattern.h"
#include "Sequence.h"
#include "io_audio.h"
#include "io_util.h"

#define MIDI_CHANNEL_COUNT 16
#define SEQUENCE_COUNT 32  // should this be define by screen size?
#define SEQUENCE_OUTPUT_COUNT \
    1 + MIDI_CHANNEL_COUNT +  \
        SYNTH_COUNT           // 0 + 16ch // need + SYNTH_COUNT + ...
#define SEQUENCE_ROW_COUNT 4  // should this be define by screen size?
#define SEQUENCE_PER_ROW 8    // should this be define by screen size?

byte currentSequence = 0;
byte sequenceOutput = 0;
byte currentSeqRow = 0;

void setCurrentSequence(int8_t direction) {
    currentSequence = mod(currentSequence + direction, SEQUENCE_COUNT);
}

void setSequenceOutput(int8_t direction) {
    sequenceOutput = mod(sequenceOutput + direction, SEQUENCE_OUTPUT_COUNT);
}

void setSequenceRow(int8_t direction) {
    currentSeqRow = mod(currentSeqRow + direction, SEQUENCE_ROW_COUNT);
}

bool isSynthOutput(byte output) { return output > 0 && output <= SYNTH_COUNT; }

bool isMidiOutput(byte output) {
    return output > SYNTH_COUNT && output <= SYNTH_COUNT + MIDI_CHANNEL_COUNT;
}
byte getMidiChannel(byte output) {
    // midi channel start at 1
    return output - SYNTH_COUNT;
}

byte getSynthChannel(byte output) {
    // synth channel start at 0
    return output - 1;
}

#endif
