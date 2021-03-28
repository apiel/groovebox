#ifndef IO_PATTERN_H_
#define IO_PATTERN_H_

#include <SD.h>

#include "Pattern.h"
#include "io_sequencer.h"
#include "io_state.h"
#include "io_pattern_storage.h"
#include "note.h"

Pattern * pattern;

byte currentPattern = 0;
byte currentStepSelection = 0;

void loadPattern() {
    loadPattern(currentPattern);
}

void setCurrentPattern(int8_t direction) {
    currentPattern += direction;
    loadPattern();
    pattern = &patterns[currentPattern];
}

void patternInit() { setCurrentPattern(0); }

void setSequence(bool start = false) {
    sequences[currentSequence].set(pattern, sequenceOutput);
    sequences[currentSequence].activate(start);
}

void setCurrentStepSelection(int8_t direction) {
    currentStepSelection = mod(currentStepSelection + direction, STEP_COUNT);
}

void setStepDuration(int8_t direction) {
    Step* pStep = &pattern->steps[currentStepSelection];
    // Instead of STEP_COUNT could try to find the next...
    pStep->duration = constrain(pStep->duration + direction, 0, STEP_COUNT);
}

// could use keyboard there
void setStepNote(int8_t direction) {
    Step* pStep = &pattern->steps[currentStepSelection];
    pStep->set(constrain(pStep->note + direction, _C0, _B8));
}

void setStepVelocity(int8_t direction) {
    Step* pStep = &pattern->steps[currentStepSelection];
    pStep->set(constrain(pStep->velocity + direction, 0, 127));
}

bool savePattern() {
    savePattern(currentPattern);
}

#endif
