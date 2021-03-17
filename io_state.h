#ifndef IO_STATE_H_
#define IO_STATE_H_

#include <Arduino.h>

enum { VIEW_TONE, VIEW_PATTERN, VIEW_KEYBOARD, VIEW_COUNT };

byte currentView = VIEW_TONE;
bool mcMode = false;

void toggleGcMode() {
        mcMode = !mcMode;
}

// ToDo: when button pressed, possibilty to press 1-8 button to directly
// select the view. Also while it is pressed the 1-8 button should light up
// to display the current selection
void setCurrentView(int8_t direction) {
    currentView = mod(currentView + direction, VIEW_COUNT);
    Serial.print("setCurrentView=");
    Serial.println(currentView);
}

#endif
