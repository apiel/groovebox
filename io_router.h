#ifndef IO_ROUTER_H_
#define IO_ROUTER_H_

#include <Arduino.h>

#include "io_util.h"

#define ROUTER_INPUT_COUNT 17 // 0 + 16 midi channels
#define ROUTER_OUTPUT_COUNT 17  // might the same as SEQUENCE_OUTPUT_COUNT
#define ROUTER_COUNT 4

int routerInputs[ROUTER_COUNT] = {11, 10, 0, 0};
int routerOutputs[ROUTER_COUNT] = {2, 2, 0, 0};

void setRouterInput(byte pos, int8_t direction) {
    routerInputs[pos] = mod(routerInputs[pos] + direction, ROUTER_INPUT_COUNT);
}

void setRouterOutput(byte pos, int8_t direction) {
    routerOutputs[pos] = mod(routerOutputs[pos] + direction, ROUTER_OUTPUT_COUNT);
}

#endif
