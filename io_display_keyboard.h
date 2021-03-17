#ifndef IO_DISPLAY_KEYBOARD_H_
#define IO_DISPLAY_KEYBOARD_H_

#include <Adafruit_SSD1306.h>

void displayKeyboard(Adafruit_SSD1306 * d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    d->println("Keyboard");
}

#endif
