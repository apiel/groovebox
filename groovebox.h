#ifndef GROOVEBOX_H_
#define GROOVEBOX_H_

#include <Arduino.h>

#include "midi.h"

void grooveboxInit() {
    Serial.println("grooveboxInit");

    midiInit();
}

void grooveboxLoop() { midiLoop(); }

#endif
