#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "audio_dumb.h"
#include "io_audio_synth_modulation.h"
#include "io_audio_synth_wave.h"
#include "io_util.h"
#include "note.h"

#define FILTER_TYPE_COUNT 3
#define AUDIO_SYNTH_MOD 3

class IO_AudioSynth : public AudioDumb {
   protected:
   public:
    IO_AudioSynthModulation modulation;
    AudioEffectEnvelope env;
    AudioFilterStateVariable filter;
    IO_AudioSynthWave wave;

    bool useAdsr = true;

    float attackMs = 0;
    float decayMs = 50;
    float sustainLevel = 0;
    float releaseMs = 0;

    float filterFrequency = 1000;
    float filterOctaveControl = 1.0;
    float filterResonance = 0.7;
    byte currentFilter = 0;

    AudioConnection* patchCordFilter[FILTER_TYPE_COUNT];
    AudioConnection* patchCordEnvToFilter;
    AudioConnection* patchCordWaveToFilter;
    AudioConnection* patchCordWaveToEnv;
    AudioConnection* patchCordMod;

    IO_AudioSynth() {
        patchCordMod = new AudioConnection(modulation, wave.input);
        patchCordWaveToFilter = new AudioConnection(wave, filter);
        patchCordWaveToEnv = new AudioConnection(wave, env);
        patchCordEnvToFilter = new AudioConnection(env, filter);
        patchCordFilter[0] = new AudioConnection(filter, 0, *this, 0);
        patchCordFilter[1] = new AudioConnection(filter, 1, *this, 0);
        patchCordFilter[2] = new AudioConnection(filter, 2, *this, 0);

        applyFilterCord();

        env.attack(attackMs);
        env.decay(decayMs);
        env.sustain(sustainLevel);
        env.release(releaseMs);
        env.hold(0);
        env.delay(0);

        setCurrentFilter(0);
        filter.frequency(filterFrequency);
        filter.resonance(filterResonance);
        filter.octaveControl(filterOctaveControl);
    }

    void init() {
        wave.init();
        modulation.init();
    }

    void toggleAdsr() {
        useAdsr = !useAdsr;
        applyFilterCord();
    }

    void applyFilterCord() {
        if (useAdsr) {
            patchCordWaveToFilter->disconnect();
            patchCordWaveToEnv->connect();
            patchCordEnvToFilter->connect();
        } else {
            patchCordWaveToFilter->connect();
            patchCordEnvToFilter->disconnect();
            patchCordWaveToEnv->disconnect();
        }
    }

    void setCurrentFilter(int8_t direction) {
        currentFilter = mod(currentFilter + direction, FILTER_TYPE_COUNT);
        // as only the last connected is the one used
        // https://www.pjrc.com/teensy/td_libs_AudioConnection.html
        patchCordFilter[currentFilter]->connect();
    }

    void setFilterFrequency(int8_t direction) {
        filterFrequency = constrain(filterFrequency + direction, 0,
                                    AUDIO_SAMPLE_RATE_EXACT / 2);
        filter.frequency(filterFrequency);
    }

    void setFilterResonance(int8_t direction) {
        filterResonance =
            constrain(filterResonance + direction * 0.1, 0.7, 5.0);
        filter.resonance(filterResonance);
    }

    void setFilterOctaveControl(int8_t direction) {
        filterOctaveControl =
            constrain(filterOctaveControl + direction * 0.1, 0.0, 7.0);
        filter.octaveControl(filterOctaveControl);
    }

    void setAttack(int8_t direction) {
        attackMs = constrain(attackMs + direction, 0, 11880);
        env.attack(attackMs);
    }

    void setDecay(int8_t direction) {
        decayMs = constrain(decayMs + direction, 0, 11880);
        env.decay(decayMs);
    }

    void setRelease(int8_t direction) {
        releaseMs = constrain(releaseMs + direction, 0, 11880);
        env.release(releaseMs);
    }

    void setSustain(int8_t direction) {
        sustainLevel = pctAdd(sustainLevel, direction);
        env.sustain(sustainLevel);
    }

    void noteOn() { noteOn(_C4, 127); }

    void noteOn(byte note, byte velocity) {
        wave.waveform.amplitude(wave.amplitude * velocity / 127);
        wave.waveform.frequency(wave.frequency + NOTE_FREQ[_C4] -
                                NOTE_FREQ[note]);
        wave.waveTable.reset()
            ->amplitude(wave.amplitude * velocity / 127)
            ->frequency(wave.frequency + NOTE_FREQ[_C4] - NOTE_FREQ[note]);
        modulation.lfoMod.phase(0.0);
        modulation.envMod.noteOn();
        env.noteOn();
    }

    void noteOff() {
        env.noteOff();
        modulation.envMod.noteOff();
    }
};

#endif
