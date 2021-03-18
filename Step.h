#ifndef STEP_H_
#define STEP_H_

#include "note.h"

#define BASE_FREQ _C4

class Step {
   public:
    byte note = BASE_FREQ;
    byte duration = 0;
    // byte velocity;
    bool slide = false;
    int freqDiff = 0;

    Step() {}

    void clear() { duration = 0; }

    void set(byte _note) {
        note = _note;
        freqDiff = NOTE_FREQ[BASE_FREQ] - NOTE_FREQ[note];
    }

    void set(byte _note, byte _duration) {
        duration = _duration;
        set(_note);
    }

    void set(byte _note, byte _duration, bool _slide) {
        slide = _slide;
        set(_note, _duration);
    }

    // Step(byte _note, byte _duration, bool _slide) {
    //     note = _note;
    //     duration = _duration;
    //     slide = _slide;
    // }
};

#endif
