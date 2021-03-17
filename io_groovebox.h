#ifndef IO_GROOVEBOX_H_
#define IO_GROOVEBOX_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_audio_synth.h"
#include "io_display.h"
#include "io_midi.h"

IO_Audio ioAudio;

void ioGrooveboxInit() {
    Serial.println("grooveboxInit");
    // delay(2000);
    midiInit(&ioAudio);
    displayInit();
    displayUpdate();
}

void ioGrooveboxLoop() {
    midiLoop();
    ioAudio.loop();
}

#endif
