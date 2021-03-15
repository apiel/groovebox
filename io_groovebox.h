#ifndef IO_GROOVEBOX_H_
#define IO_GROOVEBOX_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_audio_synth.h"
#include "io_midi.h"

AudioOutputMQS audioOut;
IO_Audio ioAudio;

AudioConnection patchCord01(ioAudio.synth, audioOut);

void ioGrooveboxInit() {
    Serial.println("grooveboxInit");
    midiInit(&ioAudio);
}

void ioGrooveboxLoop() {
    midiLoop();
    ioAudio.loop();
}

#endif
