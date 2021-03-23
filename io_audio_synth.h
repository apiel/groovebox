#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "AudioSynthWaveTableSD.h"
#include "arbitraryWaveform.h"
#include "audio_dumb.h"
#include "io_util.h"

#define WAVEFORM_COUNT 9
#define FILTER_TYPE_COUNT 3

class IO_AudioSynth : public AudioDumb {
   protected:
   public:
    AudioConnection* patchCord[5];
    AudioConnection* patchCordFilter[FILTER_TYPE_COUNT];

    AudioSynthWaveformDc dc;
    AudioEffectEnvelope envMod;
    AudioSynthWaveformModulated lfoMod;
    AudioSynthWaveformModulated waveform;
    AudioEffectEnvelope env;
    AudioFilterStateVariable filter;
    // AudioPlaySdRaw raw;
    AudioSynthWaveTableSD<> waveTable;

    byte currentWaveform = WAVEFORM_SINE;

    float attackMs = 0;
    float decayMs = 50;
    float sustainLevel = 0;
    float releaseMs = 0;

    float frequency = 440;
    float amplitude = 1.0;

    float filterFrequency = 1000;
    float filterOctaveControl = 1.0;
    float filterResonance = 0.707;
    byte currentFilter = 0;

    float modAttackMs = 0;
    float modDecayMs = 50;
    float modSustainLevel = 0;
    float modReleaseMs = 0;

    float lfoFrequency = 1.0;
    float lfoAmplitude = 0.0;
    byte lfoWave = WAVEFORM_SINE;

    IO_AudioSynth() {
        byte pci = 0;  // used only for adding new patchcords
        // patchCord[pci++] = new AudioConnection(lfoMod, waveform);
        // patchCord[pci++] = new AudioConnection(dc, envMod);
        // // this is wrong should be patchCord[pci++] = new
        // AudioConnection(envMod, waveform);
        // // and then have a way to select like for pass filter
        // patchCord[pci++] = new AudioConnection(envMod, 0, waveform, 1);
        // patchCord[pci++] = new AudioConnection(waveform, env);

        // patchCord[pci++] = new AudioConnection(waveTable, env);
        // patchCord[pci++] = new AudioConnection(env, *this);
        patchCord[pci++] = new AudioConnection(waveTable, *this);

        // patchCord[pci++] = new AudioConnection(env, filter);
        // // patchCord[pci++] = new AudioConnection(env, *this);
        // patchCordFilter[0] = new AudioConnection(filter, 0, *this, 0);
        // patchCordFilter[1] = new AudioConnection(filter, 1, *this, 0);
        // patchCordFilter[2] = new AudioConnection(filter, 2, *this, 0);

        // patchCord[pci++] = new AudioConnection(raw, *this);

        waveform.frequency(frequency);
        waveform.amplitude(amplitude);
        waveform.arbitraryWaveform(arbitraryWaveform, 172.0);
        waveform.begin(currentWaveform);

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

        waveTable.load("raw/kick.raw");
        waveTable.amplitude(amplitude);
        waveTable.frequency(frequency);
    }

    void setCurrentFilter(int8_t direction) {
        currentFilter = mod(currentFilter + direction, FILTER_TYPE_COUNT);
        // might not need to diconnect, as only the last connected is the one
        // used see https://www.pjrc.com/teensy/td_libs_AudioConnection.html
        // patchCordFilter[0]->disconnect();
        // patchCordFilter[1]->disconnect();
        // patchCordFilter[2]->disconnect();
        // patchCordFilter[currentFilter]->connect();
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

    // need to make a PR to get waveform count
    void setNextWaveform(int8_t direction) {
        currentWaveform = mod(currentWaveform + direction, WAVEFORM_COUNT);
        waveform.begin(currentWaveform);
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
        env.release(sustainLevel);
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

    void setFrequency(int8_t direction) {
        frequency =
            constrain(frequency + direction, 0, AUDIO_SAMPLE_RATE_EXACT / 2);
        waveform.frequency(frequency);
        waveTable.frequency(frequency);
    }

    void setAmplitude(int8_t direction) {
        amplitude = pctAdd(amplitude, direction);
        waveform.amplitude(amplitude);
        waveTable.amplitude(amplitude);
    }

    void noteOn() {
        // to be remove, should go in setup
        waveTable.load("raw/kick.raw");

        envMod.noteOn();
        lfoMod.phaseModulation(0);
        env.noteOn();
        // raw.play("raw/kick.raw");
    }

    void noteOff() {
        env.noteOff();
        envMod.noteOff();
        // raw.stop();
    }
};

#endif
