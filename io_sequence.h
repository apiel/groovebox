#ifndef IO_SEQUENCE_H_
#define IO_SEQUENCE_H_

#include "Pattern.h"
#include "Sequence.h"
#include "io_util.h"

#define SEQUENCE_COUNT 32 // should this be define by screen size?
#define SEQUENCE_OUTPUT_COUNT 16 // + SYNTH_COUNT + ...
#define SEQUENCE_ROW_COUNT 4 // should this be define by screen size?
#define SEQUENCE_PER_ROW 8 // should this be define by screen size?

Sequence sequences[SEQUENCE_COUNT];
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

#endif
