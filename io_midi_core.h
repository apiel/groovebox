#ifndef IO_MIDI_CORE_H_
#define IO_MIDI_CORE_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#define MIDI_COUNT 4

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
MIDIDevice midi[MIDI_COUNT] = MIDIDevice(myusb);

void noteOn(byte channel, byte note, byte velocity) {
    for (byte n = 0; n < MIDI_COUNT; n++) {
        if (midi[n].idProduct()) {
            midi[n].sendNoteOn(note, velocity, channel);
            // Serial.println("noteon" + String(n) + " " + String(note) + " " +
            // String(velocity) + " " + String(channel));
        }
    }
}

void noteOff(byte channel, byte note, byte velocity) {
    for (byte n = 0; n < MIDI_COUNT; n++) {
        if (midi[n].idProduct()) {
            midi[n].sendNoteOff(note, velocity, channel);
            // Serial.println("noteoff" + String(n) + " " + String(note) + " " +
            // String(velocity) + " " + String(channel));
        }
    }
}

#endif
