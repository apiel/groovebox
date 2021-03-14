#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "arbitraryWaveform.h"
#include "io_util.h"

#define WAVEFORM_COUNT 9

class IO_AudioSynth {
   protected:
    AudioConnection patchCord01;
    AudioConnection patchCord02;
    AudioConnection patchCord03;
    AudioConnection patchCord05;
    AudioConnection patchCord06;
    AudioConnection patchCordFilterOutLowPass;
    AudioConnection patchCordFilterOutBandPass;
    AudioConnection patchCordFilterOutHighPass;

   public:
    AudioSynthWaveformDc dc;
    AudioEffectEnvelope envMod;
    AudioSynthWaveformModulated lfoMod;
    AudioSynthWaveformModulated waveform;
    AudioEffectEnvelope env;
    AudioFilterStateVariable filter;

    byte currentWaveform = 0;

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

    IO_AudioSynth(AudioStream* audioDest)
        : patchCord01(lfoMod, waveform),
          patchCord02(dc, envMod),
          patchCord03(envMod, 0, waveform, 1),
          patchCord05(waveform, env),
          //   patchCord06(env, filter) {
          patchCord06(env, filter),
          patchCordFilterOutLowPass(filter, 0, *audioDest, 0),
          patchCordFilterOutBandPass(filter, 1, *audioDest, 0),
          patchCordFilterOutHighPass(filter, 2, *audioDest, 0) {
        waveform.frequency(frequency);
        waveform.amplitude(amplitude);
        waveform.arbitraryWaveform(arbitraryWaveform, 172.0);
        waveform.begin(WAVEFORM_SINE);

        lfoMod.frequency(1.0);
        // lfoMod.amplitude(0.5);
        lfoMod.amplitude(0.0);
        lfoMod.begin(WAVEFORM_SINE);

        env.attack(attackMs);
        env.decay(decayMs);
        env.sustain(sustainLevel);
        env.release(releaseMs);
        env.hold(0);
        env.delay(0);

        dc.amplitude(0.5);
        envMod.delay(0);
        envMod.attack(200);
        envMod.hold(200);
        envMod.decay(200);
        envMod.sustain(0.4);
        envMod.release(1500);

        setCurrentFilter(0);
        filter.frequency(filterFrequency);
        filter.resonance(filterResonance);
        filter.octaveControl(filterOctaveControl);
    }

    void setCurrentFilter(int8_t direction) {
        currentFilter = mod(currentFilter + direction, 3);
        // patchCordFilterOutLowPass.disconnect();
        // patchCordFilterOutBandPass.disconnect();
        // patchCordFilterOutHighPass.disconnect();
        // switch (currentFilter) {
        //     case 1:
        //         // Serial.println("BandPass");
        //         patchCordFilterOutBandPass.connect();
        //         break;

        //     case 2:
        //         // Serial.println("HighPass");
        //         patchCordFilterOutHighPass.connect();
        //         break;

        //     default:
        //         // Serial.println("LowPass");
        //         patchCordFilterOutLowPass.connect();
        //         break;
        // }
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

    void setFrequency(int8_t direction) {
        frequency =
            constrain(frequency + direction, 0, AUDIO_SAMPLE_RATE_EXACT / 2);
        waveform.frequency(frequency);
    }

    void setAmplitude(int8_t direction) {
        amplitude = pctAdd(amplitude, direction);
        waveform.amplitude(amplitude);
    }

    void noteOn() {
        Serial.println("note on");
        envMod.noteOn();
        lfoMod.phaseModulation(0);
        env.noteOn();
    }

    void noteOff() {
        Serial.println("note off");
        env.noteOff();
        envMod.noteOff();
    }
};

#endif
