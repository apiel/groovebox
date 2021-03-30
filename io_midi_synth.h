#ifndef IO_MIDI_SYNTH_H_
#define IO_MIDI_SYNTH_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_audio_synth_storage.h"
#include "io_midi_util.h"

void synthNoteOnHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
        byte key = getItemKey(note);
        if (key != 255) {
            currentSynth = key;
        } else if (note == 22 || note == 46) {
            synth[currentSynth].noteOn();
        } else if (note == 23 || note == 47) {
            saveSynth(currentSynth);
        } else if (note == 20) {
            synth[currentSynth].toggleAdsr();
        }
    } else if (channel == 1) {
        Serial.println("should play note");
        synth[currentSynth].noteOn(note, velocity);
    }
}

void synthNoteOffHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
        if (note == 22 || note == 46) {
            synth[currentSynth].noteOff();
        }
    } else if (channel == 1) {
        synth[currentSynth].noteOff(note);
    }
}

void synthControlChangeHandler(byte channel, byte knob, int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            synth[currentSynth].wave.setNextWaveform(direction);
        } else if (knob == 2) {
            synth[currentSynth].wave.setFrequency(direction);
        } else if (knob == 3) {
            synth[currentSynth].wave.setAmplitude(direction);
        } else if (knob == 4) {
            synth[currentSynth].modulation.setModulation(direction);
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
        } else if (knob == 15) {
            if (synth[currentSynth].modulation.currentModulation == MOD_ENV) {
                synth[currentSynth].modulation.setModAttack(direction);
            }
        } else if (knob == 16) {
            if (synth[currentSynth].modulation.currentModulation == MOD_ENV) {
                synth[currentSynth].modulation.setModDecay(direction);
            } else if (synth[currentSynth].modulation.currentModulation ==
                       MOD_LFO) {
                synth[currentSynth].modulation.setFrequency(direction);
            }
        } else if (knob == 17) {
            if (synth[currentSynth].modulation.currentModulation == MOD_ENV) {
                synth[currentSynth].modulation.setModSustain(direction);
            } else if (synth[currentSynth].modulation.currentModulation ==
                       MOD_LFO) {
                synth[currentSynth].modulation.setAmplitude(direction);
            }
        } else if (knob == 0) {  // 0 for 18
            if (synth[currentSynth].modulation.currentModulation == MOD_ENV) {
                synth[currentSynth].modulation.setModRelease(direction);
            } else if (synth[currentSynth].modulation.currentModulation ==
                       MOD_LFO) {
                synth[currentSynth].modulation.setNextWaveform(direction);
            }
        }
    }
}

#endif
