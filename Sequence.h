#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "Pattern.h"

enum { SEQ_NOTE_ON, SEQ_NOTE_OFF };

class Sequence {
   public:
    Pattern pattern;
    bool active = false;
    byte output = 0;
    byte currentStep = 0;
    Step lastStep;

    Sequence() {}

    Sequence* next() {
        if (handleNext && output) {
            if (lastStep.duration != 255) {
                lastStep.duration--;
                if (!lastStep.slide) {
                    nextOff();
                }
            }
            if (active) {
                Step* step = &pattern.steps[currentStep];
                if (step->duration) {
                    (*handleNext)(SEQ_NOTE_ON, output, step->note,
                                  step->velocity);
                    if (lastStep.duration != 255) {
                        lastStep.duration = 0;
                        nextOff();
                    }
                    lastStep.set(step);
                }
            }
            nextOff();
        }
        currentStep = (currentStep + 1) % pattern.stepCount;
        return this;
    }

    void nextOff() {
        if (lastStep.duration == 0) {
            (*handleNext)(SEQ_NOTE_OFF, output, lastStep.note,
                          lastStep.velocity);
            lastStep.duration = 255;
        }
    }

    Sequence* set(Pattern* p, byte _output) {
        output = _output;
        pattern.set(p);

        return this;
    }

    Sequence* toggle() {
        active = !active;
        return this;
    }

    Sequence* setNextHandler(void (*fptr)(byte type, byte output, byte note,
                                          byte velocity)) {
        handleNext = fptr;
        return this;
    }

   private:
    void (*handleNext)(byte type, byte output, byte note, byte velocity);
};

#endif
