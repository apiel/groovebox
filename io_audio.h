#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>
#include <Metro.h>

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

void audioPlay(bool playing) { io_playing = playing; }
bool audioIsPlaying() { return io_playing; }

// need to make a PR to get waveform count
void setNextWaveform(int8_t direction) {
    currentWaveform = mod(currentWaveform + direction, WAVEFORM_COUNT);
    waveform.begin(currentWaveform);
}

void audioInit() {
    AudioMemory(10);
    waveform.frequency(440);
    waveform.amplitude(1.0);
    waveform.begin(WAVEFORM_SINE);

    env.attack(9.2);
    env.hold(2.1);
    env.decay(31.4);
    env.sustain(0.6);
    env.release(84.5);
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
