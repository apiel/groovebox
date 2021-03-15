#ifndef IO_DISPLAY_UTIL_H_
#define IO_DISPLAY_UTIL_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_W 128  // OLED display width, in pixels
#define SCREEN_H 64  // OLED display height, in pixels

char buf[SCREEN_W];

void dprintxyAbs(Adafruit_SSD1306 * d, byte x, byte y, const char *str, ...) {
    d->setCursor(x, y);
    va_list argptr;
    va_start(argptr, str);
    vsnprintf(buf, SCREEN_W, str, argptr);
    va_end(argptr);
    d->print(buf);
}

void dprintxy(Adafruit_SSD1306 * d, byte x, byte y, const char *str, ...) {
    d->setCursor(6 * x, 8 * y);
    va_list argptr;
    va_start(argptr, str);
    vsnprintf(buf, SCREEN_W, str, argptr);
    va_end(argptr);
    d->print(buf);
}

void dprint(Adafruit_SSD1306 * d, const char *str, ...) {
    va_list argptr;
    va_start(argptr, str);
    vsnprintf(buf, SCREEN_W, str, argptr);
    va_end(argptr);
    d->print(buf);
}

void dprintln(Adafruit_SSD1306 * d, const char *str, ...) {
    va_list argptr;
    va_start(argptr, str);
    vsnprintf(buf, SCREEN_W, str, argptr);
    va_end(argptr);
    d->println(buf);
}

#endif
