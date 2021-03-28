#ifndef IO_SEQUENCER_H_
#define IO_SEQUENCER_H_

#include <Arduino.h>
#include <Metro.h>

#include "Pattern.h"
#include "Sequence.h"
#include "io_audio.h"
#include "io_midi_core.h"
#include "io_pattern_storage.h"
#include "io_sequence.h"
#include "io_state.h"
#include "io_storage.h"
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
    // Serial.printf("seq next: %s %d %d %d\n", type == SEQ_NOTE_ON ? "on" :
    // "off", output, note, velocity);
    if (isSynthOutput(output)) {
        if (type == SEQ_NOTE_ON) {
            synth[getSynthChannel(output)].noteOn(note, velocity);
        } else {
            synth[getSynthChannel(output)].noteOff();
        }
    } else if (isMidiOutput(output)) {
        if (type == SEQ_NOTE_ON) {
            noteOn(getMidiChannel(output), note, velocity);
        } else {
            noteOff(getMidiChannel(output), note, velocity);
        }
    }
}

void sequencerLoad() {
    Serial.println("sequencerLoad.");
    if (sdAvailable && SD.exists("sequencer/00.io")) {
        File file = SD.open("sequencer/00.io");
        if (file) {
            while (file.available() && assignStorageValues(&file)) {
                Pattern* ptrPattern = &patterns[(byte)storageValues[1]];
                if (ptrPattern) {
                    byte seqPos = (byte)storageValues[0];
                    sequences[seqPos].set(ptrPattern, (byte)storageValues[2]);
                }
            }
            file.close();
        }
    }
}

void sequencerInit() {
    setTempo(0);
    for (byte pos = 0; pos < SEQUENCE_COUNT; pos++) {
        sequences[pos].setNextHandler(sequencerNextHandler);
    }
    sequencerLoad();
}

void sequencerLoop() {
    if (timer.check() == 1) {
        timer.reset();
        for (byte pos = 0; pos < SEQUENCE_COUNT; pos++) {
            sequences[pos].next();
        }
    }
}

bool sequencerSave() {
    Serial.println("sequencerSave");
    if (sdAvailable) {
        File file = SD.open("sequencer/00.io", FILE_WRITE);
        if (file) {
            file.seek(0);
            for (byte pos = 0; pos < SEQUENCE_COUNT; pos++) {
                sprintf(storageBuffer, "%d %d %d %d\n", (int)pos,
                        (int)sequences[pos].pattern.pos,
                        (int)sequences[pos].output,
                        (int)sequences[pos].active ? 1 : 0);
                Serial.print(storageBuffer);
                file.print(storageBuffer);
            }
            file.close();
            return true;
        }
    }
    Serial.println("- failed to open file for writing");
    return false;
}

#endif
