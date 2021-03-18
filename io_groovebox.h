#ifndef IO_GROOVEBOX_H_
#define IO_GROOVEBOX_H_

#include <Arduino.h>
#include <SD.h>

#include "io_audio.h"
#include "io_audio_synth.h"
#include "io_config.h"
#include "io_display.h"
#include "io_midi.h"
#include "io_pattern.h"
#include "io_state.h"

IO_Audio ioAudio;

void ioGrooveboxInit() {
    Serial.println("grooveboxInit");
    displayInit();
    if (!(SD.begin(SDCARD_CS_PIN))) {
        Serial.println("Unable to access the SD card");
        sdAvailable = false;
    }

    // delay(2000);
    midiInit(&ioAudio);
    patternInit();

    displayUpdate();
}

void ioGrooveboxLoop() {
    midiLoop();
    ioAudio.loop();
}

#endif
