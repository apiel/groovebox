#ifndef IO_STATE_PATTERN_H_
#define IO_STATE_PATTERN_H_

#include <Arduino.h>

byte currentPattern = 0;

void setCurrentPattern(int8_t direction) {
    currentPattern += direction;
}

#endif
