#ifndef IO_GROOVEBOX_H_
#define IO_GROOVEBOX_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_midi.h"

void ioGrooveboxInit() {
    Serial.println("grooveboxInit");

    midiInit();
    audioInit();
}

void ioGrooveboxLoop() {
    midiLoop();
    audioLoop();
}

#endif
