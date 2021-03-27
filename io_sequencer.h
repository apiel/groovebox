#ifndef IO_SEQUENCER_H_
#define IO_SEQUENCER_H_

#include <Arduino.h>
#include <Metro.h>

#include "Pattern.h"
#include "Sequence.h"
#include "io_audio.h"
#include "io_midi_core.h"
#include "io_sequence.h"
#include "io_util.h"

// 4 ♬ ♬ step per beat
#define STEP_PER_BEAT 4

Sequence sequences[SEQUENCE_COUNT];

byte bpm = 100;
unsigned int tempo = 150;
Metro timer = Metro(tempo);

void setTempo(int8_t direction) {
    bpm = constrain(bpm + direction, 10, 255);
    // 4 ♬ ♬ step per beat in 60000ms = 1min
    tempo = 60000 / (bpm * STEP_PER_BEAT);
    timer.interval(tempo);
}

void sequencerNextHandler(byte type, byte output, byte note, byte velocity) {
    // Serial.printf("seq next: %s %d %d %d\n", type == SEQ_NOTE_ON ? "on" : "off",
    //               output, note, velocity);
    if (output > 0 && output <= 16) {
        if (type == SEQ_NOTE_ON) {
            noteOn(output, note, velocity);
        } else {
            noteOff(output, note, velocity);
        }
    } else if (output >= 17 && output <= 22) {
        if (type == SEQ_NOTE_ON) {
            synth[output - 17].noteOn(note, velocity);
        } else {
            synth[output - 17].noteOff();
        }
    } else if (output == 23) {
        if (type == SEQ_NOTE_ON) {
            wav[0].play(note);
        }
    } else if (output == 24) {
        if (type == SEQ_NOTE_ON) {
            wav[1].play(note);
        }
    }
}

void sequencerInit() {
    setTempo(0);
    for (byte pos = 0; pos < SEQUENCE_COUNT; pos++) {
        sequences[pos].setNextHandler(sequencerNextHandler);
    }
}

void sequencerLoop() {
    if (timer.check() == 1) {
        timer.reset();
        for (byte pos = 0; pos < SEQUENCE_COUNT; pos++) {
            sequences[pos].next();
        }
    }
}

#endif
