#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_audio.h"
#include "io_display.h"
#include "io_midi_default.h"
#include "io_midi_pattern.h"
#include "io_midi_sequences.h"
#include "io_midi_synth.h"
#include "io_midi_util.h"

#define MIDI_COUNT 4

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
MIDIDevice midi[MIDI_COUNT] = MIDIDevice(myusb);

void noteOn(byte channel, byte note, byte velocity) {
    for (byte n = 0; n < MIDI_COUNT; n++) {
        midi[n].sendNoteOn(note, velocity, channel);
    }
}

void noteOff(byte channel, byte note, byte velocity) {
    for (byte n = 0; n < MIDI_COUNT; n++) {
        midi[n].sendNoteOff(note, velocity, channel);
    }
}

void noteOnHandler(byte channel, byte note, byte velocity) {
    // When a USB device with multiple virtual cables is used,
    // midi[n].getCable() can be used to read which of the virtual
    // MIDI cables received this message.
    Serial.print("Note On, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);
    if (channel == 1) {
        noteOn(2, note, velocity);
    } else if (!defaultNoteOnHandler(channel, note, velocity)) {
        if (currentView == VIEW_PATTERN) {
            patternNoteOnHandler(channel, note, velocity);
        } else if (currentView == VIEW_SYNTH) {
            synthNoteOnHandler(channel, note, velocity);
        } else if (currentView == VIEW_SEQUENCES) {
            sequencesNoteOnHandler(channel, note, velocity);
        }
        displayUpdate();
    }
}

void noteOffHandler(byte channel, byte note, byte velocity) {
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    if (channel == 1) {
        noteOff(2, note, velocity);
    } else {
        if (currentView == VIEW_PATTERN) {
            patternNoteOffHandler(channel, note, velocity);
        } else if (currentView == VIEW_SYNTH) {
            synthNoteOffHandler(channel, note, velocity);
        } else if (currentView == VIEW_SEQUENCES) {
            sequencesNoteOffHandler(channel, note, velocity);
        }
        displayUpdate();
    }
}

void controlChangeHandler(byte channel, byte control, byte value) {
    Serial.print("Control Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", value=");
    Serial.println(value, DEC);

    byte knob = control % KNOB_COUNT;
    int8_t direction = getKnobDirection(knob, value);
    if (currentView == VIEW_PATTERN) {
        patternControlChangeHandler(channel, knob, direction);
    } else if (currentView == VIEW_SYNTH) {
        synthControlChangeHandler(channel, knob, direction);
    } else if (currentView == VIEW_SEQUENCES) {
        sequencesControlChangeHandler(channel, knob, direction);
    }
    displayUpdate();
}

void afterTouchPolyHandler(uint8_t channel, uint8_t note, uint8_t pressure) {
    Serial.print("AfterTouchPoly, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", pressure=");
    Serial.println(pressure, DEC);
}

void sysExHandler(const uint8_t* data, uint16_t length, bool complete) {
    Serial.println("some sysExHandler data");
    // Serial.println(data);
}

void midiInit() {
    myusb.begin();
    for (byte n = 0; n < MIDI_COUNT; n++) {
        midi[n].setHandleNoteOn(noteOnHandler);
        midi[n].setHandleNoteOff(noteOffHandler);
        midi[n].setHandleControlChange(controlChangeHandler);
        midi[n].setHandleAfterTouchPoly(afterTouchPolyHandler);
        midi[n].setHandleSysEx(sysExHandler);

        // if (midi[n].manufacturer() != NULL && midi[n].product() != NULL) {
        //     String make = (char*)midi[n].manufacturer();
        //     String model = (char*)midi[n].product();
        //     Serial.print(String(make) + " " + model + " ");
        // }
    }
}

void midiLoop() {
    myusb.Task();
    for (byte n = 0; n < MIDI_COUNT; n++) {
        midi[n].read();
    }
}

#endif
