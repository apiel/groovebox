#ifndef IO_MIDI_ROUTER_H_
#define IO_MIDI_ROUTER_H_

#include <Arduino.h>

#include "io_router.h"

void routerNoteOnHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
    }
}

void routerNoteOffHandler(byte channel, byte note, byte velocity) {
    if (channel == 11) {
    }
}

void routerControlChangeHandler(byte channel, byte knob, int8_t direction) {
    if (channel == 11) {
        if (knob == 1) {
            setRouterInput(0, direction);
        } else if (knob == 2) {
            setRouterOutput(0, direction);
        } else if (knob == 3) {
            setRouterInput(1, direction);
        } else if (knob == 4) {
            setRouterOutput(1, direction);
        } else if (knob == 5) {
            setRouterInput(2, direction);
        } else if (knob == 6) {
            setRouterOutput(2, direction);
        } else if (knob == 7) {
            setRouterInput(3, direction);
        } else if (knob == 8) {
            setRouterOutput(3, direction);
        }
    }
}

#endif
