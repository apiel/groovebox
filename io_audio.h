#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>
#include <Metro.h>

#include "arbitraryWaveform.h"
#include "io_util.h"

#define WAVEFORM_COUNT 9

AudioSynthWaveform waveform;
AudioEffectEnvelope env;
AudioOutputMQS audioOut;

AudioConnection patchCord01(waveform, env);
AudioConnection patchCord02(env, 0, audioOut, 0);
Metro timerHold = Metro(150);
Metro timerNote = Metro(1000);

bool io_playing = true;
byte currentWaveform = 0;

float attackMs = 0;  // 0 - 11880
float decayMs = 50;
float sustainLevel = 0;  // 0 - 1.0
float releaseMs = 0;

float frequency = 440;
float amplitude = 1.0;

void audioPlay(bool playing) { io_playing = playing; }
bool audioIsPlaying() { return io_playing; }

// need to make a PR to get waveform count
void setNextWaveform(int8_t direction) {
    currentWaveform = mod(currentWaveform + direction, WAVEFORM_COUNT);
    waveform.begin(currentWaveform);
}

void setAttack(int8_t direction) {
    attackMs = between(attackMs + direction, 0, 11880);
    env.attack(attackMs);
}

void setDecay(int8_t direction) {
    decayMs = between(decayMs + direction, 0, 11880);
    env.decay(decayMs);
}

void setRelease(int8_t direction) {
    releaseMs = between(releaseMs + direction, 0, 11880);
    env.release(releaseMs);
}

void setSustain(int8_t direction) {
    sustainLevel = pctAdd(sustainLevel, direction);
    env.release(sustainLevel);
}

void setFrequency(int8_t direction) {
    frequency = between(frequency + direction, 0, AUDIO_SAMPLE_RATE_EXACT / 2);
    waveform.frequency(frequency);
}

void setAmplitude(int8_t direction) {
    amplitude = pctAdd(amplitude, direction);
    waveform.amplitude(amplitude);
}

void audioInit() {
    AudioMemory(10);
    waveform.frequency(frequency);
    waveform.amplitude(amplitude);
    waveform.arbitraryWaveform(arbitraryWaveform, 172.0);
    waveform.begin(WAVEFORM_SINE);

    env.attack(attackMs);
    env.decay(decayMs);
    env.sustain(sustainLevel);
    env.release(releaseMs);
    env.hold(0);
    env.delay(0);
}

// using delay is not the best, as it will block other loops
void audioLoop() {
    if (io_playing && timerNote.check() == 1) {
        timerNote.reset();
        timerHold.reset();
        env.noteOn();
    }
    if (timerHold.check() == 1) {
        env.noteOff();
    }
}

#endif
