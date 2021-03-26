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
#include "io_sequencer.h"
#include "io_state.h"

void ioGrooveboxInit() {
    // // to comment out
    // while (!Serial)
    //     ;

    Serial.println("grooveboxInit");
    displayInit();

    if (!(SD.begin(SDCARD_CS_PIN))) {
        Serial.println("Unable to access the SD card");
        sdAvailable = false;
    }

    // delay(2000);
    audioInit();
    midiInit();
    patternInit();
    sequencerInit();

    displayUpdate();
}

unsigned long last_time = millis();
void ioGrooveboxLoop() {
    midiLoop();
    sequencerLoop();
    displayLoop();

    if (true) {
        if (millis() - last_time >= 5000) {
            Serial.print("Proc = ");
            Serial.print(AudioProcessorUsage());
            Serial.print(" (");
            Serial.print(AudioProcessorUsageMax());
            Serial.print("),  Mem = ");
            Serial.print(AudioMemoryUsage());
            Serial.print(" (");
            Serial.print(AudioMemoryUsageMax());
            Serial.println(")");
            last_time = millis();
        }
    }
}

#endif
