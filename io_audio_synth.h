#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "audio_dumb.h"
#include "io_audio_synth_wave.h"
#include "io_util.h"

#define FILTER_TYPE_COUNT 3
#define AUDIO_SYNTH_MOD 3

class IO_AudioSynth : public AudioDumb {
   protected:
   public:
    AudioSynthWaveformDc dc;
    AudioEffectEnvelope envMod;
    AudioSynthWaveform lfoMod;
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
    float filterResonance = 0.707;
    byte currentFilter = 0;

    byte modulation = 0;
    float modAttackMs = 100.0;
    float modDecayMs = 50.0;
    float modSustainLevel = 70.0;
    float modReleaseMs = 50.0;

    float lfoFrequency = 1.0;
    float lfoAmplitude = 0.5;
    byte lfoWave = WAVEFORM_SINE;

    AudioConnection* patchCordFilter[FILTER_TYPE_COUNT];
    AudioConnection* patchCordEnvToFilter;
    AudioConnection* patchCordWaveToFilter;
    AudioConnection* patchCordWaveToEnv;
    AudioConnection* patchCordLfoToWave;
    AudioConnection* patchCordDcToEnvMod;
    AudioConnection* patchCordEnvModToWave;

    // waveform knob will be used to select as well SD raw file
    // if over WAVEFORM_COUNT then go raw dir

    IO_AudioSynth() {
        patchCordDcToEnvMod = new AudioConnection(dc, envMod);
        patchCordEnvModToWave = new AudioConnection(envMod, wave.input);

        patchCordLfoToWave = new AudioConnection(lfoMod, wave.input);

        patchCordWaveToFilter = new AudioConnection(wave, filter);

        patchCordWaveToEnv = new AudioConnection(wave, env);
        patchCordEnvToFilter = new AudioConnection(env, filter);

        patchCordFilter[0] = new AudioConnection(filter, 0, *this, 0);
        patchCordFilter[1] = new AudioConnection(filter, 1, *this, 0);
        patchCordFilter[2] = new AudioConnection(filter, 2, *this, 0);

        applyFilterCord();
        applyModulationCord();

        env.attack(attackMs);
        env.decay(decayMs);
        env.sustain(sustainLevel);
        env.release(releaseMs);
        env.hold(0);
        env.delay(0);

        lfoMod.frequency(lfoFrequency);
        lfoMod.amplitude(lfoAmplitude);
        lfoMod.begin(lfoWave);

        dc.amplitude(0.5);
        envMod.delay(0);
        envMod.attack(modAttackMs);
        envMod.hold(0);
        envMod.decay(modDecayMs);
        envMod.sustain(modSustainLevel);
        envMod.release(modReleaseMs);

        setCurrentFilter(0);
        filter.frequency(filterFrequency);
        filter.resonance(filterResonance);
        filter.octaveControl(filterOctaveControl);
    }

    void init() { wave.init(); }

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

    void setModulation(int8_t direction) {
        modulation = mod(modulation + direction, AUDIO_SYNTH_MOD);
        applyModulationCord();
    }

    void applyModulationCord() {
        patchCordLfoToWave->disconnect();
        patchCordDcToEnvMod->disconnect();
        patchCordEnvModToWave->disconnect();
        if (modulation == 1) {
            patchCordDcToEnvMod->connect();
            patchCordEnvModToWave->connect();
        } else if (modulation == 2) {
            patchCordLfoToWave->connect();
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

    void setModAttack(int8_t direction) {
        modAttackMs = constrain(modAttackMs + direction, 0, 11880);
        env.attack(modAttackMs);
    }

    void setModDecay(int8_t direction) {
        modDecayMs = constrain(modDecayMs + direction, 0, 11880);
        env.decay(modDecayMs);
    }

    void setModRelease(int8_t direction) {
        modReleaseMs = constrain(modReleaseMs + direction, 0, 11880);
        env.release(modReleaseMs);
    }

    void setModSustain(int8_t direction) {
        modSustainLevel = pctAdd(modSustainLevel, direction);
        env.release(modSustainLevel);
    }

    void noteOn() {
        wave.waveTable.reset();
        lfoMod.phase(0.0);
        envMod.noteOn();
        env.noteOn();
    }

    void noteOff() {
        env.noteOff();
        envMod.noteOff();
    }
};

#endif
