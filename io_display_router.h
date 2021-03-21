#ifndef IO_DISPLAY_ROUTER_H_
#define IO_DISPLAY_ROUTER_H_

#include <Adafruit_SSD1306.h>

#include "io_router.h"

void displayRouter(Adafruit_SSD1306* d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    d->println("Router\n");

    for (byte i = 0; i < ROUTER_COUNT; i++) {
        d->printf("in %02d out %02d\n", routerInputs[i], routerOutputs[i]);
    }
}

#endif
