#ifndef IO_SEQUENCE_H_
#define IO_SEQUENCE_H_

#include <Metro.h>

#include "Pattern.h"
#include "Sequence.h"
#include "io_util.h"

#define SEQUENCE_COUNT 32         // should this be define by screen size?
#define SEQUENCE_OUTPUT_COUNT 16  // + SYNTH_COUNT + ...
#define SEQUENCE_ROW_COUNT 4      // should this be define by screen size?
#define SEQUENCE_PER_ROW 8        // should this be define by screen size?
// 4 ♬ ♬ step per beat
#define STEP_PER_BEAT 4

Sequence sequences[SEQUENCE_COUNT];
byte currentSequence = 0;
byte sequenceOutput = 0;
byte currentSeqRow = 0;

byte bpm = 100;
unsigned int tempo = 150;
Metro timer = Metro(tempo);

void setTempo(int8_t direction) {
    bpm = constrain(bpm + direction, 10, 255);
    // 4 ♬ ♬ step per beat in 60000ms = 1min
    tempo = 60000 / (bpm * STEP_PER_BEAT);
    timer.interval(tempo);
}

void setCurrentSequence(int8_t direction) {
    currentSequence = mod(currentSequence + direction, SEQUENCE_COUNT);
}

void setSequenceOutput(int8_t direction) {
    sequenceOutput = mod(sequenceOutput + direction, SEQUENCE_OUTPUT_COUNT);
}

void setSequenceRow(int8_t direction) {
    currentSeqRow = mod(currentSeqRow + direction, SEQUENCE_ROW_COUNT);
}

void sequencerInit() { setTempo(0); }

void sequencerLoop() {
    if (timer.check() == 1) {
        timer.reset();
        for(byte pos = 0; pos < SEQUENCE_COUNT; pos++) {
            sequences[pos].next();
        }
    }
}

#endif
