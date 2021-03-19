#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "Pattern.h"

class Sequence {
   public:
    byte output;
    Pattern pattern;

    Sequence() {}

    Sequence* next() { return this; }

    Sequence* set(Pattern* p, byte _output) {
        output = _output;
        pattern.set(p);

        return this;
    }

    bool isPlaying() {
        return true;
    }
};

#endif
