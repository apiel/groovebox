#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_audio_synth.h"

#define SYNTH_COUNT 1

AudioOutputMQS audioOut;

IO_AudioSynth synth[SYNTH_COUNT];
AudioConnection* patchCord[1]; // this will be dependent on SYNTH_COUNT

byte currentSynth = 0;

void audioInit() {
    AudioMemory(10);

    synth[0].waveTable.load("raw/kick.raw");

    byte pci = 0;  // used only for adding new patchcords
    patchCord[pci++] = new AudioConnection(synth[0], audioOut);
}

#endif
