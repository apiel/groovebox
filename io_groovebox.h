#ifndef IO_GROOVEBOX_H_
#define IO_GROOVEBOX_H_

#include <Arduino.h>
#include <SD.h>

#include "io_audio.h"
#include "io_audio_synth.h"
#include "io_display.h"
#include "io_midi.h"
#include "io_state.h"

#define SDCARD_CS_PIN BUILTIN_SDCARD

IO_Audio ioAudio;

void ioGrooveboxInit() {
    Serial.println("grooveboxInit");
    displayInit();
    // delay(2000);
    midiInit(&ioAudio);

    if (!(SD.begin(SDCARD_CS_PIN))) {
        Serial.println("Unable to access the SD card");
        sdAvailable = false;
    }

    displayUpdate();
}

void ioGrooveboxLoop() {
    midiLoop();
    ioAudio.loop();
}

#endif
