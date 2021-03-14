#ifndef IO_GROOVEBOX_H_
#define IO_GROOVEBOX_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_audio_synth.h"
#include "io_midi.h"

IO_Audio ioAudio;

AudioConnection patchCord01(ioAudio.synth.lfoMod, ioAudio.synth.waveform);
AudioConnection patchCord02(ioAudio.synth.dc, ioAudio.synth.envMod);
AudioConnection patchCord03(ioAudio.synth.envMod, 0, ioAudio.synth.waveform, 1);
AudioConnection patchCord05(ioAudio.synth.waveform, ioAudio.synth.env);
AudioConnection patchCord06(ioAudio.synth.env, ioAudio.synth.filter);
AudioConnection patchCordFilterOutLowPass(ioAudio.synth.filter, 0, ioAudio.audioOut, 0);
AudioConnection patchCordFilterOutBandPass(ioAudio.synth.filter, 1, ioAudio.audioOut, 0);
AudioConnection patchCordFilterOutHighPass(ioAudio.synth.filter, 3, ioAudio.audioOut, 0);

void ioGrooveboxInit() {
    Serial.println("grooveboxInit");

    midiInit(&ioAudio);
}

void ioGrooveboxLoop() {
    midiLoop();
    ioAudio.loop();
}

#endif
