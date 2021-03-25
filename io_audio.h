#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_audio_synth.h"

#define SYNTH_COUNT 2

AudioOutputMQS audioOut;

IO_AudioSynth synth[SYNTH_COUNT];
AudioConnection*
    patchCordSynth[SYNTH_COUNT];
AudioMixer4 mixer[3];
AudioConnection* patchCordMixer[3];

byte currentSynth = 0;

void audioInit() {
    AudioMemory(10);

    synth[0].init();
    synth[1].init();
    // synth[2].init();
    // synth[3].init();
    // synth[4].init();
    // synth[5].init();
    // synth[6].init();
    // synth[7].init();

    // patchCordSynth[0] = new AudioConnection(synth[0], audioOut);

    patchCordSynth[0] = new AudioConnection(synth[0], 0, mixer[1], 0);
    patchCordSynth[1] = new AudioConnection(synth[1], 0, mixer[1], 1);
    // patchCordSynth[2] = new AudioConnection(synth[2], 0, mixer[1], 2);
    // patchCordSynth[3] = new AudioConnection(synth[3], 0, mixer[1], 3);
    // patchCordSynth[4] = new AudioConnection(synth[4], 0, mixer[2], 0);
    // patchCordSynth[5] = new AudioConnection(synth[5], 0, mixer[2], 1);
    // patchCordSynth[6] = new AudioConnection(synth[6], 0, mixer[2], 2);
    // patchCordSynth[7] = new AudioConnection(synth[7], 0, mixer[2], 3);

    patchCordMixer[0] = new AudioConnection(mixer[0], audioOut);
    patchCordMixer[1] = new AudioConnection(mixer[1], mixer[0]);
    patchCordMixer[2] = new AudioConnection(mixer[2], mixer[0]);
}

#endif
