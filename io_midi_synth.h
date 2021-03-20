#ifndef IO_MIDI_SYNTH_H_
#define IO_MIDI_SYNTH_H_

#include <Arduino.h>

#include "io_audio.h"

void synthNoteOnHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
        if (note == 22) {
            synth[currentSynth].noteOn();
        }
    }
}

void synthNoteOffHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
        if (note == 22) {
            synth[currentSynth].noteOff();
        }
    }
}

void synthControlChangeHandler(byte channel, byte knob, int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            synth[currentSynth].setNextWaveform(direction);
        } else if (knob == 2) {
            synth[currentSynth].setFrequency(direction);
        } else if (knob == 3) {
            synth[currentSynth].setAmplitude(direction);
        } else if (knob == 5) {
            synth[currentSynth].setAttack(direction);
        } else if (knob == 6) {
            synth[currentSynth].setDecay(direction);
        } else if (knob == 7) {
            synth[currentSynth].setSustain(direction);
        } else if (knob == 8) {
            synth[currentSynth].setRelease(direction);
        } else if (knob == 11) {
            synth[currentSynth].setFilterFrequency(direction);
        } else if (knob == 12) {
            synth[currentSynth].setFilterResonance(direction);
        } else if (knob == 13) {
            synth[currentSynth].setFilterOctaveControl(direction);
        } else if (knob == 14) {
            synth[currentSynth].setCurrentFilter(direction);
        }
    }
}

#endif
