#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_audio.h"
#include "io_display.h"
#include "io_midi_default.h"
#include "io_midi_util.h"
#include "io_midi_pattern.h"
#include "io_midi_sequences.h"
#include "io_midi_synth.h"

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
MIDIDevice midi1(myusb);
MIDIDevice midi2(myusb);
// might need to had more, maybe even crate an array

void noteOnHandler(byte channel, byte note, byte velocity) {
    // When a USB device with multiple virtual cables is used,
    // midi1.getCable() can be used to read which of the virtual
    // MIDI cables received this message.
    Serial.print("Note On, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);
    if (channel == 1) {
        midi1.sendNoteOn(note, velocity, 2);
        midi2.sendNoteOn(note, velocity, 2);
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
        midi1.sendNoteOff(note, velocity, 2);
        midi2.sendNoteOff(note, velocity, 2);
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
    midi1.setHandleNoteOn(noteOnHandler);
    midi1.setHandleNoteOff(noteOffHandler);
    midi1.setHandleControlChange(controlChangeHandler);
    midi1.setHandleAfterTouchPoly(afterTouchPolyHandler);
    midi1.setHandleSysEx(sysExHandler);
    // Serial.print("Midi1 channel: ");
    // Serial.println(midi1.getChannel());

    // if (midi1.manufacturer() != NULL && midi1.product() != NULL) {
    //     String make = (char*)midi1.manufacturer();
    //     String model = (char*)midi1.product();
    //     Serial.print(String(make) + " " + model + " ");
    // }

    midi2.setHandleNoteOn(noteOnHandler);
    midi2.setHandleNoteOff(noteOffHandler);
    midi2.setHandleControlChange(controlChangeHandler);
    midi2.setHandleAfterTouchPoly(afterTouchPolyHandler);
    midi2.setHandleSysEx(sysExHandler);
    // Serial.print("Midi2 channel: ");
    // Serial.println(midi2.getChannel());

    // midi1.setHandleClock(myClock);
    // midi1.setHandleStart(myStart);
    // midi1.setHandleContinue(myContinue);
    // midi1.setHandleStop(myStop);
}

void midiLoop() {
    myusb.Task();
    midi1.read();
    midi2.read();
}

#endif
