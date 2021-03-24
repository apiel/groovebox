#ifndef IO_AUDIO_SYNTH_MODULATION_H_
#define IO_AUDIO_SYNTH_MODULATION_H_

#include <Arduino.h>
#include <Audio.h>

#include "audio_dumb.h"
#include "io_util.h"

#define AUDIO_SYNTH_MOD 3

class IO_AudioSynthModulation : public AudioDumb {
   protected:
   public:
    AudioSynthWaveformDc dc;
    AudioEffectEnvelope envMod;
    AudioSynthWaveform lfoMod;

    byte modulation = 0;
    float modAttackMs = 100.0;
    float modDecayMs = 50.0;
    float modSustainLevel = 70.0;
    float modReleaseMs = 50.0;

    float lfoFrequency = 1.0;
    float lfoAmplitude = 0.5;
    byte lfoWave = WAVEFORM_SINE;

    AudioConnection* patchCordLfoToWave;
    AudioConnection* patchCordDcToEnvMod;
    AudioConnection* patchCordEnvModToWave;

    IO_AudioSynthModulation() {
        patchCordDcToEnvMod = new AudioConnection(dc, envMod);
        patchCordEnvModToWave = new AudioConnection(envMod, *this);
        patchCordLfoToWave = new AudioConnection(lfoMod, *this);

        applyCord();

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
    }

    void init() { applyCord(); }

    void setModulation(int8_t direction) {
        modulation = mod(modulation + direction, AUDIO_SYNTH_MOD);
        applyCord();
    }

    void applyCord() {
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

    void setModAttack(int8_t direction) {
        modAttackMs = constrain(modAttackMs + direction, 0, 11880);
        envMod.attack(modAttackMs);
    }

    void setModDecay(int8_t direction) {
        modDecayMs = constrain(modDecayMs + direction, 0, 11880);
        envMod.decay(modDecayMs);
    }

    void setModRelease(int8_t direction) {
        modReleaseMs = constrain(modReleaseMs + direction, 0, 11880);
        envMod.release(modReleaseMs);
    }

    void setModSustain(int8_t direction) {
        modSustainLevel = pctAdd(modSustainLevel, direction);
        envMod.sustain(modSustainLevel);
    }
};

#endif
