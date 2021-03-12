#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_audio.h"

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice midi1(myusb);

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

    if (note == 22) {
        audioPlay(!audioIsPlaying());
    }
}

void noteOffHandler(byte channel, byte note, byte velocity) {
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);
}

void controlChangeHandler(byte channel, byte control, byte value) {
    Serial.print("Control Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", value=");
    Serial.println(value, DEC);
}

void midiInit() {
    myusb.begin();
    midi1.setHandleNoteOn(noteOnHandler);
    midi1.setHandleNoteOff(noteOffHandler);
    midi1.setHandleControlChange(controlChangeHandler);

    // midi1.setHandleClock(myClock);
    // midi1.setHandleStart(myStart);
    // midi1.setHandleContinue(myContinue);
    // midi1.setHandleStop(myStop);
}

void midiLoop() {
    myusb.Task();
    midi1.read();
}

#endif
