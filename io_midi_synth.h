#ifndef IO_MIDI_SYNTH_H_
#define IO_MIDI_SYNTH_H_

#include <Arduino.h>

#include "io_audio.h"

void synthNoteOnHandler(IO_Audio* audio, byte channel, byte note,
                        byte velocity) {
    if (channel == 11) {
        if (note == 22) {
            audio->synth.noteOn();
        }
    }
}

void synthNoteOffHandler(IO_Audio* audio, byte channel, byte note,
                         byte velocity) {
    if (channel == 11) {
        if (note == 22) {
            audio->synth.noteOff();
        }
    }
}

void synthControlChangeHandler(IO_Audio* audio, byte channel, byte knob,
                               int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            audio->synth.setNextWaveform(direction);
        } else if (knob == 2) {
            audio->synth.setFrequency(direction);
        } else if (knob == 3) {
            audio->synth.setAmplitude(direction);
        } else if (knob == 5) {
            audio->synth.setAttack(direction);
        } else if (knob == 6) {
            audio->synth.setDecay(direction);
        } else if (knob == 7) {
            audio->synth.setSustain(direction);
        } else if (knob == 8) {
            audio->synth.setRelease(direction);
        } else if (knob == 11) {
            audio->synth.setFilterFrequency(direction);
        } else if (knob == 12) {
            audio->synth.setFilterResonance(direction);
        } else if (knob == 13) {
            audio->synth.setFilterOctaveControl(direction);
        } else if (knob == 14) {
            audio->synth.setCurrentFilter(direction);
        }
    }
}

#endif
