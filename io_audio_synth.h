#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "AudioSynthWaveTableSD.h"
#include "arbitraryWaveform.h"
#include "audio_dumb.h"
#include "io_util.h"

// need to make a PR to get waveform count
#define WAVEFORM_COUNT 9

#define FILTER_TYPE_COUNT 3
#define AUDIO_SYNTH_MOD 3
#define WAVETABLE_COUNT 255 - WAVEFORM_COUNT
#define WAVETABLE_NAME_LEN 20
#define WAVETABLE_FOLDER "raw/"

class IO_AudioSynth : public AudioDumb {
   protected:
   public:
    byte rawWaveCount = 0;
    char wavetableName[WAVETABLE_COUNT][WAVETABLE_NAME_LEN];
    char wavetableFullPath[WAVETABLE_NAME_LEN + 10];

    AudioSynthWaveformDc dc;
    AudioEffectEnvelope envMod;
    AudioSynthWaveform lfoMod;
    AudioSynthWaveformModulated waveform;
    AudioEffectEnvelope env;
    AudioFilterStateVariable filter;
    AudioSynthWaveTableSD<> waveTable;

    byte currentWaveform = WAVEFORM_SINE;

    bool useAdsr = true;

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
        patchCordEnvModToWave = new AudioConnection(envMod, waveTable);

        patchCordLfoToWave = new AudioConnection(lfoMod, waveTable);

        patchCordWaveToFilter = new AudioConnection(waveTable, filter);

        patchCordWaveToEnv = new AudioConnection(waveTable, env);
        patchCordEnvToFilter = new AudioConnection(env, filter);

        patchCordFilter[0] = new AudioConnection(filter, 0, *this, 0);
        patchCordFilter[1] = new AudioConnection(filter, 1, *this, 0);
        patchCordFilter[2] = new AudioConnection(filter, 2, *this, 0);

        applyFilterCord();
        applyModulationCord();

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

        waveTable.amplitude(amplitude)->setStart(4000)->setEnd(5000)->frequency(
            frequency);
    }

    void init() {
        rawWaveCount = 0;

        File root = SD.open(WAVETABLE_FOLDER);
        if (root) {
            while (true) {
                File entry = root.openNextFile();
                if (!entry) {
                    break;
                }
                if (!entry.isDirectory()) {
                    snprintf(wavetableName[rawWaveCount], WAVETABLE_NAME_LEN,
                             entry.name());
                    rawWaveCount++;
                }
                entry.close();
            }
            root.close();
        }
    }

    void setNextWaveform(int8_t direction) {
        currentWaveform =
            mod(currentWaveform + direction, WAVEFORM_COUNT + rawWaveCount);
        if (currentWaveform < WAVEFORM_COUNT) {
            waveform.begin(currentWaveform);
        } else {
            sprintf(wavetableFullPath, "%s%s", WAVETABLE_FOLDER,
                    wavetableName[currentWaveform - WAVEFORM_COUNT]);
            waveTable.load(wavetableFullPath);
        }
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
        waveTable.reset();
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
