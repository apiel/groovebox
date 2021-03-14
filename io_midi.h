#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_audio.h"
#include "io_midi_knob.h"

IO_Audio * audio;

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
        audio->synth.noteOn();
    }
}

void noteOffHandler(byte channel, byte note, byte velocity) {
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    if (note == 22) {
        audio->synth.noteOff();
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

    if (knob == 1) {
        audio->synth.setNextWaveform(direction);
    } else if (knob == 2) {
        audio->synth.setFrequency(direction);
    } else if (knob == 3) {
        audio->synth.setAmplitude(direction);
    } else if (knob == 5) {
        audio->synth.setAttack(direction);
    } else if (knob == 6) {
        audio->synth.setDecay(direction);
    } else if (knob == 7) {
        audio->synth.setSustain(direction);
    } else if (knob == 8) {
        audio->synth.setRelease(direction);
    } else if (knob == 11) {
        audio->synth.setFilterFrequency(direction);
    } else if (knob == 12) {
        audio->synth.setFilterResonance(direction);
    } else if (knob == 13) {
        audio->synth.setFilterOctaveControl(direction);
    } else if (knob == 14) {
        audio->synth.setCurrentFilter(direction);
    }
}

void midiInit(IO_Audio * ptIoAudio) {
    audio = ptIoAudio;
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
