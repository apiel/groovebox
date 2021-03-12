#ifndef IO_UTIL_H_
#define IO_UTIL_H_

#include <Arduino.h>

int between(int val, int minVal, int maxVal) {
    return min(max(val, minVal), maxVal);
}

int mod(int n, int m) { return ((n % m) + m) % m; }

#endif
