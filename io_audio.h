#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_audio_synth.h"
#include "io_audio_synth_storage.h"
#include "io_audio_wav.h"

AudioOutputMQS audioOut;

AudioConnection* patchCordSynth[SYNTH_COUNT];
AudioMixer4 mixer[3];
AudioConnection* patchCordMixer[3];

void initSynth(byte synthPos) {
    synth[synthPos].init();
    loadSynth(synthPos);
}

void audioInit() {
    AudioMemory(25);

    // should it be 8 synth wavetable + waveform
    // or 6 wavetable and 2 waveform
    // or 4 synth wavetable + waveform and 2 waveform and 2 sd wav samples
    initSynth(0);
    initSynth(1);
    initSynth(2);
    initSynth(3);
    initSynth(4);
    initSynth(5);

    patchCordSynth[0] = new AudioConnection(synth[0], audioOut);

    patchCordSynth[0] = new AudioConnection(synth[0], 0, mixer[1], 0);
    patchCordSynth[1] = new AudioConnection(synth[1], 0, mixer[1], 1);
    patchCordSynth[2] = new AudioConnection(synth[2], 0, mixer[1], 2);
    patchCordSynth[3] = new AudioConnection(synth[3], 0, mixer[1], 3);
    patchCordSynth[4] = new AudioConnection(synth[4], 0, mixer[2], 0);
    patchCordSynth[5] = new AudioConnection(synth[5], 0, mixer[2], 1);

    patchCordMixer[0] = new AudioConnection(mixer[0], audioOut);
    patchCordMixer[1] = new AudioConnection(mixer[1], mixer[0]);
    patchCordMixer[2] = new AudioConnection(mixer[2], mixer[0]);
}

#endif
